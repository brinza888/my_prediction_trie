#ifndef MYCOMPLETER_H
#define MYCOMPLETER_H

#include <QListWidget>
#include <QTextEdit>
#include <string>

#include "PredictionTrie.h"


std::string getLastWord(std::string& text);


class MyCompleter : public QListWidget
{
    Q_OBJECT

public:
    MyCompleter(QTextEdit* parent, PredictionTrie& ptrie);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void textChanged();
    void cursorPositionChanged();

private:
    PredictionTrie* _ptrie;
    QTextEdit* _textEdit;
};

#endif // MYCOMPLETER_H
