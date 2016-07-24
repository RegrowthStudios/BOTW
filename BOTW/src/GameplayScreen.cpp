#include "stdafx.h"
#include "GameplayScreen.h"

#include <Vorb/graphics/SpriteBatch.h>
#include <Vorb/graphics/SpriteFont.h>
#include <Vorb/graphics/Camera.h>
#include <Vorb/graphics/GBuffer.h>
#include <Vorb/ui/InputDispatcher.h>
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

    // Set up empty test chunk
    // Only use block data. Don't need tertiary.
    m_testChunk.blockData.setArrayRecycler(&m_idRecycler);
    m_testChunk.blockData.init(vvox::VoxelStorageState::FLAT_ARRAY, m_blockPack["Bright Block"].ID);

    // Sprinkle in some blocks
    m_testChunk.blockData.set(m_testChunk.width * m_testChunk.width + 4, 0);
   

    { // Set up inputs
        m_inputMapper.get(m_inputMapper.createInput("Escape", VKEY_ESCAPE)).downEvent.addFunctor(
            [&](Sender s, ui32 a) -> void {
            exit(1);
        });

#define ROTATE_SPEED 0.05f

        m_inputMapper.get(m_inputMapper.createInput("Left", VKEY_A)).downEvent.addFunctor(
            [&](Sender s, ui32 a) -> void {
            m_scene.getCamera()->rotate(-ROTATE_SPEED, 0.0f);
        });

        m_inputMapper.get(m_inputMapper.createInput("Right", VKEY_D)).downEvent.addFunctor(
            [&](Sender s, ui32 a) -> void {
            m_scene.getCamera()->rotate(ROTATE_SPEED, 0.0f);
        });

        m_inputMapper.get(m_inputMapper.createInput("Front", VKEY_W)).downEvent.addFunctor(
            [&](Sender s, ui32 a) -> void {
            m_scene.getCamera()->rotate(0.0f, ROTATE_SPEED);
        });

        m_inputMapper.get(m_inputMapper.createInput("Back", VKEY_S)).downEvent.addFunctor(
            [&](Sender s, ui32 a) -> void {
            m_scene.getCamera()->rotate(0.0f, -ROTATE_SPEED);
        });

        m_inputMapper.startInput();
    }
}

void GameplayScreen::onExit(const vui::GameTime& gameTime) {
    // Don't render these anymore.
    m_scene.unregister();
    m_scene.dispose();
    m_bloom.unregister();
    m_bloom.dispose();
    m_inputMapper.stopInput();
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
        
        m_ssao.init(window.getWidth(), window.getHeight(),
                    renderer.getGBuffer()->getGeometryTexture((ui32)vg::GBUFFER_TEXTURE_UNITS::COLOR),
                    renderer.getGBuffer()->getDepthTexture(),
                    renderer.getGBuffer()->getGeometryTexture((ui32)vg::GBUFFER_TEXTURE_UNITS::NORMAL),
                    m_scene.getCamera());

      //  renderer.registerPostProcess(&m_ssao);

        // Visualize the normal buffer.
        m_debugPassthrough.init(renderer.getGBuffer()->getGeometryTexture((ui32)vg::GBUFFER_TEXTURE_UNITS::NORMAL));
        renderer.registerPostProcess(&m_debugPassthrough);

        // TODO(Ben): Live swapped post processes.
    }
}

void GameplayScreen::onRenderFrame(const vui::GameTime& gameTime) {
    // Empty
}

void GameplayScreen::update(const vui::GameTime& gameTime) {
    // Empty
}
