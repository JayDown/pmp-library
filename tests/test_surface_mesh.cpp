//=============================================================================
// Copyright (C) 2017 Daniel Sieger
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// =============================================================================

#include "gtest/gtest.h"

#include <surface_mesh/SurfaceMesh.h>
#include <vector>

using namespace surface_mesh;

class SurfaceMeshTest : public ::testing::Test
{
public:
    SurfaceMesh mesh;
    SurfaceMesh::Vertex v0,v1,v2,v3;
    SurfaceMesh::Face f0;

    void addTriangle()
    {
        v0 = mesh.addVertex(Point(0,0,0));
        v1 = mesh.addVertex(Point(1,0,0));
        v2 = mesh.addVertex(Point(0,1,0));
        f0 = mesh.addTriangle(v0,v1,v2);
    }

    void addQuad()
    {
        v0 = mesh.addVertex(Point(0,0,0));
        v1 = mesh.addVertex(Point(1,0,0));
        v2 = mesh.addVertex(Point(1,1,0));
        v3 = mesh.addVertex(Point(0,1,0));
        f0 = mesh.addQuad(v0,v1,v2,v3);
    }
};

TEST_F(SurfaceMeshTest, emptyMesh)
{
    EXPECT_EQ(mesh.nVertices(), 0);
    EXPECT_EQ(mesh.nEdges(), 0);
    EXPECT_EQ(mesh.nFaces(), 0);
}

TEST_F(SurfaceMeshTest, insertRemoveSingleVertex)
{
    auto v0 = mesh.addVertex(Point(0, 0, 0));
    EXPECT_EQ(mesh.nVertices(), 1);
    mesh.deleteVertex(v0);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nVertices(), 0);
}

TEST_F(SurfaceMeshTest, insertRemoveSingleTriangle)
{
    auto v0 = mesh.addVertex(Point(0, 0, 0));
    auto v1 = mesh.addVertex(Point(1, 0, 0));
    auto v2 = mesh.addVertex(Point(0, 1, 0));
    auto f0 = mesh.addTriangle(v0, v1, v2);
    EXPECT_EQ(mesh.nVertices(), 3);
    EXPECT_EQ(mesh.nEdges(), 3);
    EXPECT_EQ(mesh.nFaces(), 1);
    mesh.deleteFace(f0);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nVertices(), 0);
    EXPECT_EQ(mesh.nEdges(), 0);
    EXPECT_EQ(mesh.nFaces(), 0);
}

TEST_F(SurfaceMeshTest, insertRemoveSingleQuad)
{
    auto v0 = mesh.addVertex(Point(0, 0, 0));
    auto v1 = mesh.addVertex(Point(1, 0, 0));
    auto v2 = mesh.addVertex(Point(1, 1, 0));
    auto v3 = mesh.addVertex(Point(0, 1, 0));
    auto f0 = mesh.addQuad(v0, v1, v2, v3);
    EXPECT_EQ(mesh.nVertices(), 4);
    EXPECT_EQ(mesh.nEdges(), 4);
    EXPECT_EQ(mesh.nFaces(), 1);
    mesh.deleteFace(f0);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nVertices(), 0);
    EXPECT_EQ(mesh.nEdges(), 0);
    EXPECT_EQ(mesh.nFaces(), 0);
}

TEST_F(SurfaceMeshTest, insertRemoveSinglePolygonalFace)
{
    std::vector<SurfaceMesh::Vertex> vertices(4);
    vertices[0] = mesh.addVertex(Point(0, 0, 0));
    vertices[1] = mesh.addVertex(Point(1, 0, 0));
    vertices[2] = mesh.addVertex(Point(1, 1, 0));
    vertices[3] = mesh.addVertex(Point(0, 1, 0));

    auto f0 = mesh.addFace(vertices);
    EXPECT_EQ(mesh.nVertices(), 4);
    EXPECT_EQ(mesh.nEdges(), 4);
    EXPECT_EQ(mesh.nFaces(), 1);
    mesh.deleteFace(f0);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nVertices(), 0);
    EXPECT_EQ(mesh.nEdges(), 0);
    EXPECT_EQ(mesh.nFaces(), 0);
}

TEST_F(SurfaceMeshTest, deleteCenterVertex)
{
    ASSERT_TRUE(mesh.read("data/vertex_onering.off"));
    EXPECT_EQ(mesh.nVertices(), 7);
    EXPECT_EQ(mesh.nFaces(), 6);
    SurfaceMesh::Vertex v0(3); // the central vertex
    mesh.deleteVertex(v0);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nVertices(), 0);
    EXPECT_EQ(mesh.nFaces(), 0);
}

TEST_F(SurfaceMeshTest, deleteCenterEdge)
{
    ASSERT_TRUE(mesh.read("data/edge_onering.off"));
    EXPECT_EQ(mesh.nVertices(), 10);
    EXPECT_EQ(mesh.nFaces(), 10);
    // the two vertices of the center edge
    SurfaceMesh::Vertex v0(4);
    SurfaceMesh::Vertex v1(5);

    auto e = mesh.findEdge(v0, v1);
    mesh.deleteEdge(e);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nVertices(), 10);
    EXPECT_EQ(mesh.nFaces(), 8);
}

TEST_F(SurfaceMeshTest, copy)
{
    auto v0 = mesh.addVertex(Point(0,0,0));
    auto v1 = mesh.addVertex(Point(1,0,0));
    auto v2 = mesh.addVertex(Point(0,1,0));
    mesh.addTriangle(v0,v1,v2);

    SurfaceMesh m2 = mesh;
    EXPECT_EQ(m2.nVertices(), 3);
    EXPECT_EQ(m2.nEdges(), 3);
    EXPECT_EQ(m2.nFaces(), 1);
}

TEST_F(SurfaceMeshTest, assignment)
{
    auto v0 = mesh.addVertex(Point(0,0,0));
    auto v1 = mesh.addVertex(Point(1,0,0));
    auto v2 = mesh.addVertex(Point(0,1,0));
    mesh.addTriangle(v0,v1,v2);

    SurfaceMesh m2;
    m2.assign(mesh);
    EXPECT_EQ(m2.nVertices(), 3);
    EXPECT_EQ(m2.nEdges(), 3);
    EXPECT_EQ(m2.nFaces(), 1);
}

TEST_F(SurfaceMeshTest, vertexProperties)
{
    addTriangle();
    // explicit add
    auto vidx = mesh.addVertexProperty<int>("v:idx");
    vidx[v0] = 0;
    EXPECT_EQ(mesh.vertexProperties().size(), 5);
    mesh.removeVertexProperty(vidx);
    EXPECT_EQ(mesh.vertexProperties().size(), 4);

    // implicit add
    vidx = mesh.vertexProperty<int>("v:idx2");
    EXPECT_EQ(mesh.vertexProperties().size(), 5);
    mesh.removeVertexProperty(vidx);
    EXPECT_EQ(mesh.vertexProperties().size(), 4);
}

TEST_F(SurfaceMeshTest, halfedgeProperties)
{
    addTriangle();
    // explicit add
    auto hidx = mesh.addHalfedgeProperty<int>("h:idx");
    auto h = mesh.halfedge(v0);
    hidx[h] = 0;
    EXPECT_EQ(mesh.halfedgeProperties().size(), 3);
    mesh.removeHalfedgeProperty(hidx);
    EXPECT_EQ(mesh.halfedgeProperties().size(), 2);

    // implicit add
    hidx = mesh.halfedgeProperty<int>("h:idx2");
    EXPECT_EQ(mesh.halfedgeProperties().size(), 3);
    mesh.removeHalfedgeProperty(hidx);
    EXPECT_EQ(mesh.halfedgeProperties().size(), 2);
}

TEST_F(SurfaceMeshTest, edgeProperties)
{
    addTriangle();
    // explicit add
    auto eidx = mesh.addEdgeProperty<int>("e:idx");
    auto e = mesh.edge(mesh.halfedge(v0));
    eidx[e] = 0;
    EXPECT_EQ(mesh.edgeProperties().size(), 2);
    mesh.removeEdgeProperty(eidx);
    EXPECT_EQ(mesh.edgeProperties().size(), 1);

    // implicit add
    eidx = mesh.edgeProperty<int>("e:idx2");
    EXPECT_EQ(mesh.edgeProperties().size(), 2);
    mesh.removeEdgeProperty(eidx);
    EXPECT_EQ(mesh.edgeProperties().size(), 1);
}

