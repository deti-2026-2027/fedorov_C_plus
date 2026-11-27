#include "FibonacciCalculator.h"

#include <stdexcept>

FibonacciCalculator::FibonacciCalculator(ICache &cache)
    : cache(cache) {
}

long long FibonacciCalculator::fibonacci(int n) {
    if (n < 0 || n > 92) {
        throw std::invalid_argument(
            "Fibonacci calculator out of bounds"
        );
    }

    if (n == 0) {
        return 0;
    }

    if (n == 1) {
        return 1;
    }

    long long fib_1 = 0;
    long long fib_2 = 1;

    for (int i = 2; i <= n; i++) {
        long long tmp = fib_2;
        fib_2 = fib_1 + fib_2;
        fib_1 = tmp;
    }

    return fib_2;
}

long long FibonacciCalculator::calculate(int n) {
    if (n < 0 || n > 92) {
        throw std::invalid_argument(
            "Fibonacci calculator out of bounds"
        );
    }

    // Проверяем наличие результата в кэше
    auto result = cache[n];

    if (result.has_value()) {
        return result.value();
    }

    // При промахе вычисляем и сохраняем результат
    long long value = fibonacci(n);
    cache.put(n, value);

    return value;
}
