//
// GameplayScreen.h
// Blood on the Walls
//
// Created by Benjamin Arnold on 20 Jun 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary:
// Screen that handles the main game experience.
//

#pragma once

#ifndef GameplayScreen_h__
#define GameplayScreen_h__

#include <Vorb/ui/IGameScreen.h>
#include <Vorb/VorbPreDecl.inl>
#include <Vorb/graphics/PostProcess.h>

#include "GameplayScene.h"
#include "VoxelTypes.h"

DECL_VG(class SpriteBatch)
DECL_VG(class SpriteFont)


class GameplayScreen : public vui::IGameScreen {
public:
    friend class GameplayScene;

    GameplayScreen();
    ~GameplayScreen();
    
    virtual i32 getNextScreen() const override;

    virtual i32 getPreviousScreen() const override;

    virtual void build() override;

    virtual void destroy(const vui::GameTime& gameTime) override;

    virtual void onEntry(const vui::GameTime& gameTime) override;

    virtual void onExit(const vui::GameTime& gameTime) override;

    virtual void registerRendering(vg::Renderer& renderer) override;

    virtual void onRenderFrame(const vui::GameTime& gameTime) override;

    virtual void update(const vui::GameTime& gameTime) override;

private:
    Chunk m_testChunk; ///< Temporary
    vcore::FixedSizeArrayRecycler<CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_WIDTH, BlockID> m_idRecycler;
    BlockPack m_blockPack; ///< All blocks

    GameplayScene m_scene; ///< Rendering
    vg::PostProcessBloom m_bloom; ///< Bloom post process
};

#endif // GameplayScreen_h__