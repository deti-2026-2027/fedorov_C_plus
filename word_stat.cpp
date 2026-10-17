#include "word_stat.h"

#include <utility>

WordStat::WordStat(std::string text) {
    word = std::move(text);
    count = 1;
}

void WordStat::increment() {
    count++;
}

std::string WordStat::getWord() const {
    return word;
}

std::size_t WordStat::getCount() const {
    return count;
}

double WordStat::getFrequency() const {
    return frequency;
}

void WordStat::calcFrequency(std::size_t totalWords) {
    if (totalWords == 0)
        frequency = 0.0;
    else
        frequency = count / (double) totalWords * 100.0;
}
