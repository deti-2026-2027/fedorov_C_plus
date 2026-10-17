#include "error_handler.h"
#include <iostream>

void ErrorHandler::report(const std::string &message) {
    std::cerr << "Ошибка: " << message << "\n";
}
