#ifndef S21_CAT_H
#define S21_CAT_H

typedef struct {
  int numberNonblank;
  int printUnprintable;
  int overwriteEnd;
  int number;
  int squeezeBlank;
  int overwriteTab;
} FlagsOption;

typedef struct {
  int isCombination;
  int isStartOfString;
  int isPreviousStringEmpty;
  int isStdin;
  int error;
} FlagsCondition;

void printSymbol(int c, FlagsOption *flagsO, FlagsCondition *flagsC);
void printFirstSymbol(int c, FlagsOption *flagsO, FlagsCondition *flagsC, int *numberOfString);
void symbolProcessing(int argc, char *argv[], FlagsOption *flagsO, char *paths[argc], int *filesCount, FlagsCondition *flagsC);

#endif