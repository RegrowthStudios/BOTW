#include "stdafx.h"
#include "App.h"

#include "GameplayScreen.h"

void App::onInit() {
    // Empty
}

void App::addScreens() {

    // Add the gameplay screen to our list of screens in this game
    m_gameplayScreen = new GameplayScreen();
    m_screenList.addScreen(m_gameplayScreen);

    m_screenList.setScreen(m_gameplayScreen->getIndex());
}

void App::onExit() {
    delete m_gameplayScreen;
    // TODO(Ben): Use proper game time
    vui::GameTime tmpTime;
    m_screenList.destroy(tmpTime);
}
