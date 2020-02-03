#include "pch.h"
#include "Extension2.h"
#include "Extension2.g.cpp"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;

namespace winrt::UIExtAdvSample::implementation
{
    Extension2::Extension2()
    {
        InitializeComponent();
    }

    void Extension2::OnNavigatedTo(NavigationEventArgs e)
    {
        auto uri = e.Parameter().as<Uri>();
        uriTextBlock().Text(uri.AbsoluteUri());
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
