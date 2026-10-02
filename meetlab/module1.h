#ifndef UNTITLED2_MODULE1_H
#define UNTITLED2_MODULE1_H
#include <stdio.h>

#define MAX_WORDS 10000
#define MAX_WORD_LEN 100

typedef struct {
    size_t count;
    char text[MAX_WORD_LEN];
    double freq;
} WordStat;

int scanFile(FILE *input, WordStat words[], size_t *size);

#endif