#include "mycompleter.h"

#include <QKeyEvent>
#include <QDebug>
#include <sstream>
#include <vector>

using std::string;
using std::vector;


string getLastWord(QPlainTextEdit* textEdit) {
    QTextCursor tc = textEdit->textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    QString word = tc.selectedText();
    return word.toStdString();
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
        int key = keyEvent->key();
        if (key == Qt::Key_Down && !isHidden()) {
            setCurrentRow((currentRow() + 1) % count());
            return true;
        }
        else if (key == Qt::Key_Up && !isHidden()) {
            setCurrentRow((currentRow() + count() - 1) % count());
            return true;
        }
        else if (key == Qt::Key_Return && currentRow() != -1 && !isHidden()) {
            string lastWord = getLastWord(_textEdit);
            string word = currentItem()->text().toStdString();
            string completion = word.substr(lastWord.size(), word.size() - lastWord.size());
            _textEdit->insertPlainText(QString::fromStdString(completion));
            _ptrie->insert(word);
            return true;
        }
        else if (key == Qt::Key_Space || key == Qt::Key_Return) {
            string lastWord = getLastWord(_textEdit);
            if (!lastWord.empty()) {
                _ptrie->insert(lastWord);
            }
        }
        else if (keyEvent->key() == Qt::Key_Escape) {
            hide();
        }
    }
    return QWidget::eventFilter(object,event);
}

void MyCompleter::textChanged() {
    refresh();
}

void MyCompleter::refresh() {
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
    string word = getLastWord(_textEdit);
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

