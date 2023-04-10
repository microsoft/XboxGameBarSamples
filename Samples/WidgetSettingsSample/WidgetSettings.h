#pragma once

#include <windows.h>
#include <unknwn.h>
#include <restrictederrorinfo.h>
#include <hstring.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.ApplicationModel.Activation.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Controls.Primitives.h>
#include <winrt/Windows.UI.Xaml.Data.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Windows.UI.Xaml.Markup.h>
#include <winrt/Windows.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.Gaming.XboxGameBar.h>

#pragma comment(lib, "windowsapp")

#include "WidgetSettings.g.h"

namespace winrt::WidgetSettingsSample::implementation
{
    struct WidgetSettings : WidgetSettingsT<WidgetSettings>
    {
        WidgetSettings()
        {
            // Expensive initialization code here
            // ...

            // Create UI elements
            Grid grid;

            StackPanel stackPanel;
            stackPanel.HorizontalAlignment(winrt::Windows::UI::Xaml::HorizontalAlignment::Center);
            stackPanel.VerticalAlignment(winrt::Windows::UI::Xaml::VerticalAlignment::Center);
            stackPanel.Orientation(winrt::Windows::UI::Xaml::Controls::Orientation::Vertical);

            TextBlock textBlock;
            textBlock.Padding(winrt::Windows::UI::Xaml::Thickness(0, 0, 0, 5));
            textBlock.Text(L"WidgetSettings");

            Button button;
            button.Name(L"myButton");
            button.Content(winrt::box_value(L"Click Me"));

            stackPanel.Children().Append(textBlock);
            stackPanel.Children().Append(button);

            grid.Children().Append(stackPanel);

            this->Content(grid);
        }

        void MyButton_Click(Windows::Foundation::IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args)
        {
            // Expensive button click event handling code here
            // ...
        }
    };
}

namespace winrt::WidgetSettingsSample::factory_implementation
{
    struct WidgetSettings : WidgetSettingsT<WidgetSettings, implementation::WidgetSettings>
    {
    };
}
