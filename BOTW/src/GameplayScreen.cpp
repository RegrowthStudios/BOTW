#include "stdafx.h"
#include "GameplayScreen.h"

#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/graphics/SpriteFont.h>
#include <Vorb/graphics/ImageIO.h>

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
    m_form.init("Matt's Mad Form", this, &m_game->getWindow(), f32v4(0.0f, 0.0f, m_game->getWindow().getWidth() / 5, m_game->getWindow().getHeight()));
    m_form.setClipping(false);

    // Button 1
    vui::Button* button1 = new vui::Button(&m_form, "Cray Button 1");
    button1->setRawPosition({ 0.0f, 0.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button1->setRawDimensions({ 400.0f, 0.1f, { vui::UnitType::PIXEL, vui::UnitType::PERCENTAGE } });

    /*vg::ImageIO imageIO;
    vg::ScopedBitmapResource res = vg::ImageIO().load("test.jpg", vg::ImageIOFormat::RGB_UI8);
    VGTexture tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 300, 0, GL_RGB, GL_RGB8UI, res.bytesUI8);
    button1->setTexture(tex);*/

    button1->setBackColor(color4(1.0f, 0.0f, 0.0f));

    // Button 2
    vui::Button* button2 = new vui::Button(&m_form, "Cray Button 2");
    button2->setRawPosition({ 80.0f, 80.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button2->setRawDimensions({ 120.0f, 120.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button2->setBackColor(color4(0.0f, 0.0f, 1.0f));
    button2->setClipping(false);

    //Button 3
    vui::Button* button3 = new vui::Button(button2, "Cray Button 3");
    button3->setRawPosition({ 100.0f, 100.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button3->setRawDimensions({ 120.0f, 120.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button3->setBackColor(color4(0.0f, 1.0f, 0.0f));
    button3->setPositionType(vui::PositionType::RELATIVE);
    button3->setClipping(false);
    //button3->setClipping(vui::ClippingOptions(true, false, false, false));

    // Button 4
    vui::Button* button4 = new vui::Button(button3, "Cray Button 4");
    button4->setRawPosition({ 20.0f, 20.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button4->setRawDimensions({ 160.0f, 160.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button4->setBackColor(color4(0.0f, 1.0f, 1.0f));
    button4->setClipping(false);
    //button4->setClipping(vui::ClippingOptions(false, true, false, false));

    // Button 5
    vui::Button* button5 = new vui::Button(button4, "Cray Button 5");
    button5->setRawPosition({ -20.0f, -20.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button5->setRawDimensions({ 1.0f, 1.0f, { vui::UnitType::PERCENTAGE, vui::UnitType::PERCENTAGE } });
    button5->setBackColor(color4(1.0f, 1.0f, 0.0f));
    button5->setPositionType(vui::PositionType::ABSOLUTE);
    button5->setClipping(false);

    // Button 6
    vui::Button* button6 = new vui::Button(&m_form, "Cray Button 6");
    button6->setRawPosition({ 380.0f, 380.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button6->setRawDimensions({ 20.0f, 20.0f, { vui::UnitType::PIXEL, vui::UnitType::PIXEL } });
    button6->setBackColor(color4(1.0f, 0.0f, 1.0f));
    //button6->setClipping(false);



    /*vui::Button* buttons[100];
    for (size_t i = 0; i < 100; ++i) {
        size_t remainder = i % 10;
        size_t tens = (i - remainder) / 10;

        buttons[i] = new vui::Button(&m_form, "");
        buttons[i]->setBackColor(color4(1.0f / (remainder + 1), 1.0f / (tens + 1), 0.0f));
        buttons[i]->setRawPosition(std::pair<f32v2, vui::UnitType>(f32v2(remainder * 0.1f, tens * 0.1f), vui::UnitType::PERCENTAGE));
        buttons[i]->setRawDimensions(std::pair<f32v2, vui::UnitType>(f32v2(0.1f), vui::UnitType::PERCENTAGE));
        f32v2 position = buttons[i]->getPosition();
        printf("Position %i: %f - %f\n", i, position.x, position.y);
        f32v2 dimensions = buttons[i]->getDimensions();
        printf("Dimensions %i: %f - %f\n", i, dimensions.x, dimensions.y);
        m_form.addWidget(buttons[i]);
    }*/

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
