#pragma once

#include <wil\resource.h>
#include <wil\result_macros.h>
#include <winrt\Windows.Foundation.h>
#include <wrl\module.h>

namespace wrlrt
{
/// <summary>
/// This should be the base class for any winrt component that needs to plug into the overall WRL refcount for keeping the process alive.
/// To use, include this in your winrt::implements class mix-ins.
/// </summary>
/// <example> Runtime implementation class generated from WinRT IDL
///     <code>
///         struct MyComponent : MyComponentT<MyComponent, wrlrt::module_base>
///         {
///             // Override on_final_release if you wish to perform cleanup when the object is destroyed
///             winrt::Windows::Foundation::IAsyncAction on_final_release() override {}
///         }
///     <code>
/// </example>
/// <example> winrt:implements class
///     <code>
///         struct MyComponent : winrt::implements<MyComponent, wrlrt::module_base>
///         {
///             // Override on_final_release if you wish to perform cleanup when the object is destroyed
///             winrt::Windows::Foundation::IAsyncAction on_final_release() override {}
///         }
///     <code>
/// </example>
struct module_base : winrt::implements<module_base, winrt::Windows::Foundation::IInspectable>
{
    module_base()
    {
        if (auto modulePtr{ ::Microsoft::WRL::GetModuleBase() })
        {
            modulePtr->IncrementObjectCount();
        }
    }

    static winrt::fire_and_forget final_release(std::unique_ptr<module_base> self) noexcept
    try
    {
        try
        {
            co_await winrt::resume_background();
            co_await self->on_final_release();
            self = nullptr; // Destruct object
        }
        CATCH_LOG();

        if (auto modulePtr{ ::Microsoft::WRL::GetModuleBase() })
        {
            modulePtr->DecrementObjectCount();
        }
    }
    CATCH_LOG();

    virtual winrt::Windows::Foundation::IAsyncAction on_final_release()
    {
        co_return;
    }
};

template<typename ModuleImplClass>
class wrl_factory_for_winrt_com_class : public ::Microsoft::WRL::ClassFactory<>
{
public:
    IFACEMETHODIMP CreateInstance(_In_opt_::IUnknown* unknownOuter, REFIID riid, _COM_Outptr_ void** object) noexcept
    try
    {
        *object = nullptr;
        RETURN_HR_IF(CLASS_E_NOAGGREGATION, unknownOuter != nullptr);
        return winrt::make<ModuleImplClass>().as(riid, object);
    }
    CATCH_RETURN()
};

template<typename ModuleImplClass>
class wrl_factory_for_singleton_winrt_com_class : public ::Microsoft::WRL::ClassFactory<>
{
public:
    IFACEMETHODIMP CreateInstance(_In_opt_::IUnknown* unknownOuter, REFIID riid, _COM_Outptr_ void** object) noexcept
    try
    {
        *object = nullptr;
        RETURN_HR_IF(CLASS_E_NOAGGREGATION, unknownOuter != nullptr);
        return ModuleImplClass::Instance().as(riid, object);
    }
    CATCH_RETURN()
};

}

#define CoCreatableCppWinRtClass(className)                                                                            \
    CoCreatableClassWithFactory(className, ::wrlrt::wrl_factory_for_winrt_com_class<className>)
#define CoCreatableSingletonCppWinRtClass(className)                                                                   \
    CoCreatableClassWithFactory(className, ::wrlrt::wrl_factory_for_singleton_winrt_com_class<className>)