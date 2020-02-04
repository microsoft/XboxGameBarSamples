#include "pch.h"
#include "Extension1.h"
#include "Extension1.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::UIExtSample::implementation
{
    Extension1::Extension1()
    {
        InitializeComponent();
    }

    int32_t Extension1::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Extension1::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Extension1::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
