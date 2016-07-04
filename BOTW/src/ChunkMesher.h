//
// ChunkMesher.h
// Blood on the Walls
//
// Created by Benjamin Arnold on 4 Jul 2016
// Copyright 2016 Regrowth Studios
// All Rights Reserved
//
// Summary:
// 
//

#pragma once

#ifndef ChunkMesher_h__
#define ChunkMesher_h__

#include <Vorb/voxel/VoxelMesherCulled.h>
#include "VoxelTypes.h"



class ChunkMesher {
public:
    void init(const BlockPack* blockPack);

    void clear();

    void result(const vvox::meshalg::VoxelQuad& quad, BlockID id);

    // TODO(Ben): Shared IBO
    void generateIndices();

    vvox::meshalg::VoxelFaces occludes(BlockID a, BlockID b, vvox::Axis axis);

    typedef ui32 MeshIndex;

    struct MeshVertex {
        // Compact position
        union {
            struct {
                ui8 x;
                ui8 y;
                ui8 z;
            };
            UNIONIZE(ui8v3 position);
        };
        ui8 padding1;
        UNIONIZE(color3 color);
        ui8 padding2;
    };

    /************************************************************************/
    /* Mesh Data                                                            */
    /************************************************************************/
    std::vector<MeshVertex> vertices;
    std::vector<MeshIndex>   indices;
private:

    const BlockPack* m_blockPack = nullptr;
};

#endif // ChunkMesher_h__
