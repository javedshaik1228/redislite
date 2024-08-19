#pragma once

template <typename T>
class GenericSingleton
{
public:
    static T *getInstance()
    {
        static T instance;
        return &instance;
    }

    GenericSingleton(const GenericSingleton &) = delete;
    GenericSingleton &operator=(const GenericSingleton &) = delete;

protected:
    GenericSingleton() = default;
    virtual ~GenericSingleton() = default;
};