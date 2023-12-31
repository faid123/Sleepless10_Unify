/**********************************************************************************
* \file GLmodel.h
* \brief File contains the function definitions for all functions that are used
         to provide models.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once


#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include "../GLshader/glslshader.h"


struct RGBA
{
    glm::vec2 red;
    glm::vec2 green;
    glm::vec2 blue;
    glm::vec2 alpha;
};

struct VertexN {
    /*
        vertex values
    */
    // position
    glm::vec3 pos;
    // normal vector
    glm::vec3 normal;
    // texture coordinate
    glm::vec2 texCoord;
    // tangent vector
    //glm::vec3 tangent;

    // generate list of vertices
    static std::vector<VertexN> genList(float* vertices, int noVertices);

    // calculate tangent vectors for each face
    static void calcTanVectors(std::vector<VertexN>& list, std::vector<unsigned int>& indices);
};

//no texture included yet!
struct VAO_AOS
{
    glm::vec3 pos_vtx;
    glm::vec3 clr_vtx;
    glm::vec2 tex_vtx;

};

struct GLModel
{
    GLenum      primitive_type;
    GLuint      primitive_cnt;
    GLuint      vaoid;
    GLuint      vboid;
    GLuint      draw_cnt;
    std::vector<VAO_AOS> vao_aos_vector;

    std::string model_name;

    GLuint texId;

    GLuint width;
    GLuint height;
    GLuint depth;

    //Initalize a default box model
    void init_cube();
    void init_fbx();
    void init_fbxAnim(std::string inPath);
    void init_plane();
    void init_geom(const std::string& inPath);
    //Initalize a default wireframe model
    void init_wireframe();

    //Initalize default font model
    void init_font();

    void set_texId(GLuint id) { texId = id; };

    /*  Mesh format, only contains geometric data but not color/texture */
    typedef std::vector<VAO_AOS> VertexBufferType;
    typedef std::vector<GLushort> IndexBufferType;
    using Vertex = VAO_AOS;
    struct Mesh
    {
        Mesh() : numVertices(0), numTris(0), numIndices(0)
        {
            vertexBuffer.clear();
            indexBuffer.clear();
        }

        /*  Storing the actual vertex/index data */
        VertexBufferType vertexBuffer;
        IndexBufferType indexBuffer;

        int numVertices{};
        int numTris{};
        int numIndices{};

        /*  Once the buffer data has been copied into the pipeline, these array object and
            buffer objects will store  its "states".
            Later on if we want to render a mesh, we just need to bind the VAO.
            We don't need to copy the buffer data again.
        */
        /*GLuint VAO{};
        GLuint VBO{};
        GLuint IBO{};*/
    };


    Mesh _mesh;
    void BAR_update_uv(float newX);
    void reset_uv();

    std::map<std::string, int>m_bone_mapping = {};
    int m_num_bones = 0;

    /*Mesh LoadOBJMesh(char* filename);
    void addVertex(Mesh& mesh, const VAO_AOS& v);
    bool readDataLine(char* lineBuf, int* lineNum, FILE* fp, int MAX_LINE_LEN);*/
    /*void BuildIndexBuffer(int stacks, int slices, Mesh& mesh);
    void addIndex(Mesh& mesh, int index);
    void ComputeTangentsBitangents(VertexBufferType& vertices, const IndexBufferType& indices);
    void ComputeNormals(Mesh& mesh);
    void ComputeUVs(Mesh& mesh);*/
};