#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <vector>
#include <string>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;


class CSVParser
{
public:
    CSVParser() = default;

    string getPath() const;

    void read(string filePath);
    void write(string filePath = "") const;

    vector<string> getHeader() const;

    size_t getColumnIndex(const string& columnName) const;

    size_t getColumnsCount() const;
    size_t getRowsCount() const;

    size_t findFirst(const vector<string>& querry) const;

    void newRow(const vector<string>& values);

    vector<string>& operator[](const size_t idx);
    vector<string> operator [](const size_t idx) const;

private:
    string _filePath;
    size_t _columnsCount = 0;

    vector<vector<string>> _table;
};

#endif // CSVPARSER_H
