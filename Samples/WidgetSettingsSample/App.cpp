#include "pch.h"

#include "App.h"
#include "Widget.h"

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace WidgetSettingsSample;
using namespace WidgetSettingsSample::implementation;
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
    XboxGameBarWidgetActivatedEventArgs widgetArgs{ nullptr };
    if (e.Kind() == ActivationKind::Protocol)
    {
        auto protocolArgs = e.try_as<IProtocolActivatedEventArgs>();
        if (protocolArgs)
        {
            // If scheme name is ms-gamebarwidget, Xbox Game Bar is activating us.
            const wchar_t* scheme = protocolArgs.Uri().SchemeName().c_str();
            if (0 == wcscmp(scheme, L"ms-gamebarwidget"))
            {
                widgetArgs = e.try_as<XboxGameBarWidgetActivatedEventArgs>();
            }
        }
    }
    if (widgetArgs)
    {
        std::wstring appExtId{ widgetArgs.AppExtensionId() };

        //
        // Activation Notes:
        //
        //    If IsLaunchActivation is true, this is Game Bar launching a new instance
        // of our widget. This means we have a NEW CoreWindow with corresponding UI
        // dispatcher, and we MUST create and hold onto a new XboxGameBarWidget.
        //
        // Otherwise this is a subsequent activation coming from Game Bar. We MUST
        // continue to hold the XboxGameBarWidget created during initial activation
        // and ignore this repeat activation, or just observe the URI command here and act 
        // accordingly.  It is ok to perform a navigate on the root frame to switch 
        // views/pages if needed.  Game Bar lets us control the URI for sending widget to
        // widget commands or receiving a command from another non-widget process. 
        //
        // Important Cleanup Notes:
        //    When our widget is closed--by Game Bar or us calling XboxGameBarWidget.Close()-,
        // the CoreWindow will get a closed event.  We can register for Window.Closed
        // event to know when our particular widget has shutdown, and cleanup accordingly.
        //
        // NOTE: If a widget's CoreWindow is the LAST CoreWindow being closed for the process
        // then we won't get the Window.Closed event.  However, we will get the OnSuspending
        // call and can use that for cleanup.
        //
        if (widgetArgs.IsLaunchActivation())
        {
            auto rootFrame = Frame();
            rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });
            Window::Current().Content(rootFrame);

            if (0 == appExtId.compare(L"Widget"))
            {
                m_widget1 = XboxGameBarWidget(
                    widgetArgs,
                    Window::Current().CoreWindow(),
                    rootFrame);
                rootFrame.Navigate(xaml_typename<WidgetSettingsSample::Widget>(), m_widget1);

                m_widget1WindowClosedHandlerToken = Window::Current().Closed(
                    { get_weak(), &App::Widget1WindowClosedHandler });
            }
            else if (0 == appExtId.compare(L"WidgetSettings"))
            {
                m_widget1Settings = XboxGameBarWidget(
                    widgetArgs,
                    Window::Current().CoreWindow(),
                    rootFrame);
                rootFrame.Navigate(xaml_typename<WidgetSettingsSample::WidgetSettings>());

                m_widget1SettingsWindowClosedHandlerToken = Window::Current().Closed(
                    { get_weak(), &App::Widget1SettingsWindowClosedHandler });
            }
            else
            {
                // Unknown - Game Bar should never send you an unknown App Extension Id
                return;
            }

            Window::Current().Activate();
        }
        else
        {
            // You can perform whatever behavior you need based on the URI payload.
        }
    }
}

void App::Widget1WindowClosedHandler(IInspectable const&, IInspectable const&)
{
    m_widget1 = nullptr;
    Window::Current().Closed(m_widget1WindowClosedHandlerToken);
}

void App::Widget1SettingsWindowClosedHandler(IInspectable const&, IInspectable const&)
{
    m_widget1Settings = nullptr;
    Window::Current().Closed(m_widget1SettingsWindowClosedHandlerToken);
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
                rootFrame.Navigate(xaml_typename<WidgetSettingsSample::MainPage>(), box_value(e.Arguments()));
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
                rootFrame.Navigate(xaml_typename<WidgetSettingsSample::MainPage>(), box_value(e.Arguments()));
            }
            // Ensure the current window is active
            Window::Current().Activate();
        }
    }
}

/// <summary>
/// Invoked when application execution is being suspended.  Normally we
/// wouldn't know if the app was being terminated or just suspended at this
/// point. However, the app will never be suspended if Game Bar has an
/// active widget connection to it, so if you see this call it's safe to
/// cleanup any widget related objects. Keep in mind if all widgets are closed
/// and you have a foreground window for your app, this could still result in 
/// suspend or terminate. Regardless, it should always be safe to cleanup
/// your widget related objects.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    m_widget1 = nullptr;
    m_widget1Settings = nullptr;
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