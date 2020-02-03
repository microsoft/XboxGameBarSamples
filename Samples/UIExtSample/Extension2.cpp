#include "pch.h"
#include "Extension2.h"
#include "Extension2.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::UIExtSample::implementation
{
    Extension2::Extension2()
    {
        InitializeComponent();
    }

    int32_t Extension2::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Extension2::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Extension2::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
