#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <ostream>
#include <vector>
#include "word_stat.h"

// Записывает статистику слов в выходной поток в формате CSV
class CSVWriter {
private:
    char delimiter;

public:
    CSVWriter(char delim);

    // Формирует CSV-таблицу с заголовком
    void write(std::ostream &out, const std::vector<WordStat> &words) const;
};

#endif
