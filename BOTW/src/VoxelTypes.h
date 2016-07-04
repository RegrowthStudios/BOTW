//
// VoxelTypes.h
// Blood on the Walls
//
// Created by Benjamin Arnold on 4 Jul 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary:
// Types for voxels in BOTW
//

#pragma once

#ifndef VoxelTypes_h__
#define VoxelTypes_h__

#include <Vorb/voxel/BlockPack.h>
#include <Vorb/voxel/Chunk.h>

typedef ui16 BlockID;
typedef ui16 BlockIndex;
typedef ui8 BlockTertiary;

const ui32 CHUNK_WIDTH = 32;

// Custom block implementation
class Block : public vvox::Block<BlockID> {
public:
    color4 color;
};

// Voxel Container Types
typedef vvox::Chunk<BlockID, BlockTertiary, CHUNK_WIDTH> Chunk;
typedef vvox::BlockPack<Block, BlockIndex, BlockID> BlockPack;

#endif // VoxelTypes_h__
