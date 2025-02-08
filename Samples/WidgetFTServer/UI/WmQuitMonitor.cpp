#include "pch.h"
#include "WmQuitMonitor.h"

namespace XboxGameBarFT::UI
{

std::shared_ptr<WmQuitMonitor> WmQuitMonitor::Create(HINSTANCE hInstance, std::function<void()> onWmQuitCallback)
{
    THROW_HR_IF(E_INVALIDARG, !onWmQuitCallback);

    auto self{ std::make_shared<WmQuitMonitor>(hInstance, onWmQuitCallback) };
    self->Initialize();
    return self;
}

void WmQuitMonitor::Initialize()
{
    m_thread = std::thread([strongThis{ shared_from_this() }]()
    {
        strongThis->InitializeInternal();
    });
}

void WmQuitMonitor::InitializeInternal()
{
    winrt::init_apartment();

    // Create a hidden "main" window to listen for WM_CLOSE messages sent to this process by the OS
    // to tell us to shutdown. The window will post a WM_QUIT to exit our message loop.
    // WM_CLOSE will happen if the OS wants to uninstall or update the app.
    auto windowOptions{ WindowCreationOptions::HiddenWindow() };
    windowOptions.QuitOnDestroy = true;
    m_window = std::make_unique<Window>(m_hInstance, windowOptions);

    // Start the message loop thread
    MessageLoopThread();
}

void WmQuitMonitor::Stop() noexcept try
{
    // Releasing the window will cause a DestroyWindow which will cause
    // a PostQuitMessage, which sends a WM_QUIT to our message loop.
    // That flow will unravel everything and ultimately call the m_onWmQuitCallback
    m_window = nullptr;
    m_thread.detach();
}
CATCH_LOG();

void WmQuitMonitor::MessageLoopThread()
{
    MSG msg{};
    BOOL gotMsg{ FALSE };

    PeekMessage(&msg, nullptr, 0U, 0U, PM_NOREMOVE);

    while (WM_QUIT != msg.message)
    {
        gotMsg = PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE);

        if (gotMsg)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            WaitMessage();
        }
    }

    m_onWmQuitCallback();
}

}