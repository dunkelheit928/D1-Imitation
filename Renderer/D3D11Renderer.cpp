#include "pch.h"
#include "D3D11Renderer.h"

D3D11Renderer::~D3D11Renderer()
{
	Clear();
}

bool D3D11Renderer::Init(void* hwnd, int width, int height)
{
	m_hwnd = (HWND)hwnd;
	m_width = width;
	m_height = height;

	// Create device and swapchain.
	DXGI_SWAP_CHAIN_DESC scd = {};
	scd.BufferCount = 1;
	scd.BufferDesc.Width = m_width;
	scd.BufferDesc.Height = m_height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = m_hwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL created = D3D_FEATURE_LEVEL_11_0;
	HR_CHECK(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &scd, &m_swapchain, &m_device, &created, &m_context));

	// Create back buffer.
	ComPtr<ID3D11Texture2D> backBuffer;
	HR_CHECK(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));

	// Create render target view.
	HR_CHECK(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView));

	// Set rasterizer state.
	// - 스프라이트 랜더링을 위해 Cull 모드 'D3D11_CULL_NONE' 으로 설정
	ComPtr<ID3D11RasterizerState> rsNoCull;
	D3D11_RASTERIZER_DESC rd = {};
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.FrontCounterClockwise = FALSE;
	rd.DepthClipEnable = TRUE;
	HR_CHECK(m_device->CreateRasterizerState(&rd, &rsNoCull));
	m_context->RSSetState(rsNoCull.Get());

	// Set render target view.
	m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	// Setup viewport.
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)m_width;
	vp.Height = (FLOAT)m_height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	m_context->RSSetViewports(1, &vp);

	// Create sampler.
	D3D11_SAMPLER_DESC sd{};
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.AddressU = sd.AddressV = sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	HR_CHECK(m_device->CreateSamplerState(&sd, &m_sampler));

	// Compile shader.
	ComPtr<ID3DBlob> vsBlob, psBlob, errBlob;
	HR_CHECK(D3DCompileFromFile(L"../Shaders/DefaultVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &vsBlob, &errBlob));
	HR_CHECK(D3DCompileFromFile(L"../Shaders/DefaultPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSMain", "ps_5_0", 0, 0, &psBlob, &errBlob));
	HR_CHECK(m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &m_vertexShader));
	HR_CHECK(m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &m_pixelShader));

	// Create inputLayout.
	D3D11_INPUT_ELEMENT_DESC desc[] =
	{
		// Per vertex
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		// Per instance
		{"INSTPOS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTUV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
		{"INSTCOLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},

	};
	HR_CHECK(m_device->CreateInputLayout(desc, _countof(desc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_inputLayout));

	// Create vertex buffer and index buffer.
	Vertex vertices[4] =
	{
		{0.0f,0.0f,0.0f,0.0f,0.0f},
		{1.0f,0.0f,0.0f,1.0f,0.0f},
		{1.0f,1.0f,0.0f,1.0f,1.0f},
		{0.0f,1.0f,0.0f,0.0f,1.0f} 
	};
	D3D11_BUFFER_DESC bd = {}; 
	bd.Usage = D3D11_USAGE_DEFAULT; 
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; 

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;
	HR_CHECK(m_device->CreateBuffer(&bd, &initData, &m_vertexBuffer));

	UINT indices[6] = { 0,1,2, 0,2,3 };
	bd.Usage = D3D11_USAGE_DEFAULT; 
	bd.ByteWidth = sizeof(indices); 
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER; 

	initData.pSysMem = indices;
	HR_CHECK(m_device->CreateBuffer(&bd, &initData, &m_indexBuffer));

	// Create instance buffer.
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = (UINT)(sizeof(InstanceData) * MAX_INSTANCE_COUNT);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	m_instanceBuffer.Reset();
	HR_CHECK(m_device->CreateBuffer(&bd, nullptr, &m_instanceBuffer));

	// Create constant buffer.
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(XMMATRIX);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	HR_CHECK(m_device->CreateBuffer(&bd, nullptr, &m_constBuffer))

	return true;
}

void D3D11Renderer::Clear()
{
	m_textures.clear();
	m_drawList.clear();
}

uint32 D3D11Renderer::UploadTextureRGBA(const void* data, int width, int height)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = data;
	initData.SysMemPitch = (UINT)(width * 4);
	initData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Texture2D> tex;
	HR_CHECK(m_device->CreateTexture2D(&desc, &initData, &tex));

	ComPtr<ID3D11ShaderResourceView> srv;
	HR_CHECK(m_device->CreateShaderResourceView(tex.Get(), nullptr, &srv));

	TextureRecord record = {};
	record.id = m_lastTextureId++;
	record.srv = srv;
	record.width = width;
	record.height = height;
	m_textures[record.id] = record;

	return record.id;
}

void D3D11Renderer::UnloadTexture(uint32 textureId)
{
	m_textures.erase(textureId);
}

void D3D11Renderer::RenderFrame()
{
	XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0.0f, (float)m_width, (float)m_height, 0.0f, 0.0f, 1.0f);
	XMFLOAT4X4 m;
	XMStoreFloat4x4(&m, XMMatrixTranspose(proj));
	m_context->UpdateSubresource(m_constBuffer.Get(), 0, nullptr, &m, 0, 0);

	// Clear render target.
	float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

	// Draw call.
	DrawSprite_Implementation();

	m_drawList.clear();
	m_swapchain->Present(1, 0);
}

