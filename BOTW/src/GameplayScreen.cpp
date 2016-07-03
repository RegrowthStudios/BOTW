#include "stdafx.h"
#include "GameplayScreen.h"

#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/graphics/SpriteFont.h>
#include <Vorb/graphics/Camera.h>

GameplayScreen::GameplayScreen() {
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
        blockB.SID = "Red Block";
        blockB.color = color4(255, 0, 0, 255);
        m_blockPack.append(blockB);

        Block blockC;
        blockC.SID = "Blue Block";
        blockC.color = color4(0, 0, 255, 255);
        m_blockPack.append(blockC);
    }

    // Set up red test chunk
    // Only use block data. Don't need tertiary.
    m_testChunk.blockData.init(vvox::VoxelStorageState::INTERVAL_TREE, m_blockPack["Red Block"].ID);
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

    // Get window handle
    vui::GameWindow& window = m_game->getWindow();

    { // Scene
        m_scene.init(&window);
        // Set up the camera
        m_scene.initCamera(window.getAspectRatio());
        m_scene.getCamera()->setOrientation(f32v3(1.0, 0.0, 0.0), f32v3(0.0, 1.0, 0.0));

        renderer.registerScene(&m_scene);
    }

    { // Post processes
        m_bloom.init(window.getWidth(), window.getHeight());
        m_bloom.setParams(20u, 150.0f);
        renderer.registerPostProcess(&m_bloom);
    }
}

void GameplayScreen::onRenderFrame(const vui::GameTime& gameTime) {
    // Empty
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    // Empty
}
