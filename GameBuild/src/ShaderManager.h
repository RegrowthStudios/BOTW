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

    struct CachedShader {
        union {
            ID3D11VertexShader*   vertex;
            ID3D11GeometryShader* geometry;
            ID3D11PixelShader*    pixel;
            ID3D11ComputeShader*  compute;
        } pShader;

        struct {
            void*  pData;
            size_t length;
        } shaderBytecode;
    };

    static constexpr size_t SHADER_TYPE_COUNT = (size_t)ShaderType::COUNT;

    ShaderManager();

    bool loadCompiledShaderFile(ID3D11Device* pDevice, const std::string& name, ShaderType t, const char* path);

    void dispose();

    const CachedShader& getVertexShader(const std::string& name) const;
    const CachedShader& getGeometryShader(const std::string& name) const;
    const CachedShader& getPixelShader(const std::string& name) const;
    const CachedShader& getComputeShader(const std::string& name) const;
private:
    std::unordered_map<std::string, CachedShader> m_shaderStores[SHADER_TYPE_COUNT];
};
