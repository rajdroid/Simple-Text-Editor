#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    unsigned int len;
    char *line;
} LINE;

LINE* read_line(FILE *fp)
{
    unsigned int buf_size = 5;

    char *line = (char *)malloc(sizeof(char) * buf_size);
    if (!line) return NULL;

    unsigned int line_index = 0;

    while (1)
    {
        int ch;
        ch = fgetc(stdin);
        if (ch == EOF || ch == '\n')
        {
            line[line_index] = '\0';
            break;
        }
        else
        {
            line[line_index] = ch;
        }
        line_index++;

        if (line_index >= buf_size)
        {
            buf_size *= 1.5;
            line = (char *)realloc(line, buf_size);
            if (!line) return NULL;
        }
    }

    LINE *line_instance = (LINE *)malloc(sizeof(LINE));
    if (!line_instance) return NULL;

    line_instance->len = line_index;
    line_instance->line = line;

    return line_instance;
}

int main(int argc, char const *argv[])
{
    unsigned int no_lines = 5;
    LINE **lines = (LINE **)malloc(sizeof(LINE*) * no_lines);
    unsigned int lines_index = 0;

    while (1)
    {
        LINE *line_instance = read_line(stdin);
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

    for (unsigned int i = 0; i < lines_index; ++i)
    {
        printf("%s\n", lines[i]->line);
    }

    return 0;
}