#include "pch.h"

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
using namespace UIExtSample;
using namespace UIExtSample::implementation;
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
			const wchar_t* scheme = protocolArgs.Uri().SchemeName().c_str();
			if (0 != wcsstr(scheme, L"ms-gamebaruiextension"))
			{
				uiExtArgs = e.try_as<XboxGameBarUIExtensionActivatedEventArgs>();
			}
		}
	}
	if (uiExtArgs)
	{
		auto rootFrame = Frame();
		rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });
        Window::Current().Content(rootFrame);

        // Navigate to correct view
        std::wstring appExtId{ uiExtArgs.AppExtensionId() };
        if (0 == appExtId.compare(L"Extension1"))
        {
            m_uiExtension1 = XboxGameBarUIExtension(
                uiExtArgs,
                Window::Current().CoreWindow(),
                rootFrame);
            rootFrame.Navigate(xaml_typename<UIExtSample::Extension1>());
        }
        else if (0 == appExtId.compare(L"Extension2"))
        {
            m_uiExtension2 = XboxGameBarUIExtension(
                uiExtArgs,
                Window::Current().CoreWindow(),
                rootFrame);
            rootFrame.Navigate(xaml_typename<UIExtSample::Extension2>());
        }
        else
        {
            // Unknown - Game Bar should never send you an unknown App Extension Id
            return;
        }

		Window::Current().Activate();
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
                rootFrame.Navigate(xaml_typename<UIExtSample::MainPage>(), box_value(e.Arguments()));
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
                rootFrame.Navigate(xaml_typename<UIExtSample::MainPage>(), box_value(e.Arguments()));
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