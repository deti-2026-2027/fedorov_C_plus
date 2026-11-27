#include "LRUCache.h"
#include "LFUCache.h"
#include "FibonacciCalculator.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>

int main() {
    std::optional<std::chrono::seconds> ttl;

    int choice;
    int capacity;
    int ttlSeconds;

    std::cout << "Select cache (1 - LRU, 2 - LFU): ";
    if (!(std::cin >> choice)) {
        std::cout << "Invalid input\n";
        return 1;
    }

    std::cout << "Enter capacity: ";
    if (!(std::cin >> capacity)) {
        std::cout << "Invalid input\n";
        return 1;
    }
    if (capacity <= 0) {
        std::cout << "Capacity must be positive\n";
        return 1;
    }

    std::cout << "Enter TTL in seconds (-1 to disable): ";
    if (!(std::cin >> ttlSeconds)) {
        std::cout << "Invalid input\n";
        return 1;
    }

    if (ttlSeconds == -1) {
        ttl = std::nullopt;
    } else if (ttlSeconds < -1) {
        std::cout << "TTL out of bounds\n";
        return 1;
    } else {
        ttl = std::chrono::seconds(ttlSeconds);
    }

    // Выбираем реализацию кэша через общий интерфейс.
    std::unique_ptr<ICache> cache;

    if (choice == 1) {
        cache = std::make_unique<LRUCache>(capacity, ttl);
    } else if (choice == 2) {
        cache = std::make_unique<LFUCache>(capacity, ttl);
    } else {
        std::cout << "Invalid choice\n";
        return 1;
    }

    FibonacciCalculator calculator(*cache);

    // Принимаем запросы до ввода -1 или ошибки чтения.
    while (true) {
        int n;
        std::cout << "Enter n: ";

        if (!(std::cin >> n)) {
            break;
        }

        if (n == -1) {
            break;
        }

        try {
            long long result = calculator.calculate(n);
            std::cout << "Result: " << result << '\n';
        } catch (const std::invalid_argument &e) {
            std::cout << e.what() << '\n';
        }
    }

    return 0;
}
