#ifndef WORD_STAT_H
#define WORD_STAT_H

#include <string>
#include <cstddef>
#include <vector>

//класс, отвечающий за информацию о слове
class WordStat {
private:
    std::string word;
    std::size_t count = 0;
    double frequency = 0.0;

public:
    WordStat(std::string text);

    void increment(); //увеличение счетчика слов
    std::string getWord() const;

    std::size_t getCount() const;

    double getFrequency() const;

    void calcFrequency(std::size_t totalWords);
};

#endif
