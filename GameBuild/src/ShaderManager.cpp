#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager() :
    m_shaderStores{} {
    // Empty
}

bool ShaderManager::loadCompiledShaderFile(ID3D11Device* pDevice, const std::string& name, ShaderType t, const char* path) {
    FILE* f = fopen(path, "rb");
    
    // Get the size of the file
    fseek(f, 0, SEEK_END);
    auto length = ftell(f);
    fseek(f, 0, SEEK_SET);
    length = length - ftell(f);

    // Read into a buffer
    void* pBytes = _alloca(length);

    {
        ui8* pDst = reinterpret_cast<ui8*>(pBytes);
        size_t amountLeft = length;
        while (amountLeft != 0) {
            auto amountRead = fread(pDst, 1, amountLeft, f);
            pDst += amountRead;
            amountLeft -= amountRead;
        }
    }
    
    HRESULT res = S_OK;
    switch (t) {
        case ShaderManager::ShaderType::VERTEX:
        {
            ID3D11VertexShader* pShader;
            if (SUCCEEDED(res = pDevice->CreateVertexShader(pBytes, length, nullptr, &pShader))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, pShader));
            }
            break;
        }
        case ShaderManager::ShaderType::GEOMETRY:
        {
            ID3D11GeometryShader* pShader;
            if (SUCCEEDED(res = pDevice->CreateGeometryShader(pBytes, length, nullptr, &pShader))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, pShader));
            }
            break;
        }
        case ShaderManager::ShaderType::PIXEL:
        {
            ID3D11PixelShader* pShader;
            if (SUCCEEDED(res = pDevice->CreatePixelShader(pBytes, length, nullptr, &pShader))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, pShader));
            }
            break;
        }
        case ShaderManager::ShaderType::COMPUTE:
        {
            ID3D11ComputeShader* pShader;
            if (SUCCEEDED(res = pDevice->CreateComputeShader(pBytes, length, nullptr, &pShader))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, pShader));
            }
            break;
        }
        default:
            res = E_INVALIDARG;
            break;
    }

    return SUCCEEDED(res);
}

void ShaderManager::dispose() {
    // TODO(CZ)
}

ID3D11VertexShader* ShaderManager::getVertexShader(const std::string& name) const {
    return (ID3D11VertexShader*)(m_shaderStores[(size_t)ShaderType::VERTEX].at(name));
}

ID3D11GeometryShader* ShaderManager::getGeometryShader(const std::string& name) const {
    return (ID3D11GeometryShader*)(m_shaderStores[(size_t)ShaderType::GEOMETRY].at(name));
}

ID3D11PixelShader* ShaderManager::getPixelShader(const std::string& name) const {
    return (ID3D11PixelShader*)(m_shaderStores[(size_t)ShaderType::PIXEL].at(name));
}

ID3D11ComputeShader* ShaderManager::getComputeShader(const std::string& name) const {
    return (ID3D11ComputeShader*)(m_shaderStores[(size_t)ShaderType::COMPUTE].at(name));
}

