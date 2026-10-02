#include <stdio.h>
#include "module1.h"

#include <ctype.h>
#include <string.h>

static int isSeparator(int ch) {
    char separators[] = " \n\t\r.,!?;:\"()[]{}<>-/\\";
    return ch == '\0' || strchr(separators, ch) != NULL;
}

static int addWord(WordStat words[], size_t *size, char *text) {
    for (size_t i = 0; i < *size; i++) {
        if (strcmp(words[i].text, text) == 0) {
            words[i].count++;
            return 0;
        }
    }

    strcpy(words[*size].text, text);
    words[*size].count = 1;
    words[*size].freq = 0.0;

    (*size)++;
    return 0;
}

static void calcFreq(WordStat words[], size_t size, size_t totalWords) {
    for (size_t i = 0; i < size; i++) {
        words[i].freq = (double)words[i].count / totalWords * 100.0;
    }
}

int scanFile(FILE *input, WordStat words[], size_t *size) {
    *size = 0;

    char word[MAX_WORD_LEN];
    size_t wordLen = 0;
    size_t totalWords = 0;
    int ch;

    while ((ch = fgetc(input)) != EOF) {
        if (!isSeparator(ch)) {
            if (wordLen < MAX_WORD_LEN - 1) {
                word[wordLen++] = (char)ch;
            }
        } else if (wordLen > 0) {
            word[wordLen] = '\0';
            if (addWord(words, size, word) != 0) {
                return -1;
            }
            totalWords++;
            wordLen = 0;
        }
    }

    if (wordLen > 0) {
        word[wordLen] = '\0';

        if (addWord(words, size, word) != 0) {
            return -1;
        }

        totalWords++;
    }

    calcFreq(words, *size, totalWords);
    return 0;
}