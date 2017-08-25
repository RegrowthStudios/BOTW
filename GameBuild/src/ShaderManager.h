#pragma once

class ShaderManager {
public:
    enum class ShaderType {
        VERTEX,
        GEOMETRY,
        PIXEL,
        COMPUTE,
        COUNT
    };

    static constexpr size_t SHADER_TYPE_COUNT = (size_t)ShaderType::COUNT;

    ShaderManager();

    bool loadCompiledShaderFile(ID3D11Device* pDevice, const std::string& name, ShaderType t, const char* path);

    void dispose();

    ID3D11VertexShader* getVertexShader(const std::string& name) const;
    ID3D11GeometryShader* getGeometryShader(const std::string& name) const;
    ID3D11PixelShader* getPixelShader(const std::string& name) const;
    ID3D11ComputeShader* getComputeShader(const std::string& name) const;
private:
    std::unordered_map<std::string, void*> m_shaderStores[SHADER_TYPE_COUNT];
};
