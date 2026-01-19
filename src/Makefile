CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
CFLAGS_TEST = -DTEST_BASIC -DTEST_N -DTEST_B -DTEST_S -DTEST_E -DTEST_T -DTEST_T -DTEST_CRLF -DTEST_PR_UNP -DTEST_ERROR

SRC_FUNCS_CAT = cat/s21_cat.c
SRC_FUNCS_GREP = grep/s21_grep.c
TARGET_CAT = s21_cat
TARGET_GREP = s21_grep
OBJECT_CAT = $(SRC_FUNCS_CAT:.c=.o)
OBJECT_GREP = $(SRC_FUNCS_GREP:.c=.o)

TEST_TARGET = test_cat
TEST_SRC = cat/s21_cat_tests.c

$(TARGET_CAT): $(OBJECT_CAT)
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET_GREP): $(OBJECT_GREP)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJECT_CAT) $(OBJECT_GREP) $(TARGET_CAT) $(TARGET_GREP) $(TEST_TARGET)

.PHONY: all test clean