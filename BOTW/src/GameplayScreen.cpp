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
    /*m_bloom.unregister();
    m_bloom.dispose();*/
}

void GameplayScreen::registerRendering(vg::Renderer& renderer) {

    renderer.setBackgroundColor(f32v4(0.0f, 0.0f, 0.0f, 1.0f));

    // Scene
    m_scene.init(&m_game->getWindow());
    m_scene.initCamera();
    renderer.registerScene(&m_scene);

    // Form
    m_form.init("Matt's Mad Form", this, f32v4(0.0f, 0.0f, m_game->getWindow().getWidth(), m_game->getWindow().getHeight()));

    // Button 1
    vui::Button* button1 = new vui::Button(&m_form, "Cray Button", f32v4(10.0f, 10.0f, 390.0f, 390.0f));
    button1->setBackColor(color4(1.0f, 0.0f, 0.0f));
    button1->setText("Hello");
    button1->enable();
    button1->setDock(vorb::ui::DockStyle::LEFT);
    m_form.addWidget(button1);


    // Button 2
    vui::Button* button2 = new vui::Button(&m_form, "Cray Button", f32v4(0.0f, 0.0f, 390.0f, 390.0f));
    button2->setBackColor(color4(0.0f, 1.0f, 0.0f));
    button2->setText("Hello");
    button2->enable();
    button2->setDock(vorb::ui::DockStyle::LEFT);
    m_form.addWidget(button2);

    // Enable form.
    m_form.enable();

    // Post processes
    /*m_bloom.init(m_game->getWindow().getWidth(), m_game->getWindow().getHeight());
    m_bloom.setParams(20u, 150.0f);
    renderer.registerPostProcess(&m_bloom);*/
}

void GameplayScreen::onRenderFrame(const vui::GameTime& gameTime) {
    // Render form.
    m_form.draw();
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    // Update form.
    m_form.update();
}
