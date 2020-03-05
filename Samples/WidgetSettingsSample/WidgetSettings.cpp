#include "pch.h"
#include "WidgetSettings.h"
#include "WidgetSettings.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::WidgetSettingsSample::implementation
{
    WidgetSettings::WidgetSettings()
    {
        InitializeComponent();
    }

    void WidgetSettings::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
