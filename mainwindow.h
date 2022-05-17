#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "csvparser.h"
#include "PredictionTrie.h"


string getLastWord(string& text);


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_te_Text_textChanged();

    void on_te_Text_cursorPositionChanged();

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    Ui::MainWindow *ui;
    QListWidget* _hintList;

    CSVParser _data;
    PredictionTrie _ptrie;
};
#endif // MAINWINDOW_H
