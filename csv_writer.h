#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <ostream>
#include <vector>
#include "word_stat.h"

class CSVWriter {
public:
    void write(std::ostream &out, std::vector<WordStat> words);

private:
    void sortWords(std::vector<WordStat> &words);
};

#endif
