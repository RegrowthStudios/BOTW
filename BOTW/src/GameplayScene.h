//
// GameplayScene.h
// Blood on the Walls
//
// Created by Benjamin Arnold on 25 Jun 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary:
// 
//

#pragma once

#ifndef GameplayScene_h__
#define GameplayScene_h__

#include <Vorb/VorbPreDecl.inl>
#include <Vorb/graphics/GLProgram.h>
#include <Vorb/graphics/Scene.h>
#include <Vorb/graphics/gtypes.h>

DECL_VUI(class GameWindow);
DECL_VG(class SpriteBatch);
class GameplayScreen;

class GameplayScene : public vg::IScene {
public:
    GameplayScene();
    ~GameplayScene();

    void init(const vui::GameWindow* window, const GameplayScreen* screen);

    virtual void load() override;

    virtual void render(const vui::GameTime& gameTime) override;

    virtual void dispose() override;

private:
    const GameplayScreen* m_screen = nullptr;

    const vui::GameWindow* m_window;
    std::unique_ptr<vg::SpriteBatch> m_spriteBatch = nullptr;

    VGVertexArray m_vao = 0;
    VGBuffer      m_vbo = 0;
    VGBuffer      m_ibo = 0;
    ui32 m_indexCount = 0;

    vg::GLProgram m_program;

};

#endif // GameplayScene_h__
