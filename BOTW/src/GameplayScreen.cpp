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
    m_spriteBatch = std::make_unique<vg::SpriteBatch>();
    m_spriteFont  = std::make_unique<vg::SpriteFont>();
}

void GameplayScreen::onExit(const vui::GameTime& gameTime) {
    
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    
}

void GameplayScreen::draw(const vui::GameTime& gameTime) {
    
}
