#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDebug>
#include <QKeyEvent>
#include <sstream>
#include "PredictionTrie.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _hintList = new QListWidget(ui->te_Text);
    _hintList->show();
    _hintList->resize(150, 50);
    _hintList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _hintList->setFont(QFont("Sans Serif", 12));

    ui->te_Text->installEventFilter(this);

    _data.read("data/data.csv");
    for (size_t i = 0; i < _data.getRowsCount(); i++) {
        _ptrie.insert(_data[i][0]);
    }
}

MainWindow::~MainWindow()
{
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
            ui->te_Text->insertPlainText(_hintList->currentItem()->text() + " ");
            return true;
        }
    }
    return QWidget::eventFilter(object,event);
}

void MainWindow::on_te_Text_textChanged()
{
    QPoint cursor = ui->te_Text->cursorRect().topLeft();
    cursor.setY(cursor.y() + 20);
    if (cursor.x() + _hintList->width() <= ui->te_Text->rect().topRight().x()) {
        _hintList->move(cursor);
    }
    else {
        _hintList->move(cursor.x() - _hintList->width(), cursor.y());
    }
    std::stringstream stext;
    stext << ui->te_Text->toPlainText().toStdString();
    string word;
    while (std::getline(stext, word, ' '));
    vector<string> bestMatches = _ptrie.findBestMatches(word, 5);
    _hintList->clear();
    for (auto&& word: bestMatches) {
        _hintList->addItem(QString::fromStdString(word));
    }
}
