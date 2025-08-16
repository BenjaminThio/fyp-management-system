#ifndef INPUT_H
#define INPUT_H
#include <array>
using namespace std;

void set_input_cursor_position(int, int);
array<int, 2> better_cin(string& input, int& input_cursor_pos, bool allow_spaces = true /* , int x_offset = 0, int y_offset = 0 */ );
void test();

#endif