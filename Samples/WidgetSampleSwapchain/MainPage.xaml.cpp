//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace GameBarWVPerfTest;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();

	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(mainSwapChainPanel);

	m_cubeRenderer = std::make_unique<CubeRenderer>(m_deviceResources);

	CompositionTarget::Rendering += ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &GameBarWVPerfTest::MainPage::OnRendering);

	m_deviceResources2 = std::make_shared<DX::DeviceResources>();
	m_deviceResources2->SetSwapChainPanel(liftedSwapChainPanel);
}


void GameBarWVPerfTest::MainPage::MainSwapChainPanelScaleChanged(Windows::UI::Xaml::Controls::SwapChainPanel^ sender, Platform::Object^ args)
{
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_deviceResources2->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	RenderLiftedSwapChainPanel(); // update this swapchain only when the size or scale changes
}


void GameBarWVPerfTest::MainPage::MainSwapChainPanelSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_deviceResources2->SetLogicalSize(e->NewSize);
	RenderLiftedSwapChainPanel(); // update this swapchain only when the size or scale changes
}


void GameBarWVPerfTest::MainPage::OnRendering(Platform::Object^ sender, Platform::Object^ args)
{
	m_cubeRenderer->Render();
	m_deviceResources->Present();
}

void GameBarWVPerfTest::MainPage::RenderLiftedSwapChainPanel()
{
	// Just fill liftedSwapChainPanel with a mostly-transparent color
	const auto context = m_deviceResources2->GetD3DDeviceContext();
	ID3D11RenderTargetView* const targets[1] = { m_deviceResources2->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources2->GetDepthStencilView());
	float color[4] = { 0.0f, 0.2f, 0.0f, 0.2f };
	context->ClearRenderTargetView(m_deviceResources2->GetBackBufferRenderTargetView(), color);
	context->ClearDepthStencilView(m_deviceResources2->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_deviceResources2->Present();
}