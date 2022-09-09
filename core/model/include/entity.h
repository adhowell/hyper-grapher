#pragma once


namespace core
{
class Entity {
public:
    explicit Entity(uint32_t uuid) : mUuid(uuid) {}
    ~Entity() = default;

    uint32_t getUuid() const { return mUuid; }

private:
    uint32_t mUuid;
};
}