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

#include <vorb/graphics/Scene.h>
#include <vorb/graphics/gtypes.h>
#include <vorb/VorbPreDecl.inl>

DECL_VUI(class GameWindow);
DECL_VG(class SpriteBatch);

class GameplayScene : public vg::IScene {
public:
    GameplayScene();
    ~GameplayScene();

    void init(const vui::GameWindow* window);

    virtual void load() override;

    virtual void render(const vui::GameTime& gameTime) override;

private:
    const vui::GameWindow* m_window;
    std::unique_ptr<vg::SpriteBatch> m_spriteBatch = nullptr;
};

#endif // GameplayScene_h__
