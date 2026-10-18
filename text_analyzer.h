#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <vector>
#include <cstddef>
#include <istream>
#include <string>
#include "word_stat.h"

// Анализирует текстовый поток: разбивает на слова и считает их частоту
class TextAnalyzer {
private:
    std::vector<WordStat> words;
    std::size_t totalWords = 0;
    std::string separators = " \n\t\r.,!?;:\"()[]{}<>-/\\";

    void addWord(const std::string &text);

    void calcFrequencies();

    void sortWords();

    bool isSeparator(char ch) const;

public:
    // Читает поток до конца, формирует и сортирует статистику по убыванию частоты
    void analyze(std::istream &input);

    const std::vector<WordStat> &getWords() const;
};

#endif
