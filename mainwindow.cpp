#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "PredictionTrie.h"
#include "mycompleter.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    completer = new MyCompleter(ui->te_Text, &_ptrie);

    connect(ui->pb_Clear, SIGNAL(clicked()), this, SLOT(clearClicked()));
}

void MainWindow::clearClicked() {
    _ptrie.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}
