#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "PredictionTrie.h"
#include "mycompleter.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _data.read("data/data.csv");
    for (size_t i = 0; i < _data.getRowsCount(); i++) {
        _ptrie.insert(_data[i][0], std::stoi(_data[i][1]));
    }

    completer = new MyCompleter(ui->te_Text, _ptrie);
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
