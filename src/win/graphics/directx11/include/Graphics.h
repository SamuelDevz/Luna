#pragma once

#include <dxgi1_6.h>
#include <d3d11_4.h>
#include "DebugLayer.h"
#include "Window.h"
#include "Types.h"

namespace Luna
{
    class Graphics
    {
    private:
        // config
        uint32                        refreshRate;
        uint32                        backBufferCount;
        uint32                        antialiasing;
        uint32                        quality;
        bool                          vSync;
        float                         bgColor[4];

        // pipeline
        ID3D11Device5               * device;
        ID3D11DeviceContext4        * context;
        IDXGIFactory7               * factory;
        IDXGISwapChain1             * swapChain;
        ID3D11RenderTargetView      * renderTargetView;
        ID3D11DepthStencilState     * depthStencilState;
        ID3D11DepthStencilView      * depthStencilView;
        ID3D11Texture2D             * depthStencil;
        ID3D11BlendState            * blendState;
        D3D_FEATURE_LEVEL             featureLevel;
        D3D11_VIEWPORT                viewport;
        D3D11_RECT                    scissorRect;
        
        DebugLayer * debugLayer;

        void LogHardwareInfo() noexcept;
        void RefreshRate() noexcept;

    public:
        explicit Graphics() noexcept;
        ~Graphics() noexcept;

        void VSync(const bool state) noexcept;
        void Clear() noexcept;
        void Present() noexcept;
        void Initialize(const Window * const window);

        ID3D11Device5* Device() const noexcept;
        ID3D11DeviceContext4* Context() const noexcept;
        uint32 Antialiasing() const noexcept;
        uint32 Quality() const noexcept;
    };

    inline void Graphics::VSync(const bool state) noexcept
    { vSync = state; }

    inline void Graphics::Clear() noexcept
    { 
        context->ClearRenderTargetView(renderTargetView, bgColor);
        context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    inline void Graphics::Present() noexcept
    {
        swapChain->Present(vSync, 0);
        context->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
    }

    inline ID3D11Device5* Graphics::Device() const noexcept
    { return device; }

    inline ID3D11DeviceContext4* Graphics::Context() const noexcept
    { return context; }

    inline uint32 Graphics::Antialiasing() const noexcept
    { return antialiasing; }

    inline uint32 Graphics::Quality() const noexcept
    { return quality; }
};