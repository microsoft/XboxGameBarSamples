#include "pch.h"
#include "Widget2.h"
#include "Widget2.g.cpp"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;

namespace winrt::WidgetAdvSample::implementation
{
    std::wstring Widget2::m_suspendResumeState{};

    Widget2::Widget2()
    {
        InitializeComponent();
    }

    void Widget2::OnNavigatedTo(NavigationEventArgs e)
    {
        if (m_suspendResumeState.empty())
        {
            auto uri = e.Parameter().as<Uri>();
            uriTextBlock().Text(uri.AbsoluteUri());
        }
        else
        {
            uriTextBlock().Text(m_suspendResumeState);
        }
    }

    int32_t Widget2::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void Widget2::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void Widget2::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
