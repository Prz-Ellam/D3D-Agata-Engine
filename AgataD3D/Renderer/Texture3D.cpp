#include "pch.h"
#include "Texture3D.h"
#include "Renderer.h"
#include <stb/stb_image.h>

namespace Agata {

	ID3D11SamplerState* Texture3D::s_SamplerState = nullptr;
	bool Texture3D::s_SamplerCreated = false;

    Texture3D::Texture3D(const SkyboxTexture& paths) : m_SRV(nullptr) {

        uint8_t* buffers[6];
        std::string textures[6] = { paths.Right, paths.Left, paths.Bottom, paths.Top, paths.Front, paths.Back };

        for (int i = 0; i < 6; i++) {
            buffers[i] = stbi_load(textures[i].c_str(), &m_Width, &m_Height, &m_BPP, 4);
        }

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 6;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        D3D11_SUBRESOURCE_DATA subresourceData[6] = { 0 };
        for (int i = 0; i < 6; i++) {
            subresourceData[i].pSysMem = buffers[i];
            subresourceData[i].SysMemPitch = m_Width * 4;
            subresourceData[i].SysMemSlicePitch = 0;
        }

        HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&textureDesc, subresourceData, &m_ID);

        if (FAILED(hr)) {
            OutputDebugString("No se pudo crear el Texture2D\n");
            return;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        hr = Renderer::GetDevice()->CreateShaderResourceView(m_ID, &srvDesc, &m_SRV);

        if (FAILED(hr)) {
            OutputDebugString("No se pudo crear el Shader Resource View\n");
            return;
        }

        for (int i = 0; i < 6; i++) {
            if (buffers[i]) {
                stbi_image_free(buffers[i]);
                buffers[i] = nullptr;
            }
        }

        if (!s_SamplerCreated) {

            D3D11_SAMPLER_DESC samplerDesc = {};
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

            hr = Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &s_SamplerState);

            if (FAILED(hr)) {
                OutputDebugString("No se pudo crear el Sampler State\n");
                return;
            }
            Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &s_SamplerState);
            s_SamplerCreated = true;

        }

    }

    Texture3D::~Texture3D() {

        if (m_ID) {
            m_ID->Release();
        }

        if (m_SRV) {
            m_SRV->Release();
        }

    }

    void Texture3D::Bind(uint32_t slot) const {

        Renderer::GetDeviceContext()->PSSetShaderResources(slot, 1u, &m_SRV);
        Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &s_SamplerState);

    }

}