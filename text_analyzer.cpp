#include "text_analyzer.h"

void TextAnalyzer::addWord(std::string text) {
    totalWords++;
    for (WordStat &item: words) {
        if (item.getWord() == text) {
            item.increment();
            return;
        }
    }
    WordStat newWord(text);
    words.push_back(newWord);
}

void TextAnalyzer::calcFrequencies() {
    for (WordStat &item: words) {
        item.calcFrequency(totalWords);
    }
}

bool TextAnalyzer::isSeparator(char ch) const {
    if (ch == '\0')
        return true;
    for (char separator: separators) {
        if (ch == separator)
            return true;
    }
    return false;
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
    if (!currentWord.empty()) {
        addWord(currentWord);
    }

    calcFrequencies();
}

std::vector<WordStat> TextAnalyzer::getWords() const {
    return words;
}
