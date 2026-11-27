#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <optional>
#include <stdexcept>

#include "LRUCache.h"
#include "LFUCache.h"
#include "FibonacciCalculator.h"

// ==================== LRU ====================

// Получение отсутствующего элемента
TEST(LRUCacheTest, MissingKey) {
    LRUCache cache(3);
    EXPECT_FALSE(cache.get(1).has_value());
}

// Добавление и получение элемента
TEST(LRUCacheTest, PutAndGet) {
    LRUCache cache(3);
    cache.put(1, 10);
    auto result = cache.get(1);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 10);
}

// Обновление существующего элемента
TEST(LRUCacheTest, UpdateValue) {
    LRUCache cache(3);
    cache.put(1, 10);
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100LL);
}

// Вытеснение самого старого элемента
TEST(LRUCacheTest, Eviction) {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    EXPECT_FALSE(cache.get(1).has_value());
    EXPECT_EQ(cache.get(2), 20LL);
    EXPECT_EQ(cache.get(3), 30LL);
}

// get изменяет порядок использования
TEST(LRUCacheTest, GetChangesOrder) {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.get(1);
    cache.put(3, 30);
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_EQ(cache.get(1), 10LL);
    EXPECT_EQ(cache.get(3), 30LL);
}

// put изменяет порядок использования
TEST(LRUCacheTest, PutChangesOrder) {
    LRUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(1, 100);
    cache.put(3, 30);
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_EQ(cache.get(1), 100LL);
    EXPECT_EQ(cache.get(3), 30LL);
}

// Некорректная вместимость
TEST(LRUCacheTest, InvalidCapacity) {
    EXPECT_THROW(LRUCache(0), std::invalid_argument);
    EXPECT_THROW(LRUCache(-1), std::invalid_argument);
}

// ==================== LFU ====================

// Получение отсутствующего элемента
TEST(LFUCacheTest, MissingKey) {
    LFUCache cache(3);
    EXPECT_FALSE(cache.get(1).has_value());
}

// Добавление и получение элемента
TEST(LFUCacheTest, PutAndGet) {
    LFUCache cache(3);
    cache.put(1, 10);
    EXPECT_EQ(cache.get(1), 10LL);
}

// Обновление существующего элемента
TEST(LFUCacheTest, UpdateValue) {
    LFUCache cache(3);
    cache.put(1, 10);
    cache.put(1, 100);
    EXPECT_EQ(cache.get(1), 100LL);
}

// Вытеснение элемента с минимальной частотой
TEST(LFUCacheTest, EvictionByFrequency) {
    LFUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.get(1);
    cache.get(1);
    cache.get(2);
    cache.put(3, 30);
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_EQ(cache.get(1), 10LL);
    EXPECT_EQ(cache.get(3), 30LL);
}

// При равной частоте вытесняется самый давно использованный
TEST(LFUCacheTest, EvictionByLastUsed) {
    LFUCache cache(2);
    cache.put(1, 10);
    cache.put(2, 20);
    cache.get(2);
    cache.get(1);
    cache.put(3, 30);
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_EQ(cache.get(1), 10LL);
    EXPECT_EQ(cache.get(3), 30LL);
}

// Обновление элемента увеличивает его частоту
TEST(LFUCacheTest, PutIncreasesFrequency) {
    LFUCache cache(2);
    cache.put(1, 10);
    cache.put(1, 100);
    cache.put(2, 20);
    cache.put(3, 30);
    EXPECT_FALSE(cache.get(2).has_value());
    EXPECT_EQ(cache.get(1), 100LL);
    EXPECT_EQ(cache.get(3), 30LL);
}

// Некорректная вместимость
TEST(LFUCacheTest, InvalidCapacity) {
    EXPECT_THROW(LFUCache(0), std::invalid_argument);
    EXPECT_THROW(LFUCache(-1), std::invalid_argument);
}

// ==================== ICache ====================

// Проверка оператора [] для LRU
TEST(ICacheTest, LRUOperator) {
    LRUCache cache(3);
    ICache &interface = cache;
    interface.put(1, 10);
    EXPECT_EQ(interface[1], 10LL);
    EXPECT_FALSE(interface[2].has_value());
}

// Проверка оператора [] для LFU
TEST(ICacheTest, LFUOperator) {
    LFUCache cache(3);
    ICache &interface = cache;
    interface.put(1, 10);
    EXPECT_EQ(interface[1], 10LL);
    EXPECT_FALSE(interface[2].has_value());
}

// ==================== TTL ====================

// Получение значения до истечения TTL
TEST(TTLTest, NotExpired) {
    LRUCache lru(3, std::chrono::seconds(5));
    LFUCache lfu(3, std::chrono::seconds(5));
    lru.put(1, 10);
    lfu.put(1, 10);
    EXPECT_EQ(lru.get(1), 10LL);
    EXPECT_EQ(lfu.get(1), 10LL);
}

