CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
CFLAGS_TEST = -DTEST_BASIC -DTEST_N -DTEST_B -DTEST_S -DTEST_E -DTEST_T -DTEST_T -DTEST_CRLF -DTEST_PR_UNP -DTEST_ERROR

SRC_FUNCS = s21_cat.c
TARGET = s21_cat
OBJECT = $(SRC_FUNCS:.c=.o)

TEST_TARGET = test_cat
TEST_SRC = s21_cat_tests.c

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	$(CC) $(CFLAGS) $(CFLAGS_TEST) $(TEST_SRC) -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJECT) $(TARGET) $(TEST_TARGET)

.PHONY: all test clean