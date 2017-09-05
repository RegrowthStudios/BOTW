#pragma once

#include "ShaderManager.h"

class RenderBatch {
public:
    struct RenderQuad {
        ID3D11Texture2D* pTexture;
        f32v2 position;
        f32v2 size;
        ui8v4 tint;
    };

    struct RenderVertex {
        f32v3 position;
        f32v2 uv;
        ui8v4 tint;
    };

    struct RenderList {
        ID3D11Texture2D* pTexture;
        ui32 quadOffset;
        ui32 quadCount;
    };

    void dispose();

    void setShaders(const ShaderManager& shaderManager);

    void beginNewBatch(ID3D11Device* pDevice, ID3D11DeviceContext* pDevCtx);

    void draw(ID3D11Texture2D* pTex, f32v2 position,f32v2 size, ui8v4 tint);

    void endBatch();

    void renderBatch();
private:
    ID3D11Device*        m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDevCtx = nullptr;
    const ShaderManager::CachedShader* m_pVS = nullptr;
    const ShaderManager::CachedShader* m_pPS = nullptr;

    std::vector<RenderQuad> m_quads;
    std::vector<RenderList> m_renderLists;

    std::vector<RenderVertex> m_vertices;
    ID3D11Buffer*             m_pVertices = nullptr;
    size_t                    m_quadVertexCapacity = 0;
    std::vector<ui32>         m_sortedQuadIndices;
    ID3D11Buffer*             m_pIndices = nullptr;
    size_t                    m_quadIndexCapacity = 0;
    ID3D11InputLayout*        m_pVertexLayout;

    ID3D11Buffer* m_pShaderConstants = nullptr;
};
