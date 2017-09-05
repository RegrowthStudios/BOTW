#include "stdafx.h"
#include "RenderBatch.h"

#include <Vorb/math/MatrixMath.hpp>

#include "ShaderManager.h"

void RenderBatch::dispose() {
}

void RenderBatch::setShaders(const ShaderManager& shaderManager) {
    m_pVS = &shaderManager.getVertexShader("RenderBatch");
    m_pPS = &shaderManager.getPixelShader("RenderBatch");
}

void RenderBatch::beginNewBatch(ID3D11Device* pDevice, ID3D11DeviceContext* pDevCtx) {
    m_pDevice = pDevice;
    m_pDevCtx = pDevCtx;
}

void RenderBatch::draw(ID3D11Texture2D* pTex, f32v2 position, f32v2 size, ui8v4 tint) {
    m_quads.emplace_back(RenderQuad{ pTex, position, size, tint });
}

void RenderBatch::endBatch() {
    m_renderLists.clear();

    RenderList buildList;
    buildList.pTexture = m_quads[0].pTexture;
    buildList.quadCount = 1;
    buildList.quadOffset = 0;

    size_t i = 1;
    while (i < m_quads.size()) {
        RenderQuad& quad = m_quads[i];

        if (buildList.pTexture != quad.pTexture) {
            // Encountering a new texture commits a render list
            m_renderLists.emplace_back(std::move(buildList));
            buildList.pTexture = quad.pTexture;
            buildList.quadCount = 1;
            buildList.quadOffset = (ui32)i;
        }
        else {
            // We get one more quad to render :)
            ++buildList.quadCount;
        }
        ++i;
    }
    m_renderLists.emplace_back(std::move(buildList));

    // With all the render lists created, we should now update the vertex data
    m_vertices.clear();
    for (RenderQuad& quad : m_quads) {
        RenderVertex v;
        v.position = f32v3(quad.position.x, quad.position.y, 0.0f);
        v.tint = quad.tint;
        v.uv = f32v2(0.0f, 0.0f);
        m_vertices.push_back(v);

        v.position = f32v3(quad.position.x + quad.size.x, quad.position.y, 0.0f);
        v.tint = quad.tint;
        v.uv = f32v2(1.0f, 0.0f);
        m_vertices.push_back(v);

        v.position = f32v3(quad.position.x, quad.position.y + quad.size.y, 0.0f);
        v.tint = quad.tint;
        v.uv = f32v2(0.0f, 1.0f);
        m_vertices.push_back(v);

        v.position = f32v3(quad.position.x + quad.size.x, quad.position.y + quad.size.y, 0.0f);
        v.tint = quad.tint;
        v.uv = f32v2(1.0f, 1.0f);
        m_vertices.push_back(v);
    }

    // Make sure we have enough indices
    size_t requiredIndices = m_quads.size() * 6;
    size_t curIndices = m_sortedQuadIndices.size();
    while (m_sortedQuadIndices.size() < requiredIndices) {
        ui32 startIndex = (ui32)(m_sortedQuadIndices.size() / 6) * 4;
        m_sortedQuadIndices.emplace_back(startIndex);
        m_sortedQuadIndices.emplace_back(startIndex + 2);
        m_sortedQuadIndices.emplace_back(startIndex + 1);
        m_sortedQuadIndices.emplace_back(startIndex + 1);
        m_sortedQuadIndices.emplace_back(startIndex + 2);
        m_sortedQuadIndices.emplace_back(startIndex + 3);
    }

    // Ensure buffers are created
    if (m_pVertices == nullptr) {
        m_quadVertexCapacity = 100;
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = (UINT)(sizeof(RenderVertex) * 4 * m_quadVertexCapacity);
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        m_pDevice->CreateBuffer(&desc, nullptr, &m_pVertices);

        D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(RenderVertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, offsetof(RenderVertex, uv),       D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, offsetof(RenderVertex, tint),     D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        m_pDevice->CreateInputLayout(vertexLayout, 3, m_pVS->shaderBytecode.pData, m_pVS->shaderBytecode.length, &m_pVertexLayout);
    }
    if (m_pIndices == nullptr) {
        m_quadIndexCapacity = 100;
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = (UINT)(sizeof(ui32) * 6 * m_quadIndexCapacity);
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        m_pDevice->CreateBuffer(&desc, nullptr, &m_pIndices);
    }
    if (m_pShaderConstants == nullptr) {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.ByteWidth = sizeof(ShaderConstants);
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        m_pDevice->CreateBuffer(&desc, nullptr, &m_pShaderConstants);
    }

    // Update device buffer contents
    D3D11_MAPPED_SUBRESOURCE mappedRes;
    if (SUCCEEDED(m_pDevCtx->Map(m_pVertices, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes))) {
        memcpy(mappedRes.pData, m_vertices.data(), m_vertices.size() * sizeof(RenderVertex));
        m_pDevCtx->Unmap(m_pVertices, 0);
    }

    if (requiredIndices > curIndices) {
        if (SUCCEEDED(m_pDevCtx->Map(m_pIndices, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedRes))) {
            ui32* pDstData = reinterpret_cast<ui32*>(mappedRes.pData) + curIndices;
            memcpy(pDstData, m_sortedQuadIndices.data() + curIndices, (requiredIndices - curIndices) * sizeof(ui32));
            m_pDevCtx->Unmap(m_pIndices, 0);
        }
    }
}

void RenderBatch::renderBatch() {
    m_pDevCtx->VSSetShader(m_pVS->pShader.vertex, nullptr, 0);
    m_pDevCtx->PSSetShader(m_pPS->pShader.pixel, nullptr, 0);

    UINT stride = sizeof(RenderVertex);
    UINT offset = 0;
    m_pDevCtx->IASetVertexBuffers(0, 1, &m_pVertices, &stride, &offset);
    m_pDevCtx->IASetIndexBuffer(m_pIndices, DXGI_FORMAT_R32_UINT, 0);
    m_pDevCtx->IASetInputLayout(m_pVertexLayout);
    m_pDevCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Update the constant buffer
    D3D11_MAPPED_SUBRESOURCE mappedRes;
    if (SUCCEEDED(m_pDevCtx->Map(m_pShaderConstants, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes)))         {
        ShaderConstants constants;
        constants.m_worldViewProjection = vmath::ortho<f32>(0, 1920, 0, 1080);
        memcpy(mappedRes.pData, &constants, sizeof(ShaderConstants));
        m_pDevCtx->Unmap(m_pShaderConstants, 0);
    }
    m_pDevCtx->VSSetConstantBuffers(0, 1, &m_pShaderConstants);

    for (RenderList& l : m_renderLists) {
        m_pDevCtx->DrawIndexed(l.quadCount * 6, l.quadOffset * 6, 0);
    }
}
