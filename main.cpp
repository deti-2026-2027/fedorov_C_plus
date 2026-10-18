#include <fstream>
#include "text_analyzer.h"
#include "csv_writer.h"
#include "error_handler.h"

int main(int argc, char *argv[]) {
    // Ожидаем 2 аргумента: argv[1] — путь к входному файлу, argv[2] — путь к выходному CSV
    if (argc != 3) {
        ErrorHandler::report("Необходимы входной и выходной файлы");
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        ErrorHandler::report("Входной файл открыть не удалось");
        return 1;
    }
    TextAnalyzer analyzer;
    analyzer.analyze(input);

    if (input.bad()) {
        ErrorHandler::report("Ошибка чтения");
        return 1;
    }

    std::ofstream output(argv[2]);
    if (!output) {
        ErrorHandler::report("Выходной файл открыть не удалось");
        return 1;
    }

    const std::vector<WordStat> &statistics = analyzer.getWords();
    CSVWriter writer(';');
    writer.write(output, statistics);
    output.close();
    if (!output) {
        ErrorHandler::report("Ошибка записи");
        return 1;
    }
    return 0;
}
