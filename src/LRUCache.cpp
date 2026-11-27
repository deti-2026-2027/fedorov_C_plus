#include "LRUCache.h"

#include <stdexcept>

// Конструктор
LRUCache::LRUCache(
    int capacity,
    std::optional<std::chrono::seconds> ttl
) : capacity(capacity), ttl(ttl) {
    if (capacity <= 0) {
        throw std::invalid_argument("Capacity must be positive");
    }

    if (ttl.has_value() && ttl.value() < std::chrono::seconds(0)) {
        throw std::invalid_argument("TTL must be non-negative");
    }
}

// Вычисление момента истечения TTL
std::optional<std::chrono::steady_clock::time_point>
LRUCache::makeExpiration() const {
    if (!ttl.has_value()) {
        return std::nullopt;
    }

    return std::chrono::steady_clock::now() + ttl.value();
}

// Проверка срока действия элемента
bool LRUCache::isExpired(const CacheItem &item) const {
    if (!item.expiresAt.has_value()) {
        return false;
    }

    return std::chrono::steady_clock::now() >= item.expiresAt.value();
}

// Удаление просроченных элементов
void LRUCache::removeExpired() {
    if (!ttl.has_value()) {
        return;
    }

    for (auto it = items.begin(); it != items.end();) {
        if (isExpired(*it)) {
            index.erase(it->key);
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}

// Получение элемента
std::optional<long long> LRUCache::get(int key) {
    auto it = index.find(key);

    if (it == index.end()) {
        return std::nullopt;
    }

    auto listIt = it->second;

    if (isExpired(*listIt)) {
        index.erase(it);
        items.erase(listIt);
        return std::nullopt;
    }

    items.splice(items.begin(), items, listIt);

    return listIt->value;
}

// Добавление или обновление элемента
void LRUCache::put(int key, long long value) {
    auto it = index.find(key);

    if (it != index.end()) {
        auto listIt = it->second;

        if (isExpired(*listIt)) {
            index.erase(it);
            items.erase(listIt);
        } else {
            listIt->value = value;
            listIt->expiresAt = makeExpiration();

            items.splice(items.begin(), items, listIt);
            return;
        }
    }

    if (items.size() == capacity) {
        removeExpired();

        if (items.size() == capacity) {
            index.erase(items.back().key);
            items.pop_back();
        }
    }

    items.push_front(CacheItem{key, value, makeExpiration()});
    index[key] = items.begin();
}
