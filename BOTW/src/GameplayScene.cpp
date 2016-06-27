#include "stdafx.h"
#include "GameplayScene.h"
#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/ui/GameWindow.h>
#include <Vorb/ui/MainGame.h>

GameplayScene::GameplayScene() {
    // Empty
}

GameplayScene::~GameplayScene() {
    // Empty
}

void GameplayScene::init(const vui::GameWindow* window) {
    m_window = window;
}

void GameplayScene::load() {
    m_spriteBatch = std::make_unique<vg::SpriteBatch>();
    m_spriteBatch->init();
}

void GameplayScene::render(const vui::GameTime& gameTime) {
    m_spriteBatch->begin();

    // Debug quad
    m_spriteBatch->draw(0, f32v2(300.0f, 300.0f), f32v2(200.0f), color4(255, 255, 255, 255));
    m_spriteBatch->draw(0, f32v2(150.0f, 150.0f), f32v2(200.0f), color4(80, 80, 255, 255));

    m_spriteBatch->end();
    m_spriteBatch->render(f32v2(m_window->getViewportDims()));

    printf("%lf\n", gameTime.elapsed);
}

void GameplayScene::dispose() {
    m_spriteBatch.reset();
    IScene::dispose();
}