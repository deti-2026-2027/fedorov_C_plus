#pragma once

#include "ICache.h"

#include <chrono>
#include <optional>
#include <unordered_map>

// Кэш с вытеснением наименее часто используемого элемента.
// При равной частоте вытесняется наиболее давно использованный элемент.
class LFUCache : public ICache {
private:
    int capacity; // Максимальная вместимость кэша

    // Счётчик операций использования элементов
    long long counter;

    // Время жизни элементов (nullopt — TTL отключён)
    std::optional<std::chrono::seconds> ttl;

    // Элемент кэша
    struct CacheItem {
        long long value;
        long long frequency; // Частота использования
        long long lastUsed; // Номер последней операции использования

        // Момент истечения времени жизни элемента
        std::optional<std::chrono::steady_clock::time_point> expiresAt;
    };

    // Хеш-таблица для хранения элементов и быстрого доступа по ключу
    std::unordered_map<int, CacheItem> items;

    // Вычисляет момент истечения TTL
    std::optional<std::chrono::steady_clock::time_point>
    makeExpiration() const;

    // Проверяет, истекло ли время жизни элемента
    bool isExpired(const CacheItem &item) const;

    // Удаляет все просроченные элементы
    void removeExpired();

public:
    // Создаёт кэш заданной вместимости с опциональным TTL.
    // Выбрасывает invalid_argument при capacity <= 0 или TTL < 0.
    LFUCache(
        int capacity,
        std::optional<std::chrono::seconds> ttl = std::nullopt
    );

    // Возвращает значение и обновляет частоту использования.
    // Если элемент отсутствует или просрочен, возвращает nullopt.
    std::optional<long long> get(int key) override;

    // Добавляет или обновляет элемент и устанавливает TTL.
    // При необходимости вытесняет элемент по алгоритму LFU.
    void put(int key, long long value) override;
};
