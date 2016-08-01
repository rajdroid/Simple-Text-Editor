#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ncurses.h>

typedef struct
{
    unsigned int len;
    char *line;
} LINE;

LINE* read_line()
{
    unsigned int buf_size = 5;
    int y, x;                       // cordinates

    char *line = (char *)malloc(sizeof(char) * buf_size);
    if (!line) return NULL;

    unsigned int line_index = 0;

    while (1)
    {
        int ch;
        ch = getch();

        if (ch == EOF || ch == '\n')
        {
            line[line_index] = '\0';
            printw("\n");
            break;
        }
        else if (ch == KEY_LEFT)
        {
            getyx(stdscr, y, x);
            move(y, x-1);
        }
        else if (ch == KEY_RIGHT)
        {
            getyx(stdscr, y, x);
            move(y, x+1);
        }
        else
        {
            printw("%c", ch);
            line[line_index] = ch;
        }
        line_index++;

        if (line_index >= buf_size)
        {
            buf_size *= 1.5;
            line = (char *)realloc(line, buf_size);
            if (!line) return NULL;
        }
        refresh();
    }

    LINE *line_instance = (LINE *)malloc(sizeof(LINE));
    if (!line_instance) return NULL;

    line_instance->len = line_index;
    line_instance->line = line;

    return line_instance;
}

int main(int argc, char const *argv[])
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    unsigned int no_lines = 5;
    LINE **lines = (LINE **)malloc(sizeof(LINE*) * no_lines);
    unsigned int lines_index = 0;

    while (1)
    {
        printw("%2d ", lines_index);
        LINE *line_instance = read_line();
        if (!line_instance) break;

        if (strcmp(line_instance->line, "exit") == 0)
        {
            break;
        }

        lines[lines_index] = line_instance;
        lines_index++;


        if (lines_index >= no_lines)
        {
            no_lines *= 1.5;
            lines = (LINE **)realloc(lines, no_lines);
        }
    }

    // for debugging purposes
    for (unsigned int i = 0; i < lines_index; ++i)
    {
        // printf("%s\n", lines[i]->line);
        printw("%s\n", lines[i]->line);
        refresh();
    }
    getch();
    endwin();

    return 0;
}