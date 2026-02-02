#include "s21_cat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
  FlagsCat flags = {0};
  FlagsCondition flags_condition = {0};
  flags_condition.is_start_of_string = 1;
  char* paths[argc];
  int files_count = 0;
  symbol_processing(argc, argv, &flags, paths, &files_count, &flags_condition);

  int number_of_string = 1;
  int c;
  FILE* stream;
  if (files_count) {
    for (int i = 0; i < files_count; i++) {
      if ((stream = fopen(paths[i], "rb")) == NULL || flags_condition.error) {
        if (stream == NULL) printf("\n%s: No such file or directory", paths[i]);
        fclose(stream);
        return 0;
      } else {
        while ((c = fgetc(stream)) != -1) {
          if (flags_condition.is_start_of_string) {
            print_first_symbol(c, &flags, &flags_condition, &number_of_string);
          } else {
            print_symbol(c, &flags, &flags_condition);
          }
        }
        fclose(stream);
      }
    }
  } else {
    stream = stdin;
    flags_condition.is_stdin = 1;
    while ((c = getc(stream)) != -1) {
      if (flags_condition.is_start_of_string) {
        print_first_symbol(c, &flags, &flags_condition, &number_of_string);
      } else {
        print_symbol(c, &flags, &flags_condition);
      }
    }
  }
}

void print_symbol(int c, FlagsCat* flags_cat, FlagsCondition* flags_condition) {
  if (c == '\t' && flags_cat->t) {
    printf("^I");

  } else if (c == '\r') {
    if (flags_cat->v && flags_cat->e) flags_condition->is_combination = 1;

  } else if (c == '\n') {
    flags_condition->is_previous_string_empty = 0;

    if (flags_condition->is_combination) {
      printf("^M$");
      flags_condition->is_combination = 0;
    } else if (flags_cat->e) {
      printf("$");
    }

    flags_condition->is_start_of_string = 1;
    printf("\n");

  } else if (flags_condition->is_combination || (c == '\0' && flags_cat->v)) {
    flags_condition->is_start_of_string = 1;
    flags_condition->is_combination = 0;
    printf("$");

  } else if ((c >= 32 && c <= 126) || c == '\t' || c == '\n' || c == '\r') {
    printf("%c", c);

  } else if (flags_cat->v) {
    printf("^X");
  }
}

void print_first_symbol(int c, FlagsCat* flags_cat,
                        FlagsCondition* flags_condition,
                        int* number_of_string) {
  if (c == '\n') {
    if (flags_cat->s) {
      if (!flags_condition->is_previous_string_empty) {
        if (flags_cat->n) {
          printf("     %d  ", *number_of_string);
          (*number_of_string)++;
        }
        if (flags_cat->e) printf("$");
        printf("\n");
        flags_condition->is_previous_string_empty = 1;
      }
    } else {
      if (flags_cat->n) {
        printf("     %d  ", *number_of_string);
        (*number_of_string)++;
      }
      if (flags_cat->e) printf("$");
      printf("\n");
      flags_condition->is_previous_string_empty = 1;
    }

    flags_condition->is_start_of_string = 1;
    return;
  }

  flags_condition->is_previous_string_empty = 0;

  if (flags_cat->b || flags_cat->n) {
    printf("     %d  ", *number_of_string);
    (*number_of_string)++;
  }

  if (flags_cat->v && (c < 32 || c > 126) && c != '\r' && c != '\t' &&
      c != '\n') {
    printf("^X");
  } else {
    print_symbol(c, flags_cat, flags_condition);
  }

  flags_condition->is_start_of_string = 0;
}

void symbol_processing(int argc, char* argv[], FlagsCat* flags_cat,
                       char* paths[argc], int* files_count,
                       FlagsCondition* flags_condition) {
  int greatT = 0;
  int greatE = 0;
  int is_non_blank = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp(argv[i], "--number-nonblank") == 0) {
        flags_cat->b = 1;
        flags_cat->n = 0;
      } else if (strcmp(argv[i], "--number") == 0) {
        flags_cat->n = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        flags_cat->s = 1;
      } else {
        printf("Error: Unexpected GNU option");
        flags_condition->error = 1;
      }
    } else if (argv[i][0] == '-') {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        if (argv[i][j] == 'b') {
          flags_cat->b = 1;
          flags_cat->n = 0;
          is_non_blank = 1;
        } else if (argv[i][j] == 'e') {
          flags_cat->v = 1;
          flags_cat->e = 1;
        } else if (argv[i][j] == 'E') {
          flags_cat->e = 1;
          flags_cat->v = 0;
          greatE = 1;
        } else if (argv[i][j] == 'n') {
          flags_cat->n = 1;
        } else if (argv[i][j] == 's') {
          flags_cat->s = 1;
        } else if (argv[i][j] == 't') {
          flags_cat->t = 1;
          flags_cat->v = 1;
        } else if (argv[i][j] == 'T') {
          flags_cat->t = 1;
          flags_cat->v = 0;
          greatT = 1;
        } else {
          printf("\nError: Unexpected option");
          flags_condition->error = 1;
        }
      }
      flags_cat->v = (greatE || greatT) ? 0 : flags_cat->v;
      flags_cat->n = is_non_blank ? 0 : flags_cat->n;
    } else {
      paths[*files_count] = argv[i];
      *files_count += 1;
    }
  }
}