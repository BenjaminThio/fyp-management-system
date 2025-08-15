#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

string generate_table(vector<vector<string>> table) {
    ostringstream renderer;
    vector<int> column_widths;
    int longest_row = 0;

    for (int i = 0; i < table.size(); i++) {
        vector<string> row = table[i];

        if (row.size() > longest_row) longest_row = row.size();
    }

    for (int i = 0; i < longest_row; i++) column_widths.push_back(0);

    for (int row_index = 0; row_index < table.size(); row_index++) {
        vector<string> row = table[row_index];

        for (int cell_index = 0; cell_index < row.size(); cell_index++) {
            string cell = row[cell_index];

            if (cell.length() > column_widths[cell_index]) {
                column_widths[cell_index] = cell.length();
            } 
        }
    }

    for (int row_index = 0; row_index < table.size(); row_index++) {
        vector<string> row = table[row_index];

        for (int cell_index = 0; cell_index < row.size(); cell_index++) {
            string cell = row[cell_index];

            renderer << cell;
            if (column_widths[cell_index] - cell.length() > 0) renderer << string(column_widths[cell_index] - cell.length(), ' ');
            renderer << ' ';
        }
        renderer << '\n';
    }
    
    return renderer.str();
}