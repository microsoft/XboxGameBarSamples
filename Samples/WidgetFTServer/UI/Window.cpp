#include "pch.h"
#include "Window.h"

#include <sstream>

namespace XboxGameBarFT::UI
{
LRESULT CALLBACK GlobalWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    using namespace XboxGameBarFT::UI;

    // Forward calls back into the Window class
    Window* window;
    if (message == WM_NCCREATE)
    {
        window = reinterpret_cast<Window*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
    }
    else
    {
        window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (window)
    {
        return window->WindowProc(hWnd, message, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

std::wstring GetUniqueClassId()
{
    static int s_count = 0;
    s_count++;

    std::wstringstream classNameBuilder;
    classNameBuilder << ".class." << s_count;

    return classNameBuilder.str();
}

WindowClass::WindowClass(HINSTANCE instance, std::wstring_view name) : m_hinstance(instance), m_name(name)
{
    WNDCLASSEX wcex {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = (WNDPROC)XboxGameBarFT::UI::GlobalWindowProc;
    wcex.lpszClassName = m_name.data();
    wcex.hInstance = instance;

    THROW_LAST_ERROR_IF(0 == RegisterClassExW(&wcex));
}

WindowClass::~WindowClass()
{
    LOG_IF_WIN32_BOOL_FALSE_MSG(UnregisterClassW(m_name.c_str(), m_hinstance), "Failed to unregister window class.");
}

HINSTANCE WindowClass::Instance() const noexcept
{
    return m_hinstance;
}
std::wstring_view WindowClass::Name() const noexcept
{
    return m_name;
}

/*static*/ WindowCreationOptions WindowCreationOptions::HiddenWindow()
{
    WindowCreationOptions options {};
    options.X = 0;
    options.Y = 0;
    options.Width = 0;
    options.Height = 0;
    options.Style = WS_POPUP;
    return options;
}

Window::Window(HINSTANCE hinstance, WindowCreationOptions const& options)
    : Window(std::make_shared<WindowClass>(hinstance, GetUniqueClassId()), options)
{
}

Window::Window(std::shared_ptr<WindowClass> windowClass, WindowCreationOptions const& options)
    : m_windowClass(windowClass),
      m_name(options.Name),
      m_quitOnDestroy(options.QuitOnDestroy),
      m_messageDelegate(options.HandleMessage ? options.HandleMessage : DefWindowProc)
{
    m_hwnd = CreateWindowEx(
        options.StyleEx,
        m_windowClass->Name().data(),
        options.Name.data(),
        options.Style,
        options.X,
        options.Y,
        options.Width,
        options.Height,
        /*hwndParent*/ nullptr,
        /*hMenu*/ nullptr,
        m_windowClass->Instance(),
        /*lpParam*/ this);
    THROW_LAST_ERROR_IF_NULL(m_hwnd);
}

Window::~Window()
{
    if (m_hwnd != nullptr)
    {
        LOG_IF_WIN32_BOOL_FALSE_MSG(DestroyWindow(m_hwnd), "Failed to destroy window. m_hwnd(%I64u)", reinterpret_cast<uint64_t>(m_hwnd));
    }
    m_windowClass = nullptr;
}

HWND Window::Handle() const noexcept
{
    return m_hwnd;
}

LRESULT Window::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_NCDESTROY)
    {
        // DestroyWindow was called by someone (possibly ourselves). Release any handles we have to the window or related resources.
        m_hwnd = nullptr;

        if (m_quitOnDestroy)
        {
            // Send WM_QUIT to break out of thread's message loop
            PostQuitMessage(0);
        }
    }

    if (m_messageDelegate)
    {
        return m_messageDelegate(hwnd, message, wParam, lParam);
    }
    else
    {
        return 0;
    }
}

}