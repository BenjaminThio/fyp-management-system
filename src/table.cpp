#include <iostream>
#include <string>
#include <vector>
using namespace std;

string generateTable(vector<vector<string>> table) {
    string renderer = "";
    vector<int> columnWidths;

    if (table.size() > 0) {
        for (int i = 0; i < table[0].size(); i++) {
            columnWidths.push_back(0);
        }
    }

    for (int rowIndex = 0; rowIndex < table.size(); rowIndex++) {
        vector<string> row = table[rowIndex];

        for (int cellIndex = 0; cellIndex < row.size(); cellIndex++) {
            string cell = row[cellIndex];

            if (cell.length() > columnWidths[cellIndex]) {
                columnWidths[cellIndex] = cell.length();
            } 
        }
    }

    for (int rowIndex = 0; rowIndex < table.size(); rowIndex++) {
        vector<string> row = table[rowIndex];

        for (int cellIndex = 0; cellIndex < row.size(); cellIndex++) {
            string cell = row[cellIndex];

            renderer += cell;
            if (columnWidths[cellIndex] - cell.length() > 0) {
                for (int extraWidth = 0; extraWidth < columnWidths[cellIndex] - cell.length(); extraWidth++) {
                    renderer += ' ';
                };
            }
            renderer += ' ';
        }
        renderer += '\n';
    }
    
    return renderer;
}