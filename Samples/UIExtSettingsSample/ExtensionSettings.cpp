#include "pch.h"
#include "ExtensionSettings.h"
#include "ExtensionSettings.g.cpp"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::UIExtSettingsSample::implementation
{
    ExtensionSettings::ExtensionSettings()
    {
        InitializeComponent();
    }

    void ExtensionSettings::MyButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
