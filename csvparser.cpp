#include "csvparser.h"
#include <fstream>
#include <sstream>
#include <stdexcept>


string CSVParser::getPath() const {
    return _filePath;
}

void CSVParser::read(string filePath) {
    _table.clear();

    std::ifstream file;
    file.open(filePath);
    _filePath = filePath;
    if (!file.is_open()) {
        throw std::invalid_argument("Файл не найден: " + _filePath);
    }

    string line;
    string value;
    std::getline(file, line);
    std::stringstream lineStream(line);
    _table.push_back(vector<string>());

    do {
        std::getline(lineStream, value, ',');
        _columnsCount += 1;
        _table.back().push_back(value);
    } while (!lineStream.eof());

    do {
        std::getline(file, line);
        _table.push_back(vector<string>());
        lineStream = std::stringstream(line);
        for (size_t i = 0; i < _columnsCount; i++) {
            std::getline(lineStream, value, ',');
            _table.back().push_back(value);
        }
    } while (!file.eof());

    file.close();
}

void CSVParser::write(std::string filePath) {
    if (!filePath.size()) {
        filePath = _filePath;
    }
    std::ofstream file;
    file.open(filePath);
    for (size_t rowId = 0; rowId < _table.size(); rowId++) {
        for (size_t colId = 0; colId < _columnsCount; colId++) {
            file << _table[rowId][colId];
            if (colId < _columnsCount - 1) {
                file << ',';
            }
            else if (rowId < _table.size() - 1) {
                file << '\n';
            }
        }
    }
    file.close();
}

vector<string>& CSVParser::getHeader() {
    return _table.front();
}

size_t CSVParser::getColumnIndex(const string& columnName) {
    vector<string> header = getHeader();
    for (size_t idx = 0; idx < header.size(); idx++) {
        if (header[idx] == columnName) {
            return idx;
        }
    }
    throw std::invalid_argument("Такой столбец не найден: " + columnName);
}

size_t CSVParser::findFirst(const vector<string>& querry) const {
    bool result;
    for (size_t idx = 1; idx < _table.size(); idx++) {
        result = true;
        for (size_t qId = 0; qId < querry.size(); qId++) {
            if (!querry[qId].size()) {
                continue;
            }
            result = result && (_table[idx][qId] == querry[qId]);
        }
        if (result) {
            return idx - 1;
        }
    }
    string querryStr;
    for (auto&& el: querry) {
        querryStr += el + ",";
    }
    throw std::invalid_argument("В файле: " + _filePath  + "\nПо запросу: (" + querryStr + ")");
}

vector<string>& CSVParser::operator [](const size_t idx) {
    return _table[idx + 1];
}

vector<string> CSVParser::operator [](const size_t idx) const {
    return _table[idx + 1];
}

size_t CSVParser::getColumnsCount() const {
    return _columnsCount;
}

size_t CSVParser::getRowsCount() const {
    return _table.size() - 1;
}
