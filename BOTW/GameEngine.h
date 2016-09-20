//
// GameEngine.h
// Blood on the Walls
//
// Created by Cristian Zaloj on 1 Jul 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary: 
// Handles loading, initializing, and disposing parts of vital parts of the game.
//

#pragma once

#ifndef GameEngine_h__
#define GameEngine_h__

#include <Vorb/decorators.h>

class GameContent;
class SessionLoadInfo;

/*!
 * \class GameEngine
 *
 * \brief Module responsible for initialization and closure of a game session. Should not be
 *  invoked during the the actual session.
 */
class GameEngine {
public:
    /*!
     * \brief Loads content from the session info, keeping a reference to the session info.
     *
     * \param [out] content Pointer to a null pointer (will not be checked).
     * \param loadInfo Information describing what to load.
     */
    static void loadContent(OUT GameContent** content, const SessionLoadInfo& loadInfo);
    /*!
     * \brief Disposes and destroys/nullifies the data that content points to. After this, it is safe
     *  to destroy the session loading info.
     *
     * \param [in,out] content The loaded content, which is expected to not be null.
     */
    static void disposeContent(OUT GameContent** content);
};

#endif // !GameEngine_h__
