﻿#include "pch.h"

#include "App.h"
#include "MainPage.h"
#include "Extension1.h"
#include "Extension2.h"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace UIExtAdvSample;
using namespace UIExtAdvSample::implementation;
using namespace Microsoft::Gaming::XboxGameBar;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
    Suspending({ this, &App::OnSuspending });

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

void App::OnActivated(IActivatedEventArgs const& e)
{
    XboxGameBarUIExtensionActivatedEventArgs uiExtArgs{ nullptr };
    if (e.Kind() == ActivationKind::Protocol)
    {
        auto protocolArgs = e.try_as<IProtocolActivatedEventArgs>();
        if (protocolArgs)
        {
            // If scheme name is ms-gamebaruiextension, Xbox Game Bar is activating us.
            const wchar_t* scheme = protocolArgs.Uri().SchemeName().c_str();
            if (0 != wcsstr(scheme, L"ms-gamebaruiextension"))
            {
                uiExtArgs = e.try_as<XboxGameBarUIExtensionActivatedEventArgs>();
            }
        }
    }
    if (uiExtArgs)
    {
        std::wstring appExtId{ uiExtArgs.AppExtensionId() };

        //
        // If IsLaunchActivation is true, this is Game Bar's initial activation of us 
        // and we MUST create and hold onto XboxGameBarUIExtension.
        //
        // Otherwise this is a subsequent activation coming from Game Bar. We should
        // continue to hold the XboxGameBarUIExtension created during initial activation
        // and just observe the URI command here and act accordingly. It is ok to
        // perform a navigate on the root frame to switch views/pages if needed.
        //
        if (uiExtArgs.IsLaunchActivation())
        {
            auto rootFrame = Frame();
            rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });
            Window::Current().Content(rootFrame);

            // Navigate to correct view
            
            if (0 == appExtId.compare(L"Extension1"))
            {
                m_uiExtension1 = XboxGameBarUIExtension(
                    uiExtArgs,
                    Window::Current().CoreWindow(),
                    rootFrame);
                rootFrame.Navigate(xaml_typename<UIExtAdvSample::Extension1>(), m_uiExtension1);
            }
            else if (0 == appExtId.compare(L"Extension1Settings"))
            {
                m_uiExtension1Settings = XboxGameBarUIExtension(
                    uiExtArgs,
                    Window::Current().CoreWindow(),
                    rootFrame);
                rootFrame.Navigate(xaml_typename<UIExtAdvSample::Extension1Settings>());
            }
            else if (0 == appExtId.compare(L"Extension2"))
            {
                m_uiExtension2 = XboxGameBarUIExtension(
                    uiExtArgs,
                    Window::Current().CoreWindow(),
                    rootFrame);
                rootFrame.Navigate(xaml_typename<UIExtAdvSample::Extension2>(), uiExtArgs.Uri());
            }
            else
            {
                // Unknown - Game Bar should never send you an unknown App Extension Id
                return;
            }

            Window::Current().Activate();
        }
        else if (0 == appExtId.compare(L"Extension2"))
        {
            // You can perform whatever behavior you need based on the URI payload. In our case
            // we're simply renavigating to Extension2 and displaying the absolute URI.  You
            // define your URI schema (subpath + query + fragment). 
            Frame rootFrame{ nullptr };
            auto content = Window::Current().Content();
            if (content)
            {
                rootFrame = content.try_as<Frame>();
            }
            rootFrame.Navigate(xaml_typename<UIExtAdvSample::Extension2>(), uiExtArgs.Uri());
        }
    }
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& e)
{
    Frame rootFrame{ nullptr };
    auto content = Window::Current().Content();
    if (content)
    {
        rootFrame = content.try_as<Frame>();
    }

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        rootFrame = Frame();

        rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });

        if (e.PreviousExecutionState() == ApplicationExecutionState::Terminated)
        {
            // Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete
        }

        if (e.PrelaunchActivated() == false)
        {
            if (rootFrame.Content() == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame.Navigate(xaml_typename<UIExtAdvSample::MainPage>(), box_value(e.Arguments()));
            }
            // Place the frame in the current Window
            Window::Current().Content(rootFrame);
            // Ensure the current window is active
            Window::Current().Activate();
        }
    }
    else
    {
        if (e.PrelaunchActivated() == false)
        {
            if (rootFrame.Content() == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame.Navigate(xaml_typename<UIExtAdvSample::MainPage>(), box_value(e.Arguments()));
            }
            // Ensure the current window is active
            Window::Current().Activate();
        }
    }
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    // Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(IInspectable const&, NavigationFailedEventArgs const& e)
{
    throw hresult_error(E_FAIL, hstring(L"Failed to load Page ") + e.SourcePageType().Name);
}