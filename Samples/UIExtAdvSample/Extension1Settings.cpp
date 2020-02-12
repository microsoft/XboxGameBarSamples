#include "pch.h"
#include "Extension1Settings.h"
#if __has_include("Extension1Settings.g.cpp")
#include "Extension1Settings.g.cpp"
#endif

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::UIExtAdvSample::implementation
{
    Extension1Settings::Extension1Settings()
    {
        InitializeComponent();
    }

    void Extension1Settings::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        Button().Content(box_value(L"Clicked"));
    }
}
