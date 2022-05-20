#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "PredictionTrie.h"
#include "mycompleter.h"

#include <QDebug>
#include <QCloseEvent>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    completer = new MyCompleter(ui->te_Text, &_ptrie);
}

MainWindow::~MainWindow()
{
    delete ui;
}
