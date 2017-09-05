#include "stdafx.h"
#include "ShaderManager.h"

ShaderManager::ShaderManager() :
    m_shaderStores{} {
    // Empty
}

bool ShaderManager::loadCompiledShaderFile(ID3D11Device* pDevice, const std::string& name, ShaderType t, const char* path) {
    // Result of a shader
    CachedShader shd;

    FILE* f = fopen(path, "rb");
    
    // Get the size of the file
    fseek(f, 0, SEEK_END);
    shd.shaderBytecode.length = ftell(f);
    fseek(f, 0, SEEK_SET);
    shd.shaderBytecode.length = shd.shaderBytecode.length - ftell(f);

    // Read into a buffer
    shd.shaderBytecode.pData = new ui8[shd.shaderBytecode.length];

    {
        ui8* pDst = reinterpret_cast<ui8*>(shd.shaderBytecode.pData);
        size_t amountLeft = shd.shaderBytecode.length;
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
            if (SUCCEEDED(res = pDevice->CreateVertexShader(shd.shaderBytecode.pData, shd.shaderBytecode.length, nullptr, &shd.pShader.vertex))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, shd));
            }
            break;
        }
        case ShaderManager::ShaderType::GEOMETRY:
        {
            if (SUCCEEDED(res = pDevice->CreateGeometryShader(shd.shaderBytecode.pData, shd.shaderBytecode.length, nullptr, &shd.pShader.geometry))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, shd));
            }
            break;
        }
        case ShaderManager::ShaderType::PIXEL:
        {
            if (SUCCEEDED(res = pDevice->CreatePixelShader(shd.shaderBytecode.pData, shd.shaderBytecode.length, nullptr, &shd.pShader.pixel))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, shd));
            }
            break;
        }
        case ShaderManager::ShaderType::COMPUTE:
        {
            if (SUCCEEDED(res = pDevice->CreateComputeShader(shd.shaderBytecode.pData, shd.shaderBytecode.length, nullptr, &shd.pShader.compute))) {
                m_shaderStores[(size_t)t].insert(std::make_pair(name, shd));
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

const ShaderManager::CachedShader& ShaderManager::getVertexShader(const std::string& name) const {
    return m_shaderStores[(size_t)ShaderType::VERTEX].at(name);
}

const ShaderManager::CachedShader& ShaderManager::getGeometryShader(const std::string& name) const {
    return m_shaderStores[(size_t)ShaderType::GEOMETRY].at(name);
}

const ShaderManager::CachedShader& ShaderManager::getPixelShader(const std::string& name) const {
    return m_shaderStores[(size_t)ShaderType::PIXEL].at(name);
}

const ShaderManager::CachedShader& ShaderManager::getComputeShader(const std::string& name) const {
    return m_shaderStores[(size_t)ShaderType::COMPUTE].at(name);
}

