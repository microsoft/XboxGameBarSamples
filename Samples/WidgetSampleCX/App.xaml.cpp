//
// App.xaml.cpp
// Implementation of the App class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "Widget1.xaml.h"

using namespace WidgetSampleCX;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    InitializeComponent();
    Suspending += ref new SuspendingEventHandler(this, &App::OnSuspending);
}

void App::OnActivated(Windows::ApplicationModel::Activation::IActivatedEventArgs^ e)
{
    XboxGameBarWidgetActivatedEventArgs^ widgetArgs = nullptr;
    if (e->Kind == ActivationKind::Protocol)
    {
        auto protocolArgs = dynamic_cast<IProtocolActivatedEventArgs^>(e);
        if (protocolArgs)
        {
            // If scheme name is ms-gamebarwidget, Xbox Game Bar is activating us.
            const wchar_t* scheme = protocolArgs->Uri->SchemeName->Data();
            if (0 == wcscmp(scheme, L"ms-gamebarwidget"))
            {
                widgetArgs = dynamic_cast<XboxGameBarWidgetActivatedEventArgs^>(e);
            }
        }
    }
    if (widgetArgs)
    {
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
        if (widgetArgs->IsLaunchActivation)
        {
            auto rootFrame = ref new Frame();
            rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);
            Window::Current->Content = rootFrame;

            // Create Game Bar widget object which bootstraps the connection with Game Bar
            m_widget1 = ref new XboxGameBarWidget(
                widgetArgs,
                Window::Current->CoreWindow,
                rootFrame);
            rootFrame->Navigate(TypeName(Widget1::typeid), nullptr);

            m_widget1WindowClosedHandlerToken = Window::Current->Closed += 
                ref new WindowClosedEventHandler(this, &WidgetSampleCX::App::Widget1WindowClosedHandler);

            Window::Current->Activate();
        }
    }
}

void App::Widget1WindowClosedHandler(Platform::Object^ /*sender*/, Windows::UI::Core::CoreWindowEventArgs^ /*e*/)
{
    m_widget1 = nullptr;
    Window::Current->Closed -= m_widget1WindowClosedHandlerToken;
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e)
{
    auto rootFrame = dynamic_cast<Frame^>(Window::Current->Content);

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        rootFrame = ref new Frame();

        rootFrame->NavigationFailed += ref new Windows::UI::Xaml::Navigation::NavigationFailedEventHandler(this, &App::OnNavigationFailed);

        if (e->PreviousExecutionState == ApplicationExecutionState::Terminated)
        {
            // TODO: Restore the saved session state only when appropriate, scheduling the
            // final launch steps after the restore is complete

        }

        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Place the frame in the current Window
            Window::Current->Content = rootFrame;
            // Ensure the current window is active
            Window::Current->Activate();
        }
    }
    else
    {
        if (e->PrelaunchActivated == false)
        {
            if (rootFrame->Content == nullptr)
            {
                // When the navigation stack isn't restored navigate to the first page,
                // configuring the new page by passing required information as a navigation
                // parameter
                rootFrame->Navigate(TypeName(MainPage::typeid), e->Arguments);
            }
            // Ensure the current window is active
            Window::Current->Activate();
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
void App::OnSuspending(Object^ sender, SuspendingEventArgs^ e)
{
    (void) sender;  // Unused parameter
    (void) e;   // Unused parameter

    m_widget1 = nullptr;
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e)
{
    throw ref new FailureException("Failed to load Page " + e->SourcePageType.Name);
}
