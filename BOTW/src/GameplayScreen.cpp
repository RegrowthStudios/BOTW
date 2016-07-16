#include "stdafx.h"
#include "GameplayScreen.h"

#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/graphics/SpriteFont.h>
#include <Vorb/graphics/Camera.h>
#include <Vorb/graphics/GBuffer.h>
#include "App.h"

GameplayScreen::GameplayScreen() : m_idRecycler(10) {
    // Empty
}

GameplayScreen::~GameplayScreen() {
    // Empty
}

i32 GameplayScreen::getNextScreen() const {
    return 0;
}

i32 GameplayScreen::getPreviousScreen() const {
    return 0;
}

void GameplayScreen::build() {
    // Empty
}

void GameplayScreen::destroy(const vui::GameTime& gameTime) {
    // Empty
}

void GameplayScreen::onEntry(const vui::GameTime& gameTime) {
    { // Set up test block stuff
        Block blockA;
        blockA.SID = "Air";
        blockA.color = color4(0, 0, 0, 0);
        m_blockPack.append(blockA);

        Block blockB;
        blockB.SID = "Bright Block";
        blockB.color = color4(145, 255, 255, 255);
        m_blockPack.append(blockB);

        Block blockC;
        blockC.SID = "Blue Block";
        blockC.color = color4(0, 0, 255, 255);
        m_blockPack.append(blockC);
    }

    // Set up red test chunk
    // Only use block data. Don't need tertiary.
    m_testChunk.blockData.setArrayRecycler(&m_idRecycler);
    m_testChunk.blockData.init(vvox::VoxelStorageState::FLAT_ARRAY, m_blockPack["Bright Block"].ID);

    // Sprinkle in some air blocks
    m_testChunk.blockData.set(4, 0);
    m_testChunk.blockData.set(100, 0);
    m_testChunk.blockData.set(3000, 0);

    { // Set up inputs
        // TODO(Ben): Inputmapper
    }
}

void GameplayScreen::onExit(const vui::GameTime& gameTime) {
    // Don't render these anymore.
    m_scene.unregister();
    m_scene.dispose();
    m_bloom.unregister();
    m_bloom.dispose();
}

void GameplayScreen::registerRendering(vg::Renderer& renderer) {

    renderer.setBackgroundColor(f32v4(0.0f, 0.0f, 0.0f, 1.0f));

    // TODO(Ben): Put in GameplayScreenRenderer.

    // Get window handle
    vui::GameWindow& window = m_game->getWindow();

    { // Scene
        m_scene.init(&window, this);
        // Set up the camera
        m_scene.initCamera(window.getAspectRatio());
        m_scene.getCamera()->setOrientation(f32v3(1.0, 0.0, 0.0), f32v3(0.0, 1.0, 0.0));

        renderer.registerScene(&m_scene);
    }

    { // Post processes
        static vg::GLRenderTarget renderTarget(window.getWidth(), window.getHeight()); // TODO(Ben): Move to renderer
        if (!renderTarget.getID()) renderTarget.init(vg::TextureInternalFormat::RGBA);

        m_bloom.init(window.getWidth(), window.getHeight(), renderer.getGBuffer()->getGeometryTexture((ui32)vg::GBUFFER_TEXTURE_UNITS::COLOR));
        m_bloom.setOutputFBO(renderTarget.getID());
        m_bloom.setParams(20u, 150.0f);
        renderer.registerPostProcess(&m_bloom);
        
        m_debugPassthrough.init(renderTarget.getTextureID());
        renderer.registerPostProcess(&m_debugPassthrough);

        // TODO(Ben): Live swapped post processes.

        // GBuffer > Bound textures 0-2
        // A post process will never write to an FBO from a previous post process unless sharing, i.e. 0
        //    Always restore 0 at end of stage.
        // Can have separate render FBOs.
        // The next stage automatically binds up FBO connection with the current stage.
    }
}

void GameplayScreen::onRenderFrame(const vui::GameTime& gameTime) {
    // Empty
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    // Empty
}
