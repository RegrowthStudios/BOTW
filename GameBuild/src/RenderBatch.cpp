#include "stdafx.h"
#include "RenderBatch.h"

#include "ShaderManager.h"

void RenderBatch::dispose() {
    Utils::comRelease(m_pVS);
    Utils::comRelease(m_pPS);
}

void RenderBatch::setShaders(const ShaderManager& shaderManager) {
    m_pVS = shaderManager.getVertexShader("RenderBatch");
    m_pVS->AddRef();
    m_pPS = shaderManager.getPixelShader("RenderBatch");
    m_pPS->AddRef();
}

void RenderBatch::beginNewBatch() {

}

void RenderBatch::endBatch() {

}

void RenderBatch::renderBatch() {

}
