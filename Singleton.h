#pragma once
#include <memory>

using namespace std;

template<typename T>
class Singleton
{
private:
    static unique_ptr<T> singletonInstance;
public:
    static T& GetInstance()
    {
        if (singletonInstance)
            return *singletonInstance.get();
        singletonInstance = make_unique<T>();
        return *singletonInstance.get();
    };
};
template<typename T>
unique_ptr<T> Singleton<T>::singletonInstance;

