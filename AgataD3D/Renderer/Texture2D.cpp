#include "pch.h"
#include "Texture2D.h"
#include "Renderer.h"
#include <stb/stb_image.h>

namespace Agata {

    ID3D11SamplerState* Texture2D::s_SamplerState = nullptr;
    bool Texture2D::s_SamplerCreated = false;

    Texture2D::Texture2D(const std::string& filePath) : m_SRV(nullptr) {

        uint8_t* buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        CreateTextureFromBuffer(buffer);
        CreateSampler();

	}

    Texture2D::Texture2D(const std::string& filePath, DefaultTexture defaultTex) : m_SRV(nullptr) {

        stbi_set_flip_vertically_on_load(1);
        uint8_t* buffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        if (!buffer) {
            CreateTextureFromDefault(defaultTex);
        }
        else {
            CreateTextureFromBuffer(buffer);
        }

        CreateSampler();

    }

    Texture2D::~Texture2D() {

        if (m_ID) {
            m_ID->Release();
        }

        if (m_SRV) {
            m_SRV->Release();
        }

    }

    void Texture2D::Bind(uint32_t slot) const {

        Renderer::GetDeviceContext()->PSSetShaderResources(slot, 1u, &m_SRV);
        Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &s_SamplerState);

    }

    bool Texture2D::CreateTextureFromBuffer(uint8_t* buffer) {

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 0;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

        HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &m_ID);

        if (FAILED(hr)) {
            OutputDebugString("No se pudo crear el Texture2D\n");
            return false;
        }

        Renderer::GetDeviceContext()->UpdateSubresource(m_ID, 0, nullptr, buffer, m_Width * 4, 0);


        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = -1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        hr = Renderer::GetDevice()->CreateShaderResourceView(m_ID, &srvDesc, &m_SRV);

        if (FAILED(hr)) {
            OutputDebugString("No se pudo crear el Shader Resource View\n");
            return false;
        }

        Renderer::GetDeviceContext()->GenerateMips(m_SRV);

        if (buffer) {
            stbi_image_free(buffer);
            buffer = nullptr;
        }

        return true;

    }

    bool Texture2D::CreateTextureFromDefault(DefaultTexture defaultTex) {

        uint8_t* buffer = new uint8_t[4];

        switch (defaultTex) {

            case DefaultTexture::DiffuseMap: {
                memset(buffer, 255, 4);
                break;
            }
            case DefaultTexture::NormalMap: {
                memset(buffer, 128, 2);
                memset(buffer + 2, 255, 2);
                break;
            }
            case DefaultTexture::SpecularMap: {
                memset(buffer, 255, 4);
                break;
            }
            case DefaultTexture::DuDvMap: {
                memset(buffer, 0, 3);
                memset(buffer + 3, 255, 1);
                break;
            }
            default: {
                break;
            }

        }

        m_Width = m_Height = 1;
        m_BPP = 4;

        D3D11_TEXTURE2D_DESC textureDesc = { 0 };
        textureDesc.Width = m_Width;
        textureDesc.Height = m_Height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.SampleDesc.Quality = 0;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subresourceData = { 0 };
        subresourceData.pSysMem = buffer;
        subresourceData.SysMemPitch = m_Width * 4;
        subresourceData.SysMemSlicePitch = 0;

        HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&textureDesc, &subresourceData, &m_ID);

        if (FAILED(hr)) {
            OutputDebugString("No se pudo crear el Texture2D\n");
            delete[] buffer;
            buffer = nullptr;
            return false;
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;
        srvDesc.Texture2D.MostDetailedMip = 0;

        hr = Renderer::GetDevice()->CreateShaderResourceView(m_ID, &srvDesc, &m_SRV);

        if (FAILED(hr)) {
            OutputDebugString("No se pudo crear el Shader Resource View\n");
            delete[] buffer;
            buffer = nullptr;
            return false;
        }

        if (buffer) {
            delete[] buffer;
            buffer = nullptr;
        }

        return true;

    }

    bool Texture2D::CreateSampler() {

        if (!s_SamplerCreated) {

            D3D11_SAMPLER_DESC samplerDesc = {};
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
            samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

            HRESULT hr = Renderer::GetDevice()->CreateSamplerState(&samplerDesc, &s_SamplerState);

            if (FAILED(hr)) {
                OutputDebugString("No se pudo crear el Sampler State\n");
                return false;
            }
            Renderer::GetDeviceContext()->PSSetSamplers(0u, 1u, &s_SamplerState);
            s_SamplerCreated = true;

        }

        return true;

    }

}