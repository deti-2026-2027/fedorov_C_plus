#include "module2.h"

static void sortWords(WordStat words[], size_t size)
{
    for (size_t i = 0; i < size - 1; i++) {
        for (size_t j = 0; j < size - i - 1; j++) {

            if (words[j].freq < words[j + 1].freq) {
                WordStat temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int writeCSV(FILE *output, WordStat words[], size_t size)
{
    fprintf(output, "\xEF\xBB\xBF");
    fprintf(output, "Слова:;Частота:\n");

    sortWords(words, size);
    for (size_t i = 0; i < size; i++) {
        fprintf(output, "%s;%.1f%%\n",
                words[i].text,
                words[i].freq);
    }

    return 0;
}