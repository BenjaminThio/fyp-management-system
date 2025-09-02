#ifndef INPUT_H
#define INPUT_H
#include <array>
#include "text_formatter.h"
using namespace std;
using namespace ansi;

void set_input_cursor_position(int, int);
string render_static_input_field(const string& input, const size_t length, const TextFormat& bg_color = BG_WHITE, const TextFormat& fg_color = FG_BLACK);
string render_input_field(const string& input, const int local_caret_pos, const int input_field_view_offset, const size_t length, const TextFormat& bg_color = BG_WHITE, const TextFormat& fg_color = FG_BLACK);
array<int, 2> input_field(string& input, int& local_caret_pos, int& input_field_view_offset, bool allow_spaces = true, const size_t max_length = 0, const size_t length = 0);
array<int, 2> better_cin(string& input, int& input_cursor_pos, bool allow_spaces = true, size_t max_length = 0 /* , int x_offset = 0, int y_offset = 0 */ );
void test();

#endif