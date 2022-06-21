#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt\Windows.System.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::WidgetSample::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    winrt::fire_and_forget MainPage::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        co_return;
    }
}
