#include "LFUCache.h"

#include <stdexcept>

LFUCache::LFUCache(
    int capacity,
    std::optional<std::chrono::seconds> ttl
) : capacity(capacity), counter(0), ttl(ttl) {
    if (capacity <= 0) {
        throw std::invalid_argument("Capacity must be positive");
    }

    if (ttl.has_value() && ttl.value() < std::chrono::seconds(0)) {
        throw std::invalid_argument("TTL must be non-negative");
    }
}

std::optional<std::chrono::steady_clock::time_point>
LFUCache::makeExpiration() const {
    if (!ttl.has_value()) {
        return std::nullopt;
    }

    return std::chrono::steady_clock::now() + ttl.value();
}

bool LFUCache::isExpired(const CacheItem &item) const {
    if (!item.expiresAt.has_value()) {
        return false;
    }

    return std::chrono::steady_clock::now() >= item.expiresAt.value();
}

void LFUCache::removeExpired() {
    if (!ttl.has_value()) {
        return;
    }

    for (auto it = items.begin(); it != items.end();) {
        if (isExpired(it->second)) {
            it = items.erase(it);
        } else {
            ++it;
        }
    }
}

std::optional<long long> LFUCache::get(int key) {
    auto it = items.find(key);

    if (it == items.end()) {
        return std::nullopt;
    }

    // Просроченный элемент больше не считается частью кэша
    if (isExpired(it->second)) {
        items.erase(it);
        return std::nullopt;
    }

    it->second.frequency++;
    it->second.lastUsed = ++counter;

    return it->second.value;
}

void LFUCache::put(int key, long long value) {
    auto it = items.find(key);

    if (it != items.end()) {
        if (!isExpired(it->second)) {
            it->second.value = value;
            it->second.frequency++;
            it->second.lastUsed = ++counter;
            it->second.expiresAt = makeExpiration();
            return;
        }

        // Просроченный элемент будет добавлен заново
        // с начальной частотой использования
        items.erase(it);
    }

    if (items.size() == capacity) {
        // Сначала освобождаем место от просроченных элементов
        removeExpired();

        if (items.size() == capacity) {
            auto candidate = items.begin();

            // Ищем элемент с минимальной частотой.
            // При равенстве частот выбираем по lastUsed.
            for (auto iter = items.begin(); iter != items.end(); ++iter) {
                if ((iter->second.frequency < candidate->second.frequency) ||
                    (iter->second.frequency == candidate->second.frequency &&
                     iter->second.lastUsed < candidate->second.lastUsed)) {
                    candidate = iter;
                }
            }

            items.erase(candidate);
        }
    }

    items[key] = CacheItem{value, 1, ++counter, makeExpiration()};
}
