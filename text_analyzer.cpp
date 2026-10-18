#include "text_analyzer.h"
#include <algorithm>

void TextAnalyzer::addWord(const std::string &text) {
    totalWords++;
    for (WordStat &item: words) {
        if (item.getWord() == text) {
            item.increment();
            return;
        }
    }
    words.emplace_back(text);
}

void TextAnalyzer::calcFrequencies() {
    for (WordStat &item: words) {
        item.calcFrequency(totalWords);
    }
}

void TextAnalyzer::sortWords() {
    std::sort(words.begin(), words.end(), [](const WordStat &a, const WordStat &b) {
        return a.getCount() > b.getCount();
    });
}


bool TextAnalyzer::isSeparator(char ch) const {
    return separators.find(ch) != std::string::npos;
}

void TextAnalyzer::analyze(std::istream &input) {
    words.clear();
    totalWords = 0;
    std::string currentWord;
    char ch;

    while (input.get(ch)) {
        if (!isSeparator(ch)) {
            currentWord += ch;
        } else if (!currentWord.empty()) {
            addWord(currentWord);
            currentWord.clear();
        }
    }
    // Добавляем последнее слово, если файл не заканчивался символом-разделителем
    if (!currentWord.empty()) {
        addWord(currentWord);
    }

    calcFrequencies();
    sortWords();
}

const std::vector<WordStat> &TextAnalyzer::getWords() const {
    return words;
}
