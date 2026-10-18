#ifndef WORD_STAT_H
#define WORD_STAT_H

#include <string>
#include <cstddef>

// Хранит статистику вхождений отдельного слова в тексте
class WordStat {
private:
    std::string word;
    std::size_t count = 0;
    double frequency = 0.0; // Относительная частота в процентах (0..100%)

public:
    WordStat(std::string text);

    void increment();

    const std::string &getWord() const;

    std::size_t getCount() const;

    double getFrequency() const;

    // Вычисляет процентное соотношение слова относительно общего числа слов    
    void calcFrequency(std::size_t totalWords);
};

#endif
