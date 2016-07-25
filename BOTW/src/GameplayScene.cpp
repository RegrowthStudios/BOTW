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
uniform mat4 unW;

// Input
in vec4 vPosition_Face;
in vec3 vColor;

// Output
out vec3 fPosition;
out vec4 fColor;
out vec3 fNormal;

vec3 NORMAL_LOOKUP[6] = vec3[6](
    vec3(-1.0, 0.0, 0.0),
    vec3( 1.0, 0.0, 0.0),
    vec3( 0.0,-1.0, 0.0),
    vec3( 0.0, 1.0, 0.0),
    vec3( 0.0, 0.0,-1.0),
    vec3( 0.0, 0.0, 1.0)
);

void main() {
  // Convert voxel coordinates
  vec3 vertexPosition = vPosition_Face.xyz / 7.0;

  gl_Position = unWVP * vec4(vertexPosition, 1.0);
   
  fPosition = (unW * vec4(vertexPosition, 1.0)).rgb;
  fColor = vec4(vColor, 1.0);
  fNormal = NORMAL_LOOKUP[int(vPosition_Face.w)];
})";

    // TODO(Ben): Texture
    const cString FRAG_SRC = R"(
// Uniforms
uniform mat4 unWVP;

// Input
in vec3 fPosition;
in vec4 fColor;
in vec3 fNormal;

// Output
layout(location = 0) out vec4 pColor;
layout(location = 1) out vec4 pPosition;
layout(location = 2) out vec4 pNormal;

void main() {
  pPosition = vec4(fPosition, 1.0);
  pColor = fColor;
  pNormal = vec4(fNormal, 1.0);
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
    // Look down diagonally-ish
    m_camera->setOrientation(f32v3(0.8f, 0.0f, 0.2f), f32v3(1.0f, 1.0f, 1.0f));
    m_camera->setFieldOfView(90.0f);

    m_spriteBatch = std::make_unique<vg::SpriteBatch>();
    m_spriteBatch->init();

    // Load shader
    m_program = ShaderLoader::createProgram("Voxel Test", VERT_SRC, FRAG_SRC, vg::ShaderLanguageVersion(3, 3));

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
        // Position and face.
        glVertexAttribPointer(0, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(ChunkMesher::MeshVertex), offsetptr(ChunkMesher::MeshVertex, position));
        // Color
        glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(ChunkMesher::MeshVertex), offsetptr(ChunkMesher::MeshVertex, color));

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

void GameplayScene::render(const vui::GameTime& gameTime) {

    m_camera->update();

    m_program.use();

    f32m4 wvp = m_camera->getViewProjectionMatrix() * vmath::translate(f32v3(14.0f, 0.0f, 4.0f));

    glUniformMatrix4fv(m_program.getUniform("unWVP"), 1, GL_FALSE, &wvp[0][0]);

    glDisable(GL_CULL_FACE);
    
    // Render chunk
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    m_program.unuse();

    m_spriteBatch->begin();

    // Debug quad
    m_spriteBatch->draw(0, f32v2(300.0f, 300.0f), f32v2(50.0f), color4(255, 255, 255, 255));
    m_spriteBatch->draw(0, f32v2(150.0f, 150.0f), f32v2(50.0f), color4(80, 80, 255, 255));

    m_spriteBatch->end();
    m_spriteBatch->render(f32v2(m_window->getViewportDims()));

    printf("%lf\n", gameTime.elapsed);
}

void GameplayScene::dispose() {
    m_spriteBatch.reset();
    IScene::dispose();
}