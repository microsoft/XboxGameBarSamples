#pragma once

#include "Window.h"

namespace XboxGameBarFT::UI
{

class WmQuitMonitor :
    public std::enable_shared_from_this<WmQuitMonitor>
{
public:
    static std::shared_ptr<WmQuitMonitor> Create(HINSTANCE hInstance, std::function<void()> onWmQuitCallback);

    WmQuitMonitor::WmQuitMonitor(HINSTANCE hInstance, std::function<void()> onWmQuitCallback) :
        m_onWmQuitCallback(onWmQuitCallback),
        m_hInstance(hInstance) {}

    void Stop() noexcept;

private:

    void Initialize();
    void InitializeInternal();
    void MessageLoopThread();

private:
    std::function<void()> m_onWmQuitCallback{ nullptr };
    std::unique_ptr<Window> m_window{ nullptr };
    std::thread m_thread;
    HINSTANCE m_hInstance;
};

}