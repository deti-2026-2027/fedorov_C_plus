#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <string>

//класс для вывода ошибок
class ErrorHandler {
public:
    static void report(const std::string &message);
};

#endif
