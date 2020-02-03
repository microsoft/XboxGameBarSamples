#include "pch.h"
#include "Extension1.h"
#include "Extension1.g.cpp"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace winrt::UIExtAdvSample::implementation
{
    Extension1::Extension1()
    {
        InitializeComponent();
    }

    void Extension1::OnNavigatedTo(NavigationEventArgs e)
    {
        m_uiExtension = e.Parameter().as<XboxGameBarUIExtension>();
        m_extensionControl = XboxGameBarExtensionControl(m_uiExtension);
    }

    IAsyncAction Extension1::ActivateAsyncAppExtIdButton_Click(IInspectable const& /*sender*/, RoutedEventArgs const& /*e*/)
    {
        hstring text = this->ActivateAsyncAppExtId().Text();
        return m_extensionControl.ActivateAsync(text);
    }


    IAsyncAction Extension1::ActivateAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.ActivateAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }


    IAsyncAction Extension1::ActivateWithUriAsyncButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        Uri uri{ this->ActivateAsyncUri().Text() };
        co_return co_await m_extensionControl.ActivateWithUriAsync(uri);
    }


    IAsyncAction Extension1::MinimizeAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.MinimizeAsync(this->ActivateAsyncAppExtId().Text());
    }


    IAsyncAction Extension1::MinimizeAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.MinimizeAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }


    IAsyncAction Extension1::MaximizeAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.MaximizeAsync(this->ActivateAsyncAppExtId().Text());
    }


    IAsyncAction Extension1::MaximizeAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.MaximizeAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }


    IAsyncAction Extension1::CloseAsyncAppExtIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.CloseAsync(this->ActivateAsyncAppExtId().Text());
    }


    IAsyncAction Extension1::CloseAsyncAppIdButton_Click(IInspectable const& sender, RoutedEventArgs const& e)
    {
        co_return co_await m_extensionControl.CloseAsync(
            this->ActivateAsyncAppId().Text(),
            this->ActivateAsyncAppExtId().Text());
    }
}


