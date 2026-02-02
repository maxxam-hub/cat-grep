#ifndef S21_GREP_H
#define S21_GREP_H

#define LINE_BUFFER 1024

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
} GrepFlags;

void parse_flags(int argc, char* argv[], GrepFlags* flags, char** pattern,
                 int* file_start);
int match(char* line, char* pattern, int ignore_case);
void grep_file(char* filename, char* pattern, GrepFlags flags,
               int print_filename);

#endif