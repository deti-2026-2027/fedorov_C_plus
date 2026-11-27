#pragma once

#include "ICache.h"

// Калькулятор чисел Фибоначчи с кэшированием результатов
class FibonacciCalculator {
private:
    // Ссылка на используемый кэш
    ICache &cache;

    // Вычисляет число Фибоначчи итеративным методом
    long long fibonacci(int n);

public:
    // Принимает любую реализацию интерфейса ICache
    FibonacciCalculator(ICache &cache);

    // Возвращает результат из кэша или вычисляет его.
    // Допустимый диапазон аргумента: от 0 до 92.
    long long calculate(int n);
};
