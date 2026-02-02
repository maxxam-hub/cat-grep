#include <stdio.h>
#include <stdlib.h>

#ifdef TEST_BASIC
void test_basic(void);
#endif

#ifdef TEST_N
void test_n(void);
#endif

#ifdef TEST_B
void test_b(void);
#endif

#ifdef TEST_S
void test_s(void);
#endif

#ifdef TEST_E
void test_e(void);
#endif

#ifdef TEST_T
void test_tab(void);
#endif

#ifdef TEST_PR_UNP
void test_print_unprint(void);
#endif

#ifdef TEST_CRLF
void test_crlf(void);
#endif

#ifdef TEST_ERROR
void test_error(void);
#endif

int main(void) {
#ifdef TEST_BASIC
  test_basic();
#endif

#ifdef TEST_N
  test_n();
#endif

#ifdef TEST_B
  test_b();
#endif

#ifdef TEST_S
  test_s();
#endif

#ifdef TEST_E
  test_e();
#endif

#ifdef TEST_T
  test_tab();
#endif

#ifdef TEST_PR_UNP
  test_tab();
#endif

#ifdef TEST_PR_UNP
  test_print_unprint();
#endif

#ifdef TEST_CRLF
  test_crlf();
#endif

#ifdef TEST_ERROR
  test_error();
#endif

  return 0;
}

void test_basic(void) {
  printf("TEST: BASIC\n");
  system("./s21_cat cat/tests/test_blank.txt");
  printf("\n");
}

void test_n(void) {
  printf("TEST: -n\n");
  system("./s21_cat -n cat/tests/test_blank.txt");
  printf("\n");
}

void test_b(void) {
  printf("TEST: -b\n");
  system("./s21_cat -b cat/tests/test_mix.txt");
  printf("\n");
}

void test_s(void) {
  printf("TEST: -s\n");
  system("./s21_cat -s cat/tests/test_mix.txt");
  printf("\n");
}

void test_e(void) {
  printf("TEST: -e\n");
  system("./s21_cat -e cat/tests/test_mix.txt");
  printf("\n");
}

void test_tab(void) {
  printf("TEST: -t\n");
  system("./s21_cat -t cat/tests/test_tabs.txt");
  printf("\n");
}

void test_print_unprint(void) {
  printf("TEST: -e -v\n");
  system("./s21_cat -e cat/tests/test_ctrl.txt");
  printf("\n");
}

void test_crlf(void) {
  printf("TEST: -e in bin\n");
  system("./s21_cat -e cat/tests/test_bin.bin");
  printf("\n");
}

void test_error(void) {
  printf("TEST: ERROR (no file)\n");
  system("./s21_cat cat/tests/no_such_file.txt");
  printf("\n");
}