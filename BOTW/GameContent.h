//
// GameContent.h
// Blood on the Walls
//
// Created by Cristian Zaloj on 1 Jul 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary: 
// Structure that holds all data that is preloaded and permanent in memory for a game.
//

#pragma once

#ifndef GameContent_h__
#define GameContent_h__

class SessionLoadInfo;

/*!
 * \struct GameContent
 *
 * \brief This is game content that is found on both the CPU and the GPU. It will only be loaded
 *  once and disposed at the very end of the game session.
 */
struct GameContent {
public:
    /*!
     * \brief Constructor required for passing in the reference.
     *
     * \param info Reference to the information used for loading.
     */
    GameContent(const SessionLoadInfo& info) :
        loadInfo(info) {
        // Empty
    }

    /*! \brief Reference to loading information for the content, should remain const. */
    const SessionLoadInfo& loadInfo;

    // TODO(Everyone): Fill out as time goes on.
};

#endif // !GameContent_h__
