#include "s21_grep.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int exit_code = 0;

  if (argc < 3) {
    exit_code = 1;
  } else {
    GrepFlags flags = {0};
    char *pattern = NULL;
    int file_start = 0;

    parse_flags(argc, argv, &flags, &pattern, &file_start);

    if (pattern == NULL || file_start >= argc) {
      exit_code = 1;
    } else {
      int many_files = (argc - file_start > 1);

      for (int i = file_start; i < argc; i++) {
        grep_file(argv[i], pattern, flags, many_files);
      }
    }
  }

  return exit_code;
}

void parse_flags(int argc, char *argv[], GrepFlags *flags, char **pattern,
                 int *file_start) {
  int i = 1;

  while (i < argc && argv[i][0] == '-') {
    int j = 1;
    while (argv[i][j] != '\0') {
      if (argv[i][j] == 'e') flags->e = 1;
      else if (argv[i][j] == 'i') flags->i = 1;
      else if (argv[i][j] == 'v') flags->v = 1;
      else if (argv[i][j] == 'c') flags->c = 1;
      else if (argv[i][j] == 'l') flags->l = 1;
      else if (argv[i][j] == 'n') flags->n = 1;
      j++;
    }
    i++;
  }

  if (i < argc) {
    *pattern = argv[i];
    *file_start = i + 1;
  }
}

int match(const char *line, const char *pattern, int ignore_case) {
  int result = 0;

  if (!ignore_case) {
    if (strstr(line, pattern) != NULL) {
      result = 1;
    }
  } else {
    char line_copy[LINE_BUFFER];
    char pattern_copy[LINE_BUFFER];

    strncpy(line_copy, line, LINE_BUFFER - 1);
    strncpy(pattern_copy, pattern, LINE_BUFFER - 1);

    line_copy[LINE_BUFFER - 1] = '\0';
    pattern_copy[LINE_BUFFER - 1] = '\0';

    for (int i = 0; line_copy[i]; i++) {
      if (line_copy[i] >= 'A' && line_copy[i] <= 'Z') {
        line_copy[i] += 32;
      }
    }

    for (int i = 0; pattern_copy[i]; i++) {
      if (pattern_copy[i] >= 'A' && pattern_copy[i] <= 'Z') {
        pattern_copy[i] += 32;
      }
    }

    if (strstr(line_copy, pattern_copy) != NULL) {
      result = 1;
    }
  }

  return result;
}

void grep_file(const char *filename, const char *pattern, GrepFlags flags,
               int print_filename) {
  FILE *file = fopen(filename, "r");
  char line[LINE_BUFFER];
  int line_num = 0;
  int match_count = 0;
  int stop = 0;

  if (file == NULL) {
    perror(filename);
  } else {
    while (!stop && fgets(line, LINE_BUFFER, file) != NULL) {
      int is_match = 0;

      line_num++;
      is_match = match(line, pattern, flags.i);

      if (flags.v) {
        is_match = !is_match;
      }

      if (is_match) {
        match_count++;

        if (flags.l) {
          printf("%s\n", filename);
          stop = 1;
        } else if (!flags.c) {
          if (print_filename) {
            printf("%s:", filename);
          }
          if (flags.n) {
            printf("%d", line_num);
          }
          printf("%s", line);
        }
      }
    }

    if (flags.c && !flags.l) {
      if (print_filename) {
        printf("%s:", filename);
      }
      printf("%d\n", match_count);
    }

    fclose(file);
  }
}