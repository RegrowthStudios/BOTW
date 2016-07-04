#include "stdafx.h"
#include "ChunkMesher.h"

#define NUM_FACES 6
#define C_RES 7

const ui8v3 VOXEL_POSITIONS[NUM_FACES][4] = {
    { ui8v3(0, C_RES, 0), ui8v3(0, 0, 0), ui8v3(0, 0, C_RES), ui8v3(0, C_RES, C_RES) }, // v6-v7-v2-v1 (left)
    { ui8v3(C_RES, C_RES, C_RES), ui8v3(C_RES, 0, C_RES), ui8v3(C_RES, 0, 0), ui8v3(C_RES, C_RES, 0) }, // v0-v3-v4-v5 (right)
    { ui8v3(0, 0, C_RES), ui8v3(0, 0, 0), ui8v3(C_RES, 0, 0), ui8v3(C_RES, 0, C_RES) }, // v2-v7-v4-v3 (bottom)
    { ui8v3(C_RES, C_RES, C_RES), ui8v3(C_RES, C_RES, 0), ui8v3(0, C_RES, 0), ui8v3(0, C_RES, C_RES) }, // v0-v5-v6-v1 (top)
    { ui8v3(C_RES, C_RES, 0), ui8v3(C_RES, 0, 0), ui8v3(0, 0, 0), ui8v3(0, C_RES, 0) }, // v5-v4-v7-v6 (back)
    { ui8v3(0, C_RES, C_RES), ui8v3(0, 0, C_RES), ui8v3(C_RES, 0, C_RES), ui8v3(C_RES, C_RES, C_RES) } // v1-v2-v3-v0 (front)
};

void ChunkMesher::init(const BlockPack* blockPack) {
    m_blockPack = blockPack;
}

void ChunkMesher::clear() {
    vertices.clear();
    indices.clear();
}

void ChunkMesher::result(const vvox::meshalg::VoxelQuad& quad, BlockID id) {
    int i = vertices.size();
    vertices.resize(vertices.size() + 4);

    vertices[i    ].position = VOXEL_POSITIONS[(int)quad.direction][0];
    vertices[i + 1].position = VOXEL_POSITIONS[(int)quad.direction][1];
    vertices[i + 2].position = VOXEL_POSITIONS[(int)quad.direction][2];
    vertices[i + 3].position = VOXEL_POSITIONS[(int)quad.direction][3];

    const color4& colorRGBA = m_blockPack->operator[](id).color;
    color3 color(colorRGBA.r, colorRGBA.g, colorRGBA.b);

    vertices[i    ].color = color;
    vertices[i + 1].color = color;
    vertices[i + 2].color = color;
    vertices[i + 3].color = color;
}

void ChunkMesher::generateIndices() {
    vvox::meshalg::generateQuadIndices<MeshIndex>(indices, vertices.size() / 4);
}

// TODO(Ben): Occlusion could be prebaked?
vvox::meshalg::VoxelFaces ChunkMesher::occludes(BlockID a, BlockID b, vvox::Axis axis) {
    vvox::meshalg::VoxelFaces rv;
    rv.face1 = b == 0 ? true : false;
    rv.face2 = a == 0 ? true : false;
    return rv;
}
