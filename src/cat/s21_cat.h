#ifndef S21_CAT_H
#define S21_CAT_H

typedef struct {
  int b;
  int v;
  int e;
  int n;
  int s;
  int t;
} FlagsCat;

typedef struct {
  int is_combination;
  int is_start_of_string;
  int is_previous_string_empty;
  int is_stdin;
  int error;
} FlagsCondition;

void print_symbol(int c, FlagsCat* flags_cat, FlagsCondition* flags_condition);
void print_first_symbol(int c, FlagsCat* flags_cat,
                        FlagsCondition* flags_condition, int* numberOfString);
void symbol_processing(int argc, char* argv[], FlagsCat* flags_cat,
                       char* paths[argc], int* files_count,
                       FlagsCondition* flags_condition);

#endif