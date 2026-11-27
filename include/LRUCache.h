#pragma once

#include "ICache.h"

#include <chrono>
#include <optional>
#include <list>
#include <unordered_map>

// Кэш с вытеснением наиболее давно использованного элемента
class LRUCache : public ICache {
private:
    int capacity; // Максимальная вместимость кэша

    // Время жизни элементов (nullopt — TTL отключён)
    std::optional<std::chrono::seconds> ttl;

    // Элемент кэша
    struct CacheItem {
        int key;
        long long value;

        // Момент истечения времени жизни элемента
        std::optional<std::chrono::steady_clock::time_point> expiresAt;
    };

    // Список элементов в порядке использования:
    // начало — последний использованный,
    // конец — наиболее давно использованный
    std::list<CacheItem> items;

    // Хеш-таблица для быстрого доступа к элементам списка
    std::unordered_map<
        int,
        std::list<CacheItem>::iterator
    > index;

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
    LRUCache(
        int capacity,
        std::optional<std::chrono::seconds> ttl = std::nullopt
    );

    // Возвращает значение и обновляет порядок использования.
    // Если элемент отсутствует или просрочен, возвращает nullopt.
    std::optional<long long> get(int key) override;

    // Добавляет или обновляет элемент и устанавливает TTL.
    // При необходимости вытесняет наименее недавно
    // использованный элемент.
    void put(int key, long long value) override;
};
