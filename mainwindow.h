#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

#include "PredictionTrie.h"
#include "mycompleter.h"


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
    void clearClicked();

private:
    Ui::MainWindow *ui;

    MyCompleter* completer;
    PredictionTrie _ptrie;
};
#endif // MAINWINDOW_H
