#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <sstream>

#include "PredictionTrie.h"


string getLastWord(string& text) {
    std::stringstream stext;
    stext << text;
    string word;
    while (std::getline(stext, word, ' '));
    return word;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _hintList = new QListWidget(ui->te_Text);
    _hintList->show();
    _hintList->resize(150, 70);
    _hintList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _hintList->setFont(QFont("Sans Serif", 12));

    ui->te_Text->installEventFilter(this);

    _data.read("data/data.csv");
    for (size_t i = 0; i < _data.getRowsCount(); i++) {
        _ptrie.insert(_data[i][0], std::stoi(_data[i][1]));
    }
}

MainWindow::~MainWindow()
{
    vector<MatchedPair> allWords = _ptrie.allWordsStartedWith("");
    for (auto&& [word, points]: allWords) {
        vector<string> row({word, std::to_string(points)});
        try {
            size_t idx = _data.findFirst(vector<string>({word}));
            _data[idx] = row;
        }
        catch (std::invalid_argument) {
            _data.newRow(row);
        }
    }
    _data.write();
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (object == ui->te_Text && (event->type() == QEvent::KeyPress || event->type() == QEvent::Enter)) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int curRow = _hintList->currentRow();
        int count = _hintList->count();
        if (keyEvent->key() == Qt::Key_Down) {
            _hintList->setCurrentRow((curRow + 1) % count);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Up) {
            _hintList->setCurrentRow((curRow + count - 1) % count);
        }
        else if (keyEvent->key() == Qt::Key_Return && curRow != -1) {
            string text = ui->te_Text->toPlainText().toStdString();
            string word = _hintList->currentItem()->text().toStdString();
            string lastWord = getLastWord(text);
            string completion = word.substr(lastWord.size(), word.size() - lastWord.size());
            ui->te_Text->insertPlainText(QString::fromStdString(completion + " "));
            _ptrie.insert(word);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Escape) {
            _hintList->hide();
        }
    }
    return QWidget::eventFilter(object,event);
}

void MainWindow::on_te_Text_textChanged()
{
    QPoint cursor = ui->te_Text->cursorRect().topLeft();
    if (cursor.x() + _hintList->width() > ui->te_Text->rect().topRight().x()) {
        cursor.setX(cursor.x() - _hintList->width());
    }
    if (cursor.y() + _hintList->height() > ui->te_Text->rect().bottomLeft().y()) {
        cursor.setY(cursor.y() - _hintList->height() - 20);
    }
    else {
        cursor.setY(cursor.y() + 20);
    }
    _hintList->move(cursor);
    string text = ui->te_Text->toPlainText().toStdString();
    string word = getLastWord(text);
    vector<string> bestMatches = _ptrie.findBestMatches(word, 5);
    _hintList->clear();
    for (auto&& word: bestMatches) {
        _hintList->addItem(QString::fromStdString(word));
    }
    _hintList->show();
}

void MainWindow::on_te_Text_cursorPositionChanged()
{
    _hintList->hide();
}
