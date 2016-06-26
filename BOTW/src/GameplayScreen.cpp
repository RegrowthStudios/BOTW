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
    
}

void GameplayScreen::destroy(const vui::GameTime& gameTime) {

}

void GameplayScreen::onEntry(const vui::GameTime& gameTime) {

}

void GameplayScreen::onExit(const vui::GameTime& gameTime) {
    
}

void GameplayScreen::registerRendering(vg::Renderer& renderer) {

    m_game->getRenderer().setBackgroundColor(f32v4(0.8f, 0.8f, 1.0f, 1.0f));

    m_scene.init(&m_game->getWindow());
    m_scene.initCamera();
    renderer.registerScene(&m_scene);
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    
}
