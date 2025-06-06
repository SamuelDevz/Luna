#include "Renderer.h"
#include <d3dcompiler.h>

namespace Luna
{
    Renderer::Renderer() noexcept 
        : graphics{nullptr},
        inputLayout{nullptr},
        vertexShader{nullptr},
        pixelShader{nullptr},
        rasterState{nullptr},
        vertexBuffer{nullptr}
	{
	}

	Renderer::~Renderer() noexcept
	{
        SafeRelease(inputLayout);
        SafeRelease(vertexShader);
        SafeRelease(pixelShader);
        SafeRelease(rasterState);
        SafeRelease(vertexBuffer);
	}

	void Renderer::Initialize(Graphics * graphics, const void * vertices, const uint32 points)
	{
		this->graphics = graphics;

        //-------------------------------
        // Vertex Shader
        //-------------------------------

        ID3DBlob* vShader = nullptr;
        ThrowIfFailed(D3DReadFileToBlob(L"Shaders/Pixel.cso", &vShader))
        ThrowIfFailed(graphics->Device()->CreateVertexShader(
            vShader->GetBufferPointer(),
            vShader->GetBufferSize(),
            nullptr,
            &vertexShader))

        //-------------------------------
        // Pixel Shader
        //-------------------------------

        ID3DBlob* pShader = nullptr;
        ThrowIfFailed(D3DReadFileToBlob(L"Shaders/Pixel.cso", &vShader))
        ThrowIfFailed(graphics->Device()->CreatePixelShader(
            pShader->GetBufferPointer(),
            pShader->GetBufferSize(),
            nullptr,
            &pixelShader))

        pShader->Release();

        //-------------------------------
        // Input Layout
        //-------------------------------

        D3D11_INPUT_ELEMENT_DESC layoutDesc[]
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        ThrowIfFailed(graphics->Device()->CreateInputLayout(
            layoutDesc, 
            Countof(layoutDesc),
            vShader->GetBufferPointer(), 
            vShader->GetBufferSize(), 
            &inputLayout))

        vShader->Release();

        //-------------------------------
        // Rasterizador
        //-------------------------------

        D3D11_RASTERIZER_DESC rasterDesc{};
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        //rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
        rasterDesc.CullMode = D3D11_CULL_BACK;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.DepthBias = D3D11_DEFAULT_DEPTH_BIAS;
        rasterDesc.DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP;
        rasterDesc.SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.AntialiasedLineEnable = false;

        ThrowIfFailed(graphics->Device()->CreateRasterizerState(&rasterDesc, &rasterState))

        //-------------------------------
        // Vertex Buffer
        //-------------------------------

        D3D11_BUFFER_DESC vertexBufferDesc{};
        vertexBufferDesc.ByteWidth = sizeof(Vertex) * points;
        vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA vertexData{};
        vertexData.pSysMem = vertices;
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        ThrowIfFailed(graphics->Device()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer))

        Draw();

        SafeRelease(vShader);
        SafeRelease(pShader);
	}

    void Renderer::Draw()
    {
        uint32 vertexStride = sizeof(Vertex);
        uint32 vertexOffset{};
        graphics->Context()->IASetInputLayout(inputLayout);
        graphics->Context()->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &vertexOffset);
        graphics->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        graphics->Context()->VSSetShader(vertexShader, nullptr, 0);
        graphics->Context()->PSSetShader(pixelShader, nullptr, 0);
        graphics->Context()->RSSetState(rasterState);
    }
}