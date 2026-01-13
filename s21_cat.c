#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// переписать на обычные флаги и везде поменять
struct Flags {
    int numberNonblank;
    int printUnprintable;
    int overwriteEnd;
    int number;
    int squeezeBlank;
    int overwriteTab;
};

int main(int argc, char *argv[]) {
    struct Flags flags;
    char *paths[argc];
    int filesCount = 0;
    for (int i = 1; i < argc - 1; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            if (strcmp(argv[i], "--number-nonblank")) {
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
                    flags.overwriteEnd = 1;
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
        }
        else {
            paths[filesCount] = argv[i];
            filesCount++;
        }
    }

    int numberOfString = 1;
    int isPreviousStringEmpty = 0;
    int isStartOfString = 1;
    int isStdin = 0;
    int isCombination = 0;
    char c;
    FILE *stream;
    if (paths) {
        for (int i = 0; i < filesCount - 1; i++) {
            stream = fopen(paths[i], "r");
            while (c = fgetc(stream)) {
                if (c == '\n' && isStartOfString && flags.squeezeBlank) {
                    if (!isPreviousStringEmpty) {
                        isPreviousStringEmpty = 1;
                    } else {
                        isPreviousStringEmpty = 0;
                    }
                } else if (isStartOfString) {
                    if (flags.numberNonblank && c != '\n') {
                        printf("%d", numberOfString);
                        numberOfString++;
                        isStartOfString = 0;
                    } else if (flags.number) {
                        printf("%d", numberOfString);
                        numberOfString++;
                        isStartOfString = 0;
                    }
                } else {
                    if (c == '\t' && flags.overwriteTab) {
                        printf("^I");
                    } else if (c == '\r') {
                        if (flags.printUnprintable && flags.overwriteEnd) isCombination = 1;
                    } else if (c == '\n') {
                        if (isCombination) {
                            printf("^M$");
                            isCombination = 0;
                        }
                        else if (flags.printUnprintable && flags.overwriteEnd) printf("$");
                        printf("\n");
                        isStartOfString = 1;
                    } else if (isCombination || c == '\0') {
                        printf("$");
                    } else if (c >= 32 && c <= 126) {
                        printf("%c", c);
                        isStartOfString = 0;
                    } else if (flags.printUnprintable){
                        printf("^X");
                    }
                }
            }
        }
    } else {
        FILE *stream = stdin;
        isStdin = 1;
        while (c = getc(stream)) {
                if (c == '\n' && isStartOfString && flags.squeezeBlank) {
                    if (!isPreviousStringEmpty) {
                        isPreviousStringEmpty = 1;
                    } else {
                        isPreviousStringEmpty = 0;
                    }
                } else if (isStartOfString) {
                    if (flags.numberNonblank && c != '\n') {
                        printf("%d", numberOfString);
                        numberOfString++;
                        isStartOfString = 0;
                    } else if (flags.number) {
                        printf("%d", numberOfString);
                        numberOfString++;
                        isStartOfString = 0;
                    }
                } else {
                    if (c == '\t' && flags.overwriteTab) {
                        printf("^I");
                    } else if (c == '\r') {
                        if (flags.printUnprintable && flags.overwriteEnd) isCombination = 1;
                    } else if (c == '\n') {
                        if (isCombination) {
                            printf("^M$");
                            isCombination = 0;
                        }
                        else if (flags.printUnprintable && flags.overwriteEnd) printf("$");
                        printf("\n");
                        isStartOfString = 1;
                    } else if (isCombination || c == '\0') {
                        printf("$");
                    } else if (c >= 32 && c <= 126) {
                        printf("%c", c);
                        isStartOfString = 0;
                    } else if (flags.printUnprintable){
                        printf("^X");
                    }
                }
            }
    }
    return 1;
}