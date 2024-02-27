#pragma once
#include <memory>


template<typename T>
class Singleton
{
private:
	static std::unique_ptr<T> singletonInstance;
public:
	static T& Instance()
	{
		if (singletonInstance)
			return *singletonInstance.get();
		singletonInstance = std::make_unique<T>();
		return *singletonInstance.get();
	};
};
template<typename T>
std::unique_ptr<T> Singleton<T>::singletonInstance;