TEST_F(SurfaceMeshTest, faceProperties)
{
    addTriangle();
    // explicit add
    auto fidx = mesh.addFaceProperty<int>("f:idx");
    fidx[f0] = 0;
    EXPECT_EQ(mesh.faceProperties().size(), 3);
    mesh.removeFaceProperty(fidx);
    EXPECT_EQ(mesh.faceProperties().size(), 2);

    // implicit add
    fidx = mesh.faceProperty<int>("f:idx2");
    EXPECT_EQ(mesh.faceProperties().size(), 3);
    mesh.removeFaceProperty(fidx);
    EXPECT_EQ(mesh.faceProperties().size(), 2);
}


TEST_F(SurfaceMeshTest, polyIO)
{
    addTriangle();
    mesh.write("test.poly");
    mesh.clear();
    EXPECT_TRUE(mesh.isEmpty());
    mesh.read("test.poly");
    EXPECT_EQ(mesh.nVertices(), 3);
    EXPECT_EQ(mesh.nFaces(), 1);
}

TEST_F(SurfaceMeshTest, objIO)
{
    addTriangle();
    mesh.updateVertexNormals();
    mesh.addHalfedgeProperty<TextureCoordinate>("h:texcoord",TextureCoordinate(0,0,0));
    mesh.write("test.obj");
    mesh.clear();
    EXPECT_TRUE(mesh.isEmpty());
    mesh.read("test.obj");
    EXPECT_EQ(mesh.nVertices(), 3);
    EXPECT_EQ(mesh.nFaces(), 1);
}

TEST_F(SurfaceMeshTest, offIO)
{
    addTriangle();
    mesh.updateVertexNormals();
    mesh.addVertexProperty<TextureCoordinate>("v:texcoord",TextureCoordinate(0,0,0));
    mesh.addVertexProperty<Color>("v:color",Color(0,0,0));
    mesh.write("test.off");
    mesh.clear();
    EXPECT_TRUE(mesh.isEmpty());
    mesh.read("test.off");
    EXPECT_EQ(mesh.nVertices(), 3);
    EXPECT_EQ(mesh.nFaces(), 1);
}

TEST_F(SurfaceMeshTest, stlIO)
{
    mesh.read("data/icosahedron_ascii.stl");
    EXPECT_EQ(mesh.nVertices(), 12);
    EXPECT_EQ(mesh.nFaces(), 20);
    EXPECT_EQ(mesh.nEdges(), 30);
    mesh.clear();
    mesh.read("data/icosahedron_binary.stl");
    EXPECT_EQ(mesh.nVertices(), 12);
    EXPECT_EQ(mesh.nFaces(), 20);
    EXPECT_EQ(mesh.nEdges(), 30);
    mesh.updateFaceNormals();
    mesh.write("test.stl");
}


TEST_F(SurfaceMeshTest, faceNormals)
{
    mesh.read("data/icosahedron_ascii.stl");
    mesh.updateFaceNormals();
    auto fnormals = mesh.getFaceProperty<Normal>("f:normal");
    auto fn0 = fnormals[SurfaceMesh::Face(0)];
    EXPECT_GT(norm(fn0), 0);
}

TEST_F(SurfaceMeshTest, vertexNormals)
{
    mesh.read("data/icosahedron_ascii.stl");
    mesh.updateVertexNormals();
    auto vnormals = mesh.getVertexProperty<Normal>("v:normal");
    auto vn0 = vnormals[SurfaceMesh::Vertex(0)];
    EXPECT_GT(norm(vn0), 0);
}

TEST_F(SurfaceMeshTest, polygonalFaceNormal)
{
    std::vector<SurfaceMesh::Vertex> vertices(5);
    vertices[0] = mesh.addVertex(Point(0,0,0));
    vertices[1] = mesh.addVertex(Point(1,0,0));
    vertices[2] = mesh.addVertex(Point(1,1,0));
    vertices[3] = mesh.addVertex(Point(0.5,1,0));
    vertices[4] = mesh.addVertex(Point(0,1,0));
    auto f0 = mesh.addFace(vertices);
    auto n0 = mesh.computeFaceNormal(f0);
    EXPECT_GT(norm(n0), 0);
}

TEST_F(SurfaceMeshTest, vertexIterators)
{
    addTriangle();
    size_t sumIdx(0);
    for (auto v : mesh.vertices())
    {
        sumIdx += v.idx();
    }
    EXPECT_EQ(sumIdx, 3);
}

