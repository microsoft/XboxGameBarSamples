#include "pch.h"
#include "Extension.h"
#include "Extension.g.cpp"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace winrt::Windows::UI::Xaml::Navigation;
using namespace Microsoft::Gaming::XboxGameBar;

namespace winrt::UIExtSettingsSampleCPP::implementation
{
    Extension::Extension()
    {
        InitializeComponent();
    }

    void Extension::OnNavigatedTo(NavigationEventArgs e)
    {
        m_uiExtension = e.Parameter().as<XboxGameBarUIExtension>();
        m_extensionControl = XboxGameBarExtensionControl(m_uiExtension);

        // Hook up event that's fired when our settings button is clicked
        m_settingsToken = m_uiExtension.SettingsClicked({ this, &Extension::SettingsButton_Click });
    }

    Windows::Foundation::IAsyncAction Extension::SettingsButton_Click(
        winrt::Windows::Foundation::IInspectable const& sender,
        winrt::Windows::Foundation::IInspectable const& e)
    {
        auto strong_this{ get_strong() };
        co_await m_uiExtension.ActivateSettingsAsync();
        

		//Comment out code below to  demonstrate how to activate settings with a Uri string
		//
		//hstring appExtID = L"ExtensionSettings"; //ID of Settings Extension 
		//hstring appID = L"App";
		//hstring uriSubPath = L"[uriSubPath]";
		//hstring uriQuery = L"[?uriQuery]";
		//hstring uriFragment = L"[#uriFragment]";

		//Uri uri = m_extensionControl.CreateActivationUri(appID, appExtID, uriSubPath, uriQuery, uriFragment);

		//co_await m_uiExtension.ActivateSettingsWithUriAsync(uri);
    }
}
