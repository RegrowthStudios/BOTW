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
}
void App::onExit() {
    delete m_gameplayScreen;
}