void D3D11Renderer::DrawSprite(uint32 textureId, float posX, float posY, float width, float height, float u0, float v0, float u1, float v1, uint32 layer)
{
	Sprite s = {};
	s.textureId = textureId;
	s.instData.pos.x = posX;
	s.instData.pos.y = posY;
	s.instData.size.x = width;
	s.instData.size.y = height;
	s.instData.uv0.x = u0;
	s.instData.uv0.y = v0;
	s.instData.uv1.x = u1;
	s.instData.uv1.y = v1;
	m_drawList.push_back(s);
}

void D3D11Renderer::DrawSprite_Implementation()
{
	m_context->IASetInputLayout(m_inputLayout.Get());
	m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	m_context->PSSetSamplers(0, 1, m_sampler.GetAddressOf());
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->VSSetConstantBuffers(0, 1, m_constBuffer.GetAddressOf());
	m_context->PSSetConstantBuffers(0, 1, m_constBuffer.GetAddressOf());

	// Batch by texture handle
	std::unordered_map<uint32_t, std::vector<InstanceData>> batches;
	for (const auto& s : m_drawList) 
	{
		batches[s.textureId].push_back(s.instData);
	}

	for (const auto& batch : batches)
	{
		uint32 textureId = batch.first;
		const auto& instData = batch.second;
		if (instData.empty())
		{
			continue;
		}

		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr =  (m_context->Map(m_instanceBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped));
		if (FAILED(hr)) continue;
		memcpy(mapped.pData, instData.data(), instData.size() * sizeof(InstanceData));
		m_context->Unmap(m_instanceBuffer.Get(), 0);

		ID3D11Buffer* vertexBuffers[] =
		{
			m_vertexBuffer.Get(),
			m_instanceBuffer.Get()
		};

		UINT strides[] =
		{
			sizeof(Vertex),
			sizeof(InstanceData)
		};

		UINT offsets[] = { 0, 0 };
		m_context->IASetVertexBuffers(0, 2, vertexBuffers, strides, offsets);
		m_context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// Bind texture SRV
		auto iter = m_textures.find(textureId);
		ID3D11ShaderResourceView* srv = nullptr;
		if (iter != m_textures.end())
		{
			srv = iter->second.srv.Get();
		}
		m_context->PSSetShaderResources(0, 1, &srv);

		// DrawIndexedInstanced (6 indices per quad)
		m_context->DrawIndexedInstanced(6, (UINT)instData.size(), 0, 0, 0);
	}
}
