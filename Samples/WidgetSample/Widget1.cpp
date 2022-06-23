#include "pch.h"
#include "Widget1.h"
#include "Widget1.g.cpp"

#include "winrt\Windows.System.h"
#include <shellapi.h>

using namespace winrt;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;widget

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

    void Widget1::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
