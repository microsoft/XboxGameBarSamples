#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

#include "winrt\Windows.System.h"
#include <shellapi.h>

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace winrt::WidgetSample::implementation
{
    Widget1::Widget1()
    {
        InitializeComponent();
    }

    int32_t Widget1::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Widget1::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Widget1::OnNavigatedTo(NavigationEventArgs e)
    {
        m_widget1 = e.Parameter().as<XboxGameBarWidget>();
    }

    winrt::fire_and_forget Widget1::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        //bool result = co_await winrt::Windows::System::Launcher::LaunchUriAsync(winrt::Windows::Foundation::Uri(L"spotify://"));
        bool result = co_await m_widget1.LaunchUriAsync(winrt::Windows::Foundation::Uri(L"spotify://"));
        myButton().Content(box_value(result ? L"true" : L"false"));
    }
}
