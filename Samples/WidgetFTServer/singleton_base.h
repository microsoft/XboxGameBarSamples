#pragma once

#include <memory>
#include <wil\resource.h>

#include <winrt\base.h>

namespace xblib
{
#pragma region cpp

/// <summary>
/// Singleton template. Likely don't need to inherit directly from this. Use 
/// singleton_base or singleton_initialize_base if they work for you.
/// 
/// Base class implementation for creating a Singleton object that uses shared_ptr and weak_ptr.
/// The singleton is stored a static weak_ptr so the reference must be held externally by the consumer
/// in order to manage the lifetime.
/// </summary>
template<typename TSingleton, typename TFactory>
class singleton_with_factory
{
    friend TFactory;

public:
    //! Returns the singleton instance of the object.
    static std::shared_ptr<TSingleton> Instance()
    {
        auto lock{ s_lock.lock_exclusive() };
        auto instance{ s_instance.lock() };
        if (!instance)
        {
            auto factory{ TFactory() };
            instance = factory();
            s_instance = instance;
        }
        return instance;
    }

    // The base Singleton class must not support copy construction or copy assignment.
    singleton_with_factory(const singleton_with_factory&) = delete;
    const singleton_with_factory& operator=(const singleton_with_factory&) = delete;

protected:
    virtual ~singleton_with_factory() = default;
    singleton_with_factory() = default;

private:
    static wil::srwlock s_lock;
    static std::weak_ptr<TSingleton> s_instance;
};

template<typename TSingleton>
struct singleton_initialize_factory
{
    std::shared_ptr<TSingleton> operator()()
    {
        auto instance{ std::make_shared<TSingleton>() };
        instance->SingletonInitialize();
        return instance;
    }
};

template<typename TSingleton>
struct singleton_default_factory
{
    std::shared_ptr<TSingleton> operator()()
    {
        return std::make_shared<TSingleton>();
    }
};

template<typename TSingleton, typename TFactory>
wil::srwlock singleton_with_factory<TSingleton, TFactory>::s_lock;

template<typename TSingleton, typename TFactory>
std::weak_ptr<TSingleton> singleton_with_factory<TSingleton, TFactory>::s_instance;

/// <summary>
/// Inherit to make your class a singleton. Client's acquire the singleton with YourClass::Instance()
/// If you want an Initialize method outside of construction, use singleton_initialize_base
/// </summary>
template<typename TSingleton>
using singleton_base = singleton_with_factory<TSingleton, singleton_default_factory<TSingleton>>;

/// <summary>
/// Inherit to make your class a singleton with an explicit initialize method called after construction.
/// Clients acquire the singleton with YourClass::Instance()
/// You must implement SingletonInitialize method. 
/// </summary>
template<typename TSingleton>
using singleton_initialize_base = singleton_with_factory<TSingleton, singleton_initialize_factory<TSingleton>>;

#pragma endregion

#pragma region winrt

/// <summary>
/// Singleton template. Likely don't need to inherit directly from this. Use 
/// singleton_winrt_base or singleton_initialize_winrt_base if they work for you.
/// 
/// Base class implementation for creating a singleton object that uses com_ptr and weak_ref.
/// The singleton is stored a static weak_ref so the reference must be held externally by the consumer
/// in order to manage the lifetime.
/// </summary>
template<typename TSingleton, typename TFactory>
class singleton_with_factory_winrt
{
    friend TFactory;

public:
    //! Returns the singleton instance of the object.
    static winrt::com_ptr<TSingleton> Instance()
    {
        auto lock{ s_lock.lock_exclusive() };
        auto instance{ s_instance.get() };
        if (!instance)
        {
            auto factory{ TFactory() };
            instance = factory();
            s_instance = instance;
        }
        return instance;
    }

    // The base Singleton class must not support copy construction or copy assignment.
    singleton_with_factory_winrt(const singleton_with_factory_winrt&) = delete;
    const singleton_with_factory_winrt& operator=(const singleton_with_factory_winrt&) = delete;

protected:
    virtual ~singleton_with_factory_winrt() = default;
    singleton_with_factory_winrt() = default;

private:
    static wil::srwlock s_lock;
    static winrt::weak_ref<TSingleton> s_instance;
};

template<typename TSingleton>
struct singleton_initialize_factory_winrt
{
    winrt::com_ptr<TSingleton> operator()()
    {
        auto instance{ winrt::make_self<TSingleton>() };
        instance->SingletonInitialize();
        return instance;
    }
};

template<typename TSingleton>
struct singleton_default_factory_winrt
{
    winrt::com_ptr<TSingleton> operator()()
    {
        auto instance{ winrt::make_self<TSingleton>() };
        return instance;
    }
};

template<typename TSingleton, typename TFactory>
wil::srwlock singleton_with_factory_winrt<TSingleton, TFactory>::s_lock;

template<typename TSingleton, typename TFactory>
winrt::weak_ref<TSingleton> singleton_with_factory_winrt<TSingleton, TFactory>::s_instance;

/// <summary>
/// Inherit to make your class a singleton. Client's acquire the singleton with YourClass::Instance()
/// If you want an Initialize method outside of construction, use singleton_initialize_winrt_base
/// </summary>
template<typename TSingleton>
using singleton_winrt_base = singleton_with_factory_winrt<TSingleton, singleton_default_factory_winrt<TSingleton>>;

/// <summary>
/// Inherit to make your class a singleton with an explicit initialize method called after construction.
/// Clients acquire the singleton with YourClass::Instance()
/// You must implement SingletonInitialize method. 
/// </summary>
template<typename TSingleton>
using singleton_initialize_winrt_base = singleton_with_factory_winrt<TSingleton, singleton_initialize_factory_winrt<TSingleton>>;

#pragma endregion
}
