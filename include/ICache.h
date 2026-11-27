#pragma once

#include <optional>

class ICache {
public:
    virtual std::optional<long long> get(int key) = 0;

    virtual void put(int key, long long value) = 0;

    std::optional<long long> operator[](int key) {
        return get(key);
    }

    virtual ~ICache() = default;
};
