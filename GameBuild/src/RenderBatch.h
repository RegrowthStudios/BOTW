#pragma once

class ShaderManager;

class RenderBatch {
public:
    void dispose();

    void setShaders(const ShaderManager& shaderManager);
    
    void beginNewBatch();

    void endBatch();

    void renderBatch();
private:
    ID3D11VertexShader* m_pVS = nullptr;
    ID3D11PixelShader*  m_pPS = nullptr;
};
