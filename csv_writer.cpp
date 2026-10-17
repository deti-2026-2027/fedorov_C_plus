#include "csv_writer.h"
#include <iomanip>

void CSVWriter::sortWords(std::vector<WordStat> &words) {
    if (words.size() < 2) {
        //кол-во слов <2 - сортировка не нужна
        return;
    }

    for (std::size_t i = 0; i < words.size() - 1; i++) {
        //цикл сортировки
        for (std::size_t j = 0; j < words.size() - i - 1; j++) {
            if (words[j].getFrequency() < words[j + 1].getFrequency()) {
                WordStat temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void CSVWriter::write(std::ostream &out, std::vector<WordStat> words) {
    sortWords(words);
    out << "\xEF\xBB\xBF";
    out << "Слова:;Частота:\n";
    out << std::fixed << std::setprecision(1);
    for (const WordStat &item: words) {
        out << item.getWord() << ";" << item.getFrequency() << "%\n";
    }
}
