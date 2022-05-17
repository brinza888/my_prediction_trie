#include "mycompleter.h"

#include <QKeyEvent>
#include <QDebug>
#include <sstream>
#include <vector>

using std::string;
using std::vector;


string getLastWord(string& text) {
    std::stringstream stext;
    stext << text;
    string word;
    while (std::getline(stext, word, ' '));
    return word;
}


MyCompleter::MyCompleter(QPlainTextEdit* parent, PredictionTrie* ptrie) :
    QListWidget((QWidget*) parent)
{
    _ptrie = ptrie;
    _textEdit = parent;

    resize(150, 70);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFont(QFont("Sans Serif", 12));

    _textEdit->installEventFilter(this);
    connect(_textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
    connect(_textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
}

bool MyCompleter::eventFilter(QObject *object, QEvent *event) {
    if (object == _textEdit && (event->type() == QEvent::KeyPress || event->type() == QEvent::Enter)) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Down) {
            setCurrentRow((currentRow() + 1) % count());
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Up) {
            setCurrentRow((currentRow() + count() - 1) % count());
        }
        else if (keyEvent->key() == Qt::Key_Return && currentRow() != -1) {
            string text = _textEdit->toPlainText().toStdString();
            string lastWord = getLastWord(text);
            string word = currentItem()->text().toStdString();
            string completion = word.substr(lastWord.size(), word.size() - lastWord.size());
            _textEdit->insertPlainText(QString::fromStdString(completion + " "));
            _ptrie->insert(word);
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Space) {
            string text = _textEdit->toPlainText().toStdString();
            string lastWord = getLastWord(text);
            _ptrie->insert(lastWord);
        }
        else if (keyEvent->key() == Qt::Key_Escape) {
            hide();
        }
    }
    return QWidget::eventFilter(object,event);
}

void MyCompleter::textChanged()
{
    QPoint cursor = _textEdit->cursorRect().topLeft();
    if (cursor.x() + width() > _textEdit->rect().topRight().x()) {
        cursor.setX(cursor.x() - width());
    }
    if (cursor.y() + height() > _textEdit->rect().bottomLeft().y()) {
        cursor.setY(cursor.y() - height() - 20);
    }
    else {
        cursor.setY(cursor.y() + 20);
    }
    move(cursor);
    string text = _textEdit->toPlainText().toStdString();
    string word = getLastWord(text);
    vector<string> bestMatches = _ptrie->findBestMatches(word, 10);
    clear();
    for (auto&& word: bestMatches) {
        addItem(QString::fromStdString(word));
    }
    show();
}

void MyCompleter::cursorPositionChanged()
{
    hide();
}

