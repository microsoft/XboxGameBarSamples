#include "pch.h"
#include "Widget.h"
#include "Widget.g.cpp"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace winrt::WidgetSettingsSample::implementation
{
    Widget::Widget()
    {
        InitializeComponent();
    }

    void Widget::OnNavigatedTo(NavigationEventArgs e)
    {
        m_widget = e.Parameter().as<XboxGameBarWidget>();
        m_widgetControl = XboxGameBarWidgetControl(m_widget);

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_widget.SettingsClicked({ this, &Widget::SettingsButton_Click });
    }

    void Widget::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }

    Windows::Foundation::IAsyncAction Widget::SettingsButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strong_this{ get_strong() };
        co_await m_widget.ActivateSettingsAsync();        

        //Comment out code below to  demonstrate how to activate settings with a Uri string
        //
        //hstring appExtID = L"WidgetSettings"; // ID of Settings Widget 
        //hstring appID = L"App";
        //hstring uriSubPath = L"[uriSubPath]";
        //hstring uriQuery = L"[?uriQuery]";
        //hstring uriFragment = L"[#uriFragment]";

        //Uri uri = m_widgetControl.CreateActivationUri(appID, appExtID, uriSubPath, uriQuery, uriFragment);

        //co_await m_widget.ActivateSettingsWithUriAsync(uri);
    }
}
