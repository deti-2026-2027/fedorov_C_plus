#ifndef TEXT_ANALYZER_H
#define TEXT_ANALYZER_H

#include <vector>
#include <cstddef>
#include <istream>
#include "word_stat.h"

//класс для обработки текста
class TextAnalyzer {
private:
    std::vector<WordStat> words;
    std::size_t totalWords = 0;
    std::string separators = " \n\t\r.,!?;:\"()[]{}<>-/\\";

    void addWord(std::string word);

    void calcFrequencies();

    bool isSeparator(char ch) const;

public:
    void analyze(std::istream &input);

    std::vector<WordStat> getWords() const;
};

#endif