TEST_F(SurfaceMeshTest, edgeIterators)
{
    addTriangle();
    size_t sumIdx(0);
    for (auto e : mesh.edges())
    {
        sumIdx += e.idx();
    }
    EXPECT_EQ(sumIdx, 3);
}

TEST_F(SurfaceMeshTest, halfedgeIterators)
{
    addTriangle();
    size_t sumIdx(0);
    for (auto h : mesh.halfedges())
    {
        sumIdx += h.idx();
    }
    EXPECT_EQ(sumIdx, 15);
}

TEST_F(SurfaceMeshTest, faceIterators)
{
    addTriangle();
    size_t sumIdx(0);
    for (auto f : mesh.faces())
    {
        sumIdx += f.idx();
    }
    EXPECT_EQ(sumIdx, 0);
}


TEST_F(SurfaceMeshTest, isTriangleMesh)
{
    addTriangle();
    EXPECT_TRUE(mesh.isTriangleMesh());
}

TEST_F(SurfaceMeshTest, isQuadMesh)
{
    auto v0 = mesh.addVertex(Point(0,0,0));
    auto v1 = mesh.addVertex(Point(1,0,0));
    auto v2 = mesh.addVertex(Point(1,1,0));
    auto v3 = mesh.addVertex(Point(0,1,0));
    mesh.addQuad(v0,v1,v2,v3);
    EXPECT_TRUE(mesh.isQuadMesh());
    EXPECT_FALSE(mesh.isTriangleMesh());
    mesh.triangulate();
    EXPECT_TRUE(mesh.isTriangleMesh());
}

TEST_F(SurfaceMeshTest, polyMesh)
{
    std::vector<SurfaceMesh::Vertex> vertices(5);
    vertices[0] = mesh.addVertex(Point(0,0,0));
    vertices[1] = mesh.addVertex(Point(1,0,0));
    vertices[2] = mesh.addVertex(Point(1,1,0));
    vertices[3] = mesh.addVertex(Point(0.5,1,0));
    vertices[4] = mesh.addVertex(Point(0,1,0));
    mesh.addFace(vertices);
    EXPECT_FALSE(mesh.isTriangleMesh());
    EXPECT_FALSE(mesh.isQuadMesh());
    mesh.triangulate();
    EXPECT_TRUE(mesh.isTriangleMesh());
}

TEST_F(SurfaceMeshTest, valence)
{
    addTriangle();
    size_t sumValence(0);
    for (auto v : mesh.vertices())
    {
        sumValence += mesh.valence(v);
    }
    EXPECT_EQ(sumValence, 6);
}

TEST_F(SurfaceMeshTest, collapse)
{
    addQuad();
    mesh.triangulate();
    EXPECT_EQ(mesh.nFaces(), 2);
    auto h0  = mesh.findHalfedge(v3,v2);
    if (mesh.isCollapseOk(h0))
        mesh.collapse(h0);
    mesh.garbageCollection();
    EXPECT_EQ(mesh.nFaces(), 1);
}

TEST_F(SurfaceMeshTest, faceSplit)
{
    addQuad();
    EXPECT_EQ(mesh.nFaces(), 1);
    Point p(0.5,0.5,0);
    mesh.split(f0,p);
    EXPECT_EQ(mesh.nFaces(), 4);
}

TEST_F(SurfaceMeshTest, edgeSplit)
{
    addTriangle();
    EXPECT_EQ(mesh.nFaces(), 1);
    auto e = mesh.findEdge(v0,v1);
    Point p0 = mesh.position(v0);
    Point p1 = mesh.position(v1);
    Point p = (p0 + p1) * 0.5;
    mesh.split(e,p);
    EXPECT_EQ(mesh.nFaces(), 2);
}

TEST_F(SurfaceMeshTest, edgeFlip)
{
    mesh.read("data/edge_onering.off");
    EXPECT_EQ(mesh.nVertices(), 10);
    EXPECT_EQ(mesh.nFaces(), 10);

    // the two vertices of the center edge
    SurfaceMesh::Vertex v0(4);
    SurfaceMesh::Vertex v1(5);
    auto e = mesh.findEdge(v0,v1);
    if (mesh.isFlipOk(e))
        mesh.flip(e);
    EXPECT_EQ(mesh.nVertices(), 10);
    EXPECT_EQ(mesh.nFaces(), 10);
}