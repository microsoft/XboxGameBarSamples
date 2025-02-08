#pragma once

namespace XboxGameBarFT::UI
{
/// <summary>
/// Manages a Window Class used to create windows
/// </summary>
struct WindowClass
{
public:
    WindowClass(HINSTANCE instance, std::wstring_view name);
    ~WindowClass();

    HINSTANCE Instance() const noexcept;
    std::wstring_view Name() const noexcept;

private:
    HINSTANCE m_hinstance;
    std::wstring m_name;
};

struct WindowCreationOptions
{
public:
    std::wstring Name;
    int X {CW_USEDEFAULT};
    int Y {CW_USEDEFAULT};
    int Width {CW_USEDEFAULT};
    int Height {CW_USEDEFAULT};
    int ClassStyle {0};
    int Style {0};
    int StyleEx {0};

    /// <summary>
    /// Sends WM_QUIT to the thread message loop if the HWND is destroyed.
    /// Destruction may be triggered by default WM_CLOSE handling or by destroying the Window instance.
    /// </summary>
    bool QuitOnDestroy {false};

    // A message handler. Must call DefWindowProc for messages it does not explicitly process.
    std::function<LRESULT(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)> HandleMessage;

    static WindowCreationOptions HiddenWindow();
};

/// <summary>
/// Manages a Window handle and associated message handler.
/// The thread this object is created on must run a message pump.
/// </summary>
struct Window
{
public:
    Window(HINSTANCE instance, WindowCreationOptions const& options);
    Window(std::shared_ptr<WindowClass> windowClass, WindowCreationOptions const& options);
    ~Window();

    Window(Window const&) = delete;
    Window& operator=(Window const&) = delete;

    HWND Handle() const noexcept;

private:
    std::shared_ptr<WindowClass> m_windowClass;
    HWND m_hwnd {nullptr};
    std::wstring m_name;
    bool m_quitOnDestroy;
    std::function<LRESULT(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)> m_messageDelegate;

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    friend LRESULT CALLBACK GlobalWindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

}