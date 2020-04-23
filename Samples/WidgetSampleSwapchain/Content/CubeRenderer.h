#pragma once

#include "..\Common\DeviceResources.h"

class CubeRenderer
{
public:
	CubeRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
	~CubeRenderer() {}

	void Render();

private:
	// Cached pointer to device resources.
	std::shared_ptr<DX::DeviceResources>            m_deviceResources;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>            m_constantBuffer;

	bool                                            m_loadingComplete = false;
};
