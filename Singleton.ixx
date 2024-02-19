module;
#include <memory>
export module utils:singleton;

using namespace std;

export
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
