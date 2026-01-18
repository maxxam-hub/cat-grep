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
  int error;
};

void printSymbol(int c, struct FlagsOption *flagsO,
                 struct FlagsCondition *flagsC);
void printFirstSymbol(int c, struct FlagsOption *flagsO,
                      struct FlagsCondition *flagsC, int *numberOfString);
void symbolProcessing(int argc, char *argv[], struct FlagsOption *flagsO,
                      char *paths[argc], int *filesCount, struct FlagsCondition *flagsC);
struct FlagsCondition initConditionFlags();
struct FlagsOption initOptionFlags();

int main(int argc, char *argv[]) {
  struct FlagsOption flags = initOptionFlags();
  struct FlagsCondition flagsCondition = initConditionFlags();
  char *paths[argc];
  int filesCount = 0;
  symbolProcessing(argc, argv, &flags, paths, &filesCount, &flagsCondition);

  if (flagsCondition.error) return 0;

  int numberOfString = 1;
  int c;
  FILE *stream;
  if (filesCount) {
    for (int i = 0; i < filesCount; i++) {
      stream = fopen(paths[i], "rb");
      while ((c = fgetc(stream)) != -1) {
        if (flagsCondition.isStartOfString) {
          printFirstSymbol(c, &flags, &flagsCondition, &numberOfString);
        } else {
          flagsCondition.isPreviousStringEmpty = 0;
          printSymbol(c, &flags, &flagsCondition);
        }
      }
    }
  } else {
    FILE *stream = stdin;
    flagsCondition.isStdin = 1;
    while ((c = getc(stream)) != -1) {
      if (flagsCondition.isStartOfString) {
        printFirstSymbol(c, &flags, &flagsCondition, &numberOfString);
      } else {
        flagsCondition.isPreviousStringEmpty = 0;
        printSymbol(c, &flags, &flagsCondition);
      }
    }
  }
}

void printSymbol(int c, struct FlagsOption *flagsO,
                 struct FlagsCondition *flagsC) {
  if (c == '\t' && flagsO->overwriteTab) {
    printf("^I");
  } else if (c == '\r') {
    if (flagsO->printUnprintable && flagsO->overwriteEnd)
      flagsC->isCombination = 1;
  } else if (c == '\n') {
    if (flagsC->isCombination) {
      printf("^M$");
      flagsC->isCombination = 0;
    } else if (flagsO->overwriteEnd) {
      printf("$");
    }
    flagsC->isStartOfString = 1;
    printf("\n");
  } else if (flagsC->isCombination || (c == '\0' && flagsO->printUnprintable)) {
    flagsC->isStartOfString = 1;
    flagsC->isCombination = 0;
    printf("$");
  } else if ((c >= 32 && c <= 126) || (c == '\t' || c == '\n' || c == '\r')) {
    printf("%c", c);
  } else if (flagsO->printUnprintable) {
    printf("^X");
  }
}

void printFirstSymbol(int c, struct FlagsOption *flagsO,
                      struct FlagsCondition *flagsC, int *numberOfString) {
  if (c == '\n') {
    if (flagsO->squeezeBlank) {
      if ((flagsO->numberNonblank || flagsO->number) && !flagsC->isPreviousStringEmpty) {
        printf("%d", *numberOfString);
        *numberOfString += 1;
      }
      if (!flagsC->isPreviousStringEmpty) {
        flagsC->isPreviousStringEmpty = 1;
        printf("%c", c);
      }
    } else {
      if (flagsO->numberNonblank) {
        printf("%d%c", *numberOfString, c);
        *numberOfString += 1;
      }
      if (flagsO->number) {
        printf("%d%c", *numberOfString, c);
        *numberOfString += 1;
      }
    }
  } else {
    if (flagsO->number) {
      printf("%d", *numberOfString);
      *numberOfString += 1;
    }
    if (flagsO->printUnprintable && (c < 32 || c > 126) && c != '\r' &&
        c != '\t' && c != '\n') {
      printf("^X");
      flagsC->isStartOfString = 0;
    } else {
      printSymbol(c, flagsO, flagsC);
      flagsC->isStartOfString = 0;
    }
  }
}

void symbolProcessing(int argc, char *argv[], struct FlagsOption *flagsO,
                      char *paths[argc], int *filesCount, struct FlagsCondition *flagsC) {
  int greatT = 0;
  int greatE = 0;
  int isNonBlank = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (strcmp(argv[i], "--number-nonblank") == 0) {
        flagsO->numberNonblank = 1;
        flagsO->number = 0;
      } else if (strcmp(argv[i], "--number") == 0) {
        flagsO->number = 1;
      } else if (strcmp(argv[i], "--squeeze-blank") == 0) {
        flagsO->squeezeBlank = 1;
      } else {
        printf("Error: Unexpected GNU option");
        flagsC->error = 1;
      }
    } else if (argv[i][0] == '-') {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        if (argv[i][j] == 'b') {
          flagsO->numberNonblank = 1;
          flagsO->number = 0;
          isNonBlank = 1;
        } else if (argv[i][j] == 'e') {
          flagsO->printUnprintable = 1;
          flagsO->overwriteEnd = 1;
        } else if (argv[i][j] == 'E') {
          flagsO->overwriteEnd = 1;
          flagsO->printUnprintable = 0;
          greatE = 1;
        } else if (argv[i][j] == 'n') {
          flagsO->number = 1;
        } else if (argv[i][j] == 's') {
          flagsO->squeezeBlank = 1;
        } else if (argv[i][j] == 't') {
          flagsO->overwriteTab = 1;
          flagsO->printUnprintable = 1;
        } else if (argv[i][j] == 'T') {
          flagsO->overwriteTab = 1;
          flagsO->printUnprintable = 0;
          greatT = 1;
        } else {
          printf("Error: Unexpected option");
          flagsC->error = 1;
        }
      }
      flagsO->printUnprintable = (greatE || greatT) ? 0 : flagsO->printUnprintable;
      flagsO->number = isNonBlank ? 0 : flagsO->number;
    } else {
      paths[*filesCount] = argv[i];
      *filesCount += 1;
    }
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