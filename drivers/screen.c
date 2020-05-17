#include "screen.h"
#include "ports.h"
#include "../utils/utils.h"

int CURRENT_CURSOR_POSITION = 0;

void set_cursor_position(int offset);
int get_cursor_position();

int get_current_cursor_row();
int get_current_cursor_column();
int get_offset(int col, int row);

void clear() {
    char* video_memory = (char*) VIDEO_ADDRESS;
    int row = 0;
    for(row = 0; row < TOTAL_COLS * TOTAL_ROWS; row++)
    {
        video_memory[row * 2] = ' ';
        video_memory[row * 2 + 1] = STANDARD_MSG_COLOR;
    }
    CURRENT_CURSOR_POSITION = 0x00;
    set_cursor_position(CURRENT_CURSOR_POSITION);
}

void printf(char *str, ...) {
    char* video_memory = (char*) VIDEO_ADDRESS;
    int pos = 0;
    int cursorPosition = get_cursor_position();
    while(str[pos] != 0)
    {
        int row = get_current_cursor_row(cursorPosition);
        int column = get_current_cursor_column(cursorPosition);

        if(str[pos] == '\n')
        {
            cursorPosition = get_offset(column, row + 1);
            pos++;
        }
        else if(str[pos] == '\r')
        {
            cursorPosition = get_offset(0, row);
            pos++;
        }
        else
        {
            video_memory[cursorPosition] = str[pos++];
            video_memory[cursorPosition + 1] = STANDARD_MSG_COLOR;
            cursorPosition = cursorPosition + 2;
        }


        //check are we on last row?
        if(cursorPosition >= TOTAL_ROWS * TOTAL_COLS * 2)
        {
            int i;
            /*shift the content by 1 line up*/
            for(i = 1; i < TOTAL_ROWS; i++)
            {
                memcopy(get_offset(0, i) + VIDEO_ADDRESS, get_offset(0, i - 1) + VIDEO_ADDRESS, TOTAL_COLS * 2);
            }

            /*create one blank line at the end*/
            char *last_line = get_offset(0, TOTAL_ROWS - 1) + VIDEO_ADDRESS;
            for(i = 0; i < TOTAL_COLS * 2; i++)
            {
                last_line[i] = 0;
            }

            cursorPosition = get_offset(0, TOTAL_ROWS - 1); /*TODO: revisit this.. last line cursor not visible*/
        }
    }
    set_cursor_position(cursorPosition);
}

int get_cursor_position() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int position = port_byte_in(REG_SCREEN_DATA) << 8;

    port_byte_out(REG_SCREEN_CTRL, 15);
    position += port_byte_in(REG_SCREEN_DATA);

    return position * 2;
}

void set_cursor_position(int offset)
{
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, offset >> 8);

    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, offset & 0xff);
}

int get_current_cursor_row(int cursorPosition) {
    return cursorPosition / (2 * TOTAL_COLS);
}

int get_current_cursor_column(int cursorPosition) {
    return (cursorPosition - (get_current_cursor_row(cursorPosition) * 2 * TOTAL_COLS)) / 2;
}

int get_offset(int col, int row) { return 2 * (row * TOTAL_COLS + col); }