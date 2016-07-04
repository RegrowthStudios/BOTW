#include "stdafx.h"
#include "GameplayScene.h"
#include "GameplayScreen.h"
#include "ChunkMesher.h"
#include "ShaderLoader.h"

#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/ui/GameWindow.h>
#include <Vorb/ui/MainGame.h>
#include <Vorb/graphics/ShaderCommon.inl>
#include <Vorb/graphics/Camera.h>

namespace {
    // Block shading
    const cString VERT_SRC = R"(
// Uniforms
uniform mat4 unWVP;

// Input
in vec3 vPosition;
in vec3 vColor;

// Output
out vec4 fColor;

void main() {
  // Convert voxel coordinates
  vec3 vertexPosition = vPosition / 7.0;

  gl_Position = unWVP * vec4(vertexPosition, 1.0);

  fColor = vec4(vColor, 1.0);
})";
}

GameplayScene::GameplayScene() {
    // Empty
}

GameplayScene::~GameplayScene() {
    // Empty
}

void GameplayScene::init(const vui::GameWindow* window, const GameplayScreen* screen) {
    m_window = window;
    m_screen = screen;
}

void GameplayScene::load() {
    m_spriteBatch = std::make_unique<vg::SpriteBatch>();
    m_spriteBatch->init();

    // Load shader
    m_program = ShaderLoader::createProgram("Voxel Test", VERT_SRC, vg::shadercommon::COLOR_FRAG_SRC);

    { // Set up chunk mesh
        const Chunk& chunk = m_screen->m_testChunk;

        ChunkMesher mesher;
        mesher.init(&m_screen->m_blockPack);

        vvox::meshalg::createCulled<BlockID, ChunkMesher>(chunk.blockData.getDataArray(), ui32v3(CHUNK_WIDTH), &mesher);
        mesher.generateIndices();

        // TODO(Ben): Move to vorb
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ibo);

        m_indexCount = mesher.indices.size();

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, mesher.vertices.size() * sizeof(ChunkMesher::MeshVertex), mesher.vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesher.indices.size() * sizeof(ChunkMesher::MeshIndex), mesher.indices.data(), GL_STATIC_DRAW);

        m_program.enableVertexAttribArrays();
        glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(ChunkMesher::MeshVertex), offsetptr(ChunkMesher::MeshVertex, position));
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ChunkMesher::MeshVertex), offsetptr(ChunkMesher::MeshVertex, color));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void GameplayScene::render(const vui::GameTime& gameTime) {

    m_camera->update();

    m_spriteBatch->begin();

    // Debug quad
    m_spriteBatch->draw(0, f32v2(300.0f, 300.0f), f32v2(50.0f), color4(255, 255, 255, 255));
    m_spriteBatch->draw(0, f32v2(150.0f, 150.0f), f32v2(50.0f), color4(80, 80, 255, 255));

    m_spriteBatch->end();
    m_spriteBatch->render(f32v2(m_window->getViewportDims()));

    m_program.use();

    f32m4 wvp = m_camera->getViewProjectionMatrix();

    glUniformMatrix4fv(m_program.getUniform("unWVP"), 1, GL_FALSE, &wvp[0][0]);

    glDisable(GL_CULL_FACE);
    
    // Render chunk
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    m_program.unuse();

    printf("%lf\n", gameTime.elapsed);
}

void GameplayScene::dispose() {
    m_spriteBatch.reset();
    IScene::dispose();
}