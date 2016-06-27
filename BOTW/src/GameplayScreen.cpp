#include "stdafx.h"
#include "GameplayScreen.h"

#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/graphics/SpriteFont.h>

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
    // Empty
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

    // Scene
    m_scene.init(&m_game->getWindow());
    m_scene.initCamera();
    renderer.registerScene(&m_scene);

    // Post processes
    m_bloom.init(m_game->getWindow().getWidth(), m_game->getWindow().getHeight());
    m_bloom.setParams(20u, 150.0f);
    renderer.registerPostProcesses(&m_bloom);
}

void GameplayScreen::onRenderFrame(const vui::GameTime& gameTime) {
    // Empty
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    // Empty
}
