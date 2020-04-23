//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

#include "Common\DeviceResources.h"
#include "Content\CubeRenderer.h"

namespace GameBarWVPerfTest
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void MainSwapChainPanelScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Platform::Object^ args);
		void MainSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void OnRendering(Platform::Object^ sender, Platform::Object^ args);

		void RenderLiftedSwapChainPanel();

		// For the main swapchain (playing the role of WebView2 for this test app)
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<CubeRenderer> m_cubeRenderer;

		// For the overlay swapchain (playing the role of the Lifted swapchain for WinUI)
		std::shared_ptr<DX::DeviceResources> m_deviceResources2;
	};
}
