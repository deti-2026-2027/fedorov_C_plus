#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <string>

// Класс для вывода сообщений об ошибках в стандартный поток ошибок
class ErrorHandler {
public:
    static void report(const std::string &message);
};

#endif
