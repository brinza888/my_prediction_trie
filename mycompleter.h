#ifndef MYCOMPLETER_H
#define MYCOMPLETER_H

#include <QListWidget>
#include <QPlainTextEdit>
#include <string>

#include "PredictionTrie.h"


std::string getLastWord(QPlainTextEdit* textEdit);


class MyCompleter : public QListWidget
{
    Q_OBJECT

public:
    MyCompleter(QPlainTextEdit* parent, PredictionTrie* ptrie);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void textChanged();
    void cursorPositionChanged();

private:
    PredictionTrie* _ptrie;
    QPlainTextEdit* _textEdit;
};

#endif // MYCOMPLETER_H
