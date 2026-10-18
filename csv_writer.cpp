#include "csv_writer.h"
#include <iomanip>

CSVWriter::CSVWriter(char delim) {
    delimiter = delim;
}

void CSVWriter::write(std::ostream &out, const std::vector<WordStat> &words) const {
    // Чтобы Excel корректно распознал кириллицу
    out << "\xEF\xBB\xBF";
    out << "Слова:" << delimiter << "Частота:\n";

    // Фиксируем формат вывода: 1 знак после запятой для процентов
    out << std::fixed << std::setprecision(1);
    for (const WordStat &item: words) {
        out << item.getWord() << delimiter << item.getFrequency() << "%\n";
    }
}