// Истечение времени жизни
TEST(TTLTest, Expired) {
    LRUCache lru(3, std::chrono::seconds(1));
    LFUCache lfu(3, std::chrono::seconds(1));
    lru.put(1, 10);
    lfu.put(1, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_FALSE(lru.get(1).has_value());
    EXPECT_FALSE(lfu.get(1).has_value());
}

// Нулевой TTL
TEST(TTLTest, ZeroTTL) {
    LRUCache lru(3, std::chrono::seconds(0));
    LFUCache lfu(3, std::chrono::seconds(0));
    lru.put(1, 10);
    lfu.put(1, 10);
    EXPECT_FALSE(lru.get(1).has_value());
    EXPECT_FALSE(lfu.get(1).has_value());
}

// Отключённый TTL
TEST(TTLTest, DisabledTTL) {
    LRUCache lru(3, std::nullopt);
    LFUCache lfu(3, std::nullopt);
    lru.put(1, 10);
    lfu.put(1, 10);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(lru.get(1), 10LL);
    EXPECT_EQ(lfu.get(1), 10LL);
}

// Отрицательный TTL
TEST(TTLTest, InvalidTTL) {
    EXPECT_THROW(LRUCache(3, std::chrono::seconds(-1)), std::invalid_argument);
    EXPECT_THROW(LFUCache(3, std::chrono::seconds(-1)), std::invalid_argument);
}

// put продлевает время жизни элемента
TEST(TTLTest, PutRefreshesTTL) {
    LRUCache lru(3, std::chrono::seconds(2));
    LFUCache lfu(3, std::chrono::seconds(2));
    lru.put(1, 10);
    lfu.put(1, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    lru.put(1, 100);
    lfu.put(1, 100);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_EQ(lru.get(1), 100LL);
    EXPECT_EQ(lfu.get(1), 100LL);
}

// get не продлевает время жизни
TEST(TTLTest, GetDoesNotRefreshTTL) {
    LRUCache lru(3, std::chrono::seconds(2));
    LFUCache lfu(3, std::chrono::seconds(2));
    lru.put(1, 10);
    lfu.put(1, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_EQ(lru.get(1), 10LL);
    EXPECT_EQ(lfu.get(1), 10LL);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    EXPECT_FALSE(lru.get(1).has_value());
    EXPECT_FALSE(lfu.get(1).has_value());
}

// Очистка просроченных элементов перед вытеснением
TEST(TTLTest, RemoveExpiredBeforeEviction) {
    LRUCache lru(2, std::chrono::seconds(2));
    LFUCache lfu(2, std::chrono::seconds(2));
    lru.put(1, 10);
    lfu.put(1, 10);
    lfu.get(1);
    lfu.get(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    lru.put(2, 20);
    lfu.put(2, 20);
    lru.get(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    lru.put(3, 30);
    lfu.put(3, 30);
    EXPECT_FALSE(lru.get(1).has_value());
    EXPECT_FALSE(lfu.get(1).has_value());
    EXPECT_EQ(lru.get(2), 20LL);
    EXPECT_EQ(lfu.get(2), 20LL);
    EXPECT_EQ(lru.get(3), 30LL);
    EXPECT_EQ(lfu.get(3), 30LL);
}

// Повторная вставка просроченного элемента
TEST(TTLTest, ReinsertExpired) {
    LRUCache lru(2, std::chrono::seconds(1));
    LFUCache lfu(2, std::chrono::seconds(1));
    lru.put(1, 10);
    lfu.put(1, 10);
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    lru.put(1, 100);
    lfu.put(1, 100);
    EXPECT_EQ(lru.get(1), 100LL);
    EXPECT_EQ(lfu.get(1), 100LL);
}

// ==================== FIBONACCI ====================

// Корректность вычислений
TEST(FibonacciTest, CorrectValues) {
    LRUCache cache(5);
    FibonacciCalculator calculator(cache);
    EXPECT_EQ(calculator.calculate(0), 0LL);
    EXPECT_EQ(calculator.calculate(1), 1LL);
    EXPECT_EQ(calculator.calculate(10), 55LL);
    EXPECT_EQ(calculator.calculate(20), 6765LL);
    EXPECT_EQ(calculator.calculate(92), 7540113804746346429LL);
}

// Некорректные аргументы
TEST(FibonacciTest, InvalidArgument) {
    LRUCache cache(3);
    FibonacciCalculator calculator(cache);
    EXPECT_THROW(calculator.calculate(-1), std::invalid_argument);
    EXPECT_THROW(calculator.calculate(93), std::invalid_argument);
}

// Результат вычисления сохраняется в кэше
TEST(FibonacciTest, StoresResultInCache) {
    LRUCache cache(3);
    FibonacciCalculator calculator(cache);
    EXPECT_EQ(calculator.calculate(10), 55LL);
    EXPECT_EQ(cache.get(10), 55LL);
}

// Калькулятор использует сохранённое значение
TEST(FibonacciTest, UsesCachedValue) {
    LRUCache cache(3);
    FibonacciCalculator calculator(cache);
    cache.put(10, 999);
    EXPECT_EQ(calculator.calculate(10), 999LL);
}
