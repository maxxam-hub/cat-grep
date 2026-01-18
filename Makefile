CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC_FUNCS = s21_cat.c
TARGET = s21_cat

OBJECT = $(SRC_FUNCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJECT) $(TARGET)

.PHONY: make run clean