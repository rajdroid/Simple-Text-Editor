#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>


// to store cursor position
typedef struct {
    unsigned int x;
    unsigned int y;
} POSITION;

// to store line with some metadata
typedef struct {
    POSITION pos;
    unsigned int line_len;
    unsigned int line_capacity;
    char *line;
} LINE;

void store_char(LINE **lines, char ch)
{
    POSITION curr_pos;
    getyx(stdscr, curr_pos.y, curr_pos.x);

/*    each lines has corresponding buffer to store characters
    so we check if buffer already exists, if not than we allocate
    emory for it
*/
    if (!lines[curr_pos.y])
    {
        lines[curr_pos.y] = (LINE *)malloc(sizeof(LINE));
        memset(lines[curr_pos.y], 0, sizeof(LINE));
        lines[curr_pos.y]->line_capacity = 5;
        lines[curr_pos.y]->line = (char *)malloc(sizeof(char) * lines[curr_pos.y]->line_capacity);
        memset(lines[curr_pos.y]->line, 0, lines[curr_pos.y]->line_capacity);
    }

    // if need to store more characters than previous allocation than realloc
    if (lines[curr_pos.y]->line_capacity <= curr_pos.x)
    {
        lines[curr_pos.y]->line_capacity *= 2;
        lines[curr_pos.y]->line = (char *)realloc(lines[curr_pos.y]->line, lines[curr_pos.y]->line_capacity);
    }

    // store character in buffer
    lines[curr_pos.y]->line[curr_pos.x] = ch;

    // increase the line_len variable
    if (lines[curr_pos.y]->line_len < curr_pos.x + 1)
    {
        lines[curr_pos.y]->line_len = curr_pos.x + 1;
    }

    return;
}

void repaint(LINE **lines)
{
    POSITION pos;
    getyx(stdscr, pos.y, pos.x);

    // iterate on each line and print character upto current cursor
    for (unsigned int i = 0; i <= pos.y; ++i)
    {
        move(i, 0);
        for (unsigned int j = 0; j <= pos.x; ++j)
        {
            printw("%c", lines[i]->line[j]);
        }
    }
    refresh();
}

int main(int argc, char const *argv[])
{
    // ncurses initialization
    initscr();
    raw();

    // take every key strokes ex F1.. F2
    keypad(stdscr, TRUE);

    // does not print key strokes to screen
    noecho();

    // allocate memory to store rows (or lines)
    unsigned int lines_capacity = 5;
    LINE **lines = (LINE **)malloc(sizeof(LINE *) * lines_capacity);
    memset(lines, 0, sizeof(LINE *) * lines_capacity);

    while (1)
    {
        int ch = getch();

        POSITION pos;

        // get current cursor position
        getyx(stdscr, pos.y, pos.x);

        // if left arrow key is pressed
        if (ch == KEY_LEFT)
        {
            // move left if we already not at left most position
            if (pos.x > 0) move(pos.y, pos.x-1);
            continue;
        }
        // if right arrow key is pressed
        else if (ch == KEY_RIGHT)
        {
            // move right if we already not at right most position
            if (lines[pos.y]->line_len > pos.x+1) move(pos.y, pos.x+1);
            continue;
        }
        // if up arrow key is pressed move cursor one line up
        else if (ch == KEY_UP)
        {
            // move up only if we are not at top most line
            if (pos.y > 0) move(pos.y-1, pos.x);
            continue;
        }
        // if down arrow key is pressed move cursor one line down
        else if (ch == KEY_DOWN)
        {
            // move down only if we are not at last line
            if (lines[pos.y+1] && lines[pos.y+1]->line) move(pos.y+1, pos.x);
            continue;
        }
        // if end key is pressed exit from editor
        else if (ch == KEY_END)
        {
            break;
        }
        // if any valid character
        else
        {
            store_char(lines, ch);
            repaint(lines);
        }

        // if need more lines than previous allocation than realloc
        if (pos.y+1 >= lines_capacity)
        {
            LINE **lines_temp = (LINE **)malloc(sizeof(LINE *) * lines_capacity * 2);
            memset(lines_temp, 0, sizeof(LINE *) * lines_capacity * 2);
            LINE **lines_bkp = lines;
            lines = memcpy(lines_temp, lines, sizeof(LINE *) * lines_capacity);
            lines_capacity *= 2;

            free(lines_bkp);
            lines_bkp = NULL;
        }
    }


    for (unsigned int i = 0; lines[i] != NULL; ++i)
    {
        if (lines[i]->line != NULL)
        {
            free(lines[i]->line);
        }
        free(lines[i]);
    }
    free(lines);

    endwin();
    return 0;
}