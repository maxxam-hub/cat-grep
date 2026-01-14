#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FlagsOption {
  int numberNonblank;
  int printUnprintable;
  int overwriteEnd;
  int number;
  int squeezeBlank;
  int overwriteTab;
};

struct FlagsCondition {
  int isCombination;
  int isStartOfString;
  int isPreviousStringEmpty;
  int isStdin;
};

void printSymbol(char c, struct FlagsOption flagsO,
                 struct FlagsCondition flagsC);
struct FlagsCondition initConditionFlags();
struct FlagsOption initOptionFlags();

int main(int argc, char *argv[]) {
  struct FlagsOption flags = initOptionFlags();
  struct FlagsCondition flagsCondition = initConditionFlags();
  char *paths[argc];
  int filesCount = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      printf("\n1");
      if (strcmp(argv[i], "--number-nonblank") == 0) {
        flags.numberNonblank = 1;
        flags.number = 0;
      } else if (strcmp(argv[i], "--number")) {
        flags.number = 0;
      } else if (strcmp(argv[i], "--squeeze-blank")) {
        flags.squeezeBlank = 1;
      } else {
        printf("Unexpected GNU option");
      }
    } else if (argv[i][0] == '-') {
      printf("\n2");
      for (int j = 1; j < strlen(argv[i]); j++) {
        if (argv[i][j] == 'b') {
          flags.numberNonblank = 1;
          flags.number = 0;
        } else if (argv[i][j] == 'e') {
          flags.printUnprintable = 1;
          flags.overwriteEnd = 1;
        } else if (argv[i][j] == 'E') {
          flags.overwriteEnd = 1;
          flags.printUnprintable = 0;
        } else if (argv[i][j] == 'n') {
          flags.number = 1;
        } else if (argv[i][j] == 's') {
          flags.squeezeBlank = 1;
        } else if (argv[i][j] == 't') {
          flags.overwriteTab = 1;
          flags.printUnprintable = 1;
        } else if (argv[i][j] == 'T') {
          flags.overwriteTab = 1;
          flags.printUnprintable = 0;
        } else {
          printf("Unexpected option");
        }
      }
    } else {
      printf("\n3");
      paths[filesCount] = argv[i];
      filesCount++;
    }
  }

  int numberOfString = 1;
  char c;
  FILE *stream;
  if (filesCount) {
    printf("\n4");
    for (int i = 0; i < filesCount; i++) {
      stream = fopen(paths[i], "r");
      while ((c = fgetc(stream)) != -1) {
        if (flagsCondition.isStartOfString) {
          if (c == '\n') {
            printf("\n7");
            if (flags.squeezeBlank)
              flagsCondition.isPreviousStringEmpty =
                  !flagsCondition.isPreviousStringEmpty ? 1 : 0;
          } else {
            if (flags.numberNonblank) {
              printf("%d", numberOfString);
              numberOfString++;
            } else if (flags.number) {
              printf("\n8");
              printf("%d", numberOfString);
              numberOfString++;
            }
            if (flags.printUnprintable &&
                ((c < 32 || c > 126) &&
                 (c != '\r' && c != '\t' && c != '\n'))) {
              printf("^X");
              flagsCondition.isStartOfString = 0;
              printf("\n9\n");
            } else {
              printf("\n10");
              printSymbol(c, flags, flagsCondition);
              flagsCondition.isStartOfString = 0;
            }
          }
        } else {
          // printf("\n11\n");
          printSymbol(c, flags, flagsCondition);
        }
      }
    }
  } else {
    printf("\n5\n");
    FILE *stream = stdin;
    flagsCondition.isStdin = 1;
    while (c = getc(stream) != -1) {
      if (flagsCondition.isStartOfString) {
        if (c == '\n') {
          printf("\n7");
          if (flags.squeezeBlank)
            flagsCondition.isPreviousStringEmpty =
                !flagsCondition.isPreviousStringEmpty ? 1 : 0;
        } else {
          if (flags.numberNonblank) {
            printf("%d", numberOfString);
            numberOfString++;
          } else if (flags.number) {
            printf("\n8");
            printf("%d", numberOfString);
            numberOfString++;
          }
          if (flags.printUnprintable &&
              ((c < 32 || c > 126) && (c != '\r' && c != '\t' && c != '\n'))) {
            printf("^X");
            flagsCondition.isStartOfString = 0;
            printf("\n9\n");
          } else {
            printf("\n10");
            printSymbol(c, flags, flagsCondition);
            flagsCondition.isStartOfString = 0;
          }
        }
      } else {
        // printf("\n11\n");
        printSymbol(c, flags, flagsCondition);
      }
    }
  }
  return 0;
}

void printSymbol(char c, struct FlagsOption flagsO,
                 struct FlagsCondition flagsC) {
  if (c == '\t' && flagsO.overwriteTab) {
    printf("\n6");
    printf("^I");
  } else if (c == '\r') {
    if (flagsO.printUnprintable && flagsO.overwriteEnd)
      flagsC.isCombination = 1;
  } else if (c == '\n') {
    if (flagsC.isCombination) {
      printf("^M$");
      flagsC.isCombination = 0;
    } else if (flagsO.printUnprintable && flagsO.overwriteEnd)
      printf("$");
    printf("\n");
    flagsC.isStartOfString = 1;
  } else if (flagsC.isCombination || (c == '\0' && flagsO.printUnprintable)) {
    printf("$");
  } else if (c >= 32 && c <= 126) {
    printf("%c", c);
  } else if (flagsO.printUnprintable) {
    printf("^X");
  }
}

struct FlagsCondition initConditionFlags() {
  struct FlagsCondition flags;
  flags.isPreviousStringEmpty = 0;
  flags.isStartOfString = 1;
  flags.isStdin = 0;
  flags.isCombination = 0;
  return flags;
}

struct FlagsOption initOptionFlags() {
  struct FlagsOption flags;
  flags.numberNonblank = 0;
  flags.printUnprintable = 0;
  flags.overwriteEnd = 0;
  flags.number = 0;
  flags.squeezeBlank = 0;
  flags.overwriteTab = 0;
  return flags;
}