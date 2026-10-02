#include <stdio.h>
#include "module1.h"
#include "module2.h"

int main(int argc, char *argv[])
{
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    if (argc != 3) {
        return 1;
    }

    WordStat words[MAX_WORDS];
    size_t size = 0;

    if (scanFile(input, words, &size) != 0) {
        fclose(input);
        fclose(output);
        return 1;
    }

    if (writeCSV(output, words, size) != 0) {
        fclose(input);
        fclose(output);
        return 1;
    }

    fclose(input);
    fclose(output);

    return 0;
}