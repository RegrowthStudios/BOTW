//
// App.h
// Blood on the Walls
//
// Created by Benjamin Arnold on 20 Jun 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary:
// Controller for the game.
//

#pragma once

#ifndef App_h__
#define App_h__

#include <Vorb/ui/MainGame.h>

class GameplayScreen;

class App : public vui::MainGame {
public:

    virtual void onInit()     override;

    virtual void addScreens() override;

    virtual void onExit()     override;

private:
    GameplayScreen * m_gameplayScreen = nullptr;
};

#endif // App_h__
