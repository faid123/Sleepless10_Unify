#include "GLmodel.h"
#include <array>
#include "../../Graphics/texture.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../Graphics/DDSLoader/ModelLoader.h"
#include <assimp/Importer.hpp>
#include "Animation/Skinned_Mesh.h"

#pragma region read_number
using namespace std;

template<typename T>
typename T   read_number(ifstream& fStream)
{
    unsigned char result[sizeof(T)];
    T number = 0;

    fStream.read((char*)(result), sizeof(T));
    memcpy(&number, result, sizeof(T));

    //read space
    fStream >> result[0];
    return number;
}

template <>
int   read_number(ifstream& fStream)
{
    unsigned char result[sizeof(int)];
    int number = 0;

    fStream.read((char*)(result), sizeof(int));

    memcpy(&number, result, sizeof(int));

    //read space
    fStream >> result[0];
    return number;
}
template <>
float   read_number(ifstream& fStream)
{
    unsigned char result[sizeof(float)];
    float number = 0;

    fStream.read((char*)(result), sizeof(float));
    memcpy(&number, result, sizeof(float));

    //read space
    fStream >> result[0];
    return number;
}
template <>
glm::vec3   read_number(ifstream& fStream)
{
    glm::vec3 number;
    number.x = read_number<float>(fStream);
    number.y = read_number<float>(fStream);
    number.z = read_number<float>(fStream);
    return number;
}

template <>
glm::vec2   read_number(ifstream& fStream)
{
    glm::vec2 number;
    number.x = read_number<float>(fStream);
    number.y = read_number<float>(fStream);
    return number;
}
template <>
glm::vec4   read_number(ifstream& fStream)
{
    glm::vec4 number;
    number.x = read_number<float>(fStream);
    number.y = read_number<float>(fStream);
    number.w = read_number<float>(fStream);
    number.z = read_number<float>(fStream);
    return number;
}

#pragma endregion


//Initialize a default box model
void GLModel::init_cube()
{
    std::array<glm::vec3, 8> pos_vtx;
    /*pos_vtx[0] = glm::vec3(-0.5f, -0.5f,-0.5f);     pos_vtx[1] = glm::vec3(0.5f, -0.5f, -0.5f);
    pos_vtx[2] = glm::vec3(-0.5f, 0.5f, -0.5f);     pos_vtx[3] = glm::vec3(0.5f, 0.5f, -0.5f);
    pos_vtx[4] = glm::vec3(-0.5f, -0.5f, 0.5f);     pos_vtx[5] = glm::vec3(0.5f, -0.5f, 0.5f);
    pos_vtx[6] = glm::vec3(-0.5f, 0.5f, 0.5f);      pos_vtx[7] = glm::vec3(0.5f, 0.5f, 0.5f);*/

    pos_vtx[0] = glm::vec3(-0.5f, -0.5f, -0.5f);    pos_vtx[1] = glm::vec3(-0.5f, -0.5f, 0.5f);
    pos_vtx[2] = glm::vec3(-0.5f, 0.5f, -0.5f);     pos_vtx[3] = glm::vec3(-0.5f, 0.5f, 0.5f);
    pos_vtx[4] = glm::vec3(0.5f, -0.5f, -0.5f);     pos_vtx[5] = glm::vec3(0.5f, -0.5f, 0.5f);
    pos_vtx[6] = glm::vec3(0.5f, 0.5f, -0.5f);      pos_vtx[7] = glm::vec3(0.5f, 0.5f, 0.5f); 

    
   /* v  0.0  0.0  0.0
    v  0.0  0.0  1.0
    v  0.0  1.0  0.0
    v  0.0  1.0  1.0
    v  1.0  0.0  0.0
    v  1.0  0.0  1.0
    v  1.0  1.0  0.0
    v  1.0  1.0  1.0*/


    std::array<glm::vec3, 8> clr_vtx;
    clr_vtx[0] = glm::vec3(1.0f, 0.f, 0.f);     clr_vtx[1] = glm::vec3(0.f, 1.f, 0.f);
    clr_vtx[2] = glm::vec3(1.f, 0.f, 1.f);      clr_vtx[3] = glm::vec3(0.f, 0.f, 1.f);
    clr_vtx[4] = glm::vec3(1.0f, 0.f, 0.f);     clr_vtx[5] = glm::vec3(0.f, 1.f, 0.f);
    clr_vtx[6] = glm::vec3(1.f, 0.f, 1.f);      clr_vtx[7] = glm::vec3(0.f, 0.f, 1.f);
    
    std::array<glm::vec2, 8> tex_vtx;

    tex_vtx[0] = glm::vec2(0.0f, 0.0f); tex_vtx[1] = glm::vec2(0.0f, 0.0f);
    tex_vtx[2] = glm::vec2(0.0f, 1.0f); tex_vtx[3] = glm::vec2(0.0f, 1.0f);
    tex_vtx[4] = glm::vec2(1.0f, 0.0f); tex_vtx[5] = glm::vec2(1.0f, 0.0f);
    tex_vtx[6] = glm::vec2(1.0f, 1.0f); tex_vtx[7] = glm::vec2(1.0f, 1.0f);


    std::vector<VAO_AOS> vao_aos_list;
    for (int i{ 0 }; i < 8; ++i)
    {
        VAO_AOS temp;
        temp.pos_vtx = pos_vtx[i];
        temp.clr_vtx = clr_vtx[i];
        temp.tex_vtx = tex_vtx[i];
        vao_aos_list.push_back(temp);
    }


    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(VAO_AOS) * vao_aos_list.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vbo_hdl, 0, sizeof(VAO_AOS) * vao_aos_list.size(), vao_aos_list.data());

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    //create attribute pointer for pos vertices
    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(VAO_AOS));
    glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);
    //create attribute pointer for clr vertices
    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl,
        sizeof(glm::vec3), sizeof(VAO_AOS));
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

    //create attribute pointer for texture vertices
    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl,
        sizeof(glm::vec3) + sizeof(glm::vec3), sizeof(VAO_AOS));
    glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 2, 2);

    /*std::array<GLushort, 36> idx_vtx
    {   
        0,  6,  4,
        0,  2,  6,
        0,  3,  2,
        0,  1,  3,
        2,  7,  6,
        2,  3,  7,
        4,  6,  7,
        4,  7,  5,
        0,  4,  5,
        0,  5,  1,
        1,  5,  7,
        1,  7,  3
    };*/
    std::array<GLushort, 36> idx_vtx
    {
        1,  7,  5,
        1,  3,  7,
        1,  4,  3,
        1,  2,  4,
        3,  8,  7,
        3,  4,  8,
        5,  7,  8,
        5,  8,  6,
        1,  5,  6,
        1,  6,  2,
        2,  6,  8,
        2,  8,  4
    }; 
    for (auto& x : idx_vtx)
        x -= 1;

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_vtx.size(),
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel
    vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
    vboid = vbo_hdl;
    primitive_type = GL_TRIANGLES;
    draw_cnt = (GLuint)idx_vtx.size();
    primitive_cnt = (GLuint)idx_vtx.size();

}

void GLModel::init_geom(const std::string& inPath)
{
    // Reading in 
    ifstream infile(inPath, ifstream::binary);
    assert(infile.is_open());

    ///read object name
    char c, space;
    string obj_name;
    infile.read(&c, sizeof(char));
    infile >> space;
    int nameLength = (int)c;
    for (int i = 0; i < nameLength; i++)
    {
        infile >> c;
        obj_name.push_back(c);
    }

    infile >> space;
    model_name = obj_name;
    // read number of vertices;
    int vertNum = read_number<int>(infile);
    vector<VertexN> mVertices;
    try 
    {
    mVertices.resize(vertNum);
    }
    catch (const std::bad_alloc& e) {
        std::cout << "Allocation failed: " << e.what() << '\n';
    }
    _mesh.vertexBuffer.resize(vertNum);

    //read positions in
    for (int i = 0; i < vertNum; ++i)
    {
        mVertices[i].pos = read_number<glm::vec3>(infile);
        _mesh.vertexBuffer[i].pos_vtx = mVertices[i].pos;
    }

    //read normals in
    for (int i = 0; i < vertNum; ++i)
    {
        mVertices[i].normal = read_number<glm::vec3>(infile);
        _mesh.vertexBuffer[i].clr_vtx = mVertices[i].normal;
    }
    //read textures in
    for (int i = 0; i < vertNum; ++i)
    {
        mVertices[i].texCoord = read_number<glm::vec2>(infile);
        _mesh.vertexBuffer[i].tex_vtx = mVertices[i].texCoord;
    }
    //read number of indices in
    int indNum = read_number<int>(infile);
    vector<GLushort> indices(indNum);
    for (int i = 0; i < indNum; ++i)
        indices[i] = read_number<GLushort>(infile);

    infile.close();

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(VertexN) * mVertices.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vbo_hdl, 0, sizeof(VertexN) * mVertices.size(), mVertices.data());

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    //create attribute pointer for pos vertices
    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(VertexN));
    glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    //create attribute pointer for clr vertices
    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl,
        sizeof(glm::vec3), sizeof(VertexN));
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

#if 1
    //create attribute pointer for tex uv vertices
    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl,
        sizeof(glm::vec3) * 2, sizeof(VertexN));
    glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 2, 2);
#endif

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * indices.size(),
        reinterpret_cast<GLvoid*>(indices.data()),
        GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel

    vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
    vboid = vbo_hdl;
    primitive_type = GL_TRIANGLES;
    draw_cnt = (GLuint)indices.size();
    primitive_cnt = (GLuint)indices.size();


    //fill in Mesh details
    _mesh.numVertices = vertNum;
    _mesh.numTris = draw_cnt / 3;
    _mesh.indexBuffer = indices;
    _mesh.numIndices = draw_cnt;

}



void GLModel::init_fbx()
{
    std::vector<glm::vec3> pos_vtx;
    /*pos_vtx[0] = glm::vec3(-0.5f, -0.5f,-0.5f);     pos_vtx[1] = glm::vec3(0.5f, -0.5f, -0.5f);
    pos_vtx[2] = glm::vec3(-0.5f, 0.5f, -0.5f);     pos_vtx[3] = glm::vec3(0.5f, 0.5f, -0.5f);
    pos_vtx[4] = glm::vec3(-0.5f, -0.5f, 0.5f);     pos_vtx[5] = glm::vec3(0.5f, -0.5f, 0.5f);
    pos_vtx[6] = glm::vec3(-0.5f, 0.5f, 0.5f);      pos_vtx[7] = glm::vec3(0.5f, 0.5f, 0.5f);*/

    if (auto walkingMan = aiImportFile("Resources/Models/Unicorn_Bone.fbx", aiProcessPreset_TargetRealtime_MaxQuality))
    {
        std::cout << "Success!\n";
        std::cout << "Number of meshes found in file: " << walkingMan->mNumMeshes << std::endl;
        std::cout << "Number of vertices in first mesh: " << walkingMan->mMeshes[0]->mNumVertices << std::endl;



        // process mesh in object file
        aiMesh* mesh = walkingMan->mMeshes[0];
        std::vector<VertexN> vertices(mesh->mNumVertices);
        std::vector<GLushort> indices(3 * mesh->mNumFaces);
        std::vector<Texture> textures;

        // vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) 
        {
            // position
            vertices[i].pos = glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );
      /*      std::cout << "correct pos: " << vertices[i].pos.x
                << " y: " << vertices[i].pos.y << " z: "
                << vertices[i].pos.z << std::endl;*/

            // normal vectors
            vertices[i].normal = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
     /*       std::cout << "correct n: " << vertices[i].normal.x
                << " y: " << vertices[i].normal.y << " z: "
                << vertices[i].normal.z << std::endl;*/

            // textures
            if (mesh->mTextureCoords[0]) {
                vertices[i].texCoord = glm::vec2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
            }
            else {
                vertices[i].texCoord = glm::vec2(0.0f);
            }

#if 1

            for (unsigned j = 0; j < mesh->GetNumUVChannels(); j++)
            {
                glTexCoord3d(mesh->mTextureCoords[0][j].x,
                    mesh->mTextureCoords[0][j].y,
                    mesh->mTextureCoords[0][j].z);
            }


            glVertex3d(mesh->mVertices[0].x, 
                mesh->mVertices[0].y, 
                mesh->mVertices[0].z);

#endif
#if 0
            // tangent vector
            vertices[i].tangent = {
                mesh->mTangents[i].x,
                mesh->mTangents[i].y,
                mesh->mTangents[i].z
            };
#endif
        }

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back((GLushort)(face.mIndices[j]));
               
            }
        }

#if 0
        // load data into mesh
        Mesh ret;

        // process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = walkingMan->mMaterials[mesh->mMaterialIndex];

            if (States::isActive<unsigned int>(&switches, NO_TEX)) {
                // use materials

                // 1. diffuse colors
                aiColor4D diff(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
                // 2. specular colors
                aiColor4D spec(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

                ret = Mesh(br, diff, spec);
            }
            else {
                // use textures

                // 1. diffuse maps
                std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                // 2. specular maps
                std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
                // 3. normal maps
                // .obj, use aiTextureType_HEIGHT
                std::vector<Texture> normalMaps = loadTextures(material, aiTextureType_NORMALS);
                textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

                ret = Mesh(br, textures);
            }
        }
#endif

        GLuint vbo_hdl;
        glCreateBuffers(1, &vbo_hdl);
        glNamedBufferStorage(vbo_hdl, sizeof(VertexN) * vertices.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
        glNamedBufferSubData(vbo_hdl, 0, sizeof(VertexN) * vertices.size(), vertices.data());

        GLuint vao_hdl;
        glCreateVertexArrays(1, &vao_hdl);

        //create attribute pointer for pos vertices
        glEnableVertexArrayAttrib(vao_hdl, 0);
        glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(VertexN));
        glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao_hdl, 0, 0);

        //create attribute pointer for clr vertices
        glEnableVertexArrayAttrib(vao_hdl, 1);
        glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl,
            sizeof(glm::vec3), sizeof(VertexN));
        glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao_hdl, 1, 1);

        
#if 1
        //create attribute pointer for tex uv vertices
        glEnableVertexArrayAttrib(vao_hdl, 2);
        glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl,
            sizeof(glm::vec3) * 2, sizeof(VertexN));
        glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao_hdl, 2, 2);
#endif

        

        GLuint ebo_hdl;
        glCreateBuffers(1, &ebo_hdl);
        glNamedBufferStorage(ebo_hdl,
            sizeof(GLushort) * indices.size(),
            reinterpret_cast<GLvoid*>(indices.data()),
            GL_DYNAMIC_STORAGE_BIT);

        glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
        glBindVertexArray(0);

        // Return an appropriately initialized instance of GLApp::GLModel

        vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
        vboid = vbo_hdl;
        primitive_type = GL_TRIANGLES;
        draw_cnt = (GLuint)indices.size();
        primitive_cnt = (GLuint)indices.size();

        // close file 
        aiReleaseImport(walkingMan);

    }

    else
    {
        std::cout << "Model not loaded and Fail!\n";
    }

    
     
    //texId = modelLoader::mLoader::LoadTextureDDS("Textures/unicorn2.dds");

#if 0
    //Check texture
    DDSFile* file = nullptr;

    if (file = dds_load("Textures/unicorn.dds"))
    {
        std::cout << "Texture success\n";
    }
    else
    {
        std::cout << "Texture fail\n";
    }

    std::cout << "Number of mipmap lmao: " << file->dwMipMapCount << "\n";


    // texture time losers
    GLuint texture = 0;
    width = file->dwWidth;
    height = file->dwHeight;
    depth = file->dwDepth;



#if 1
    // encapsulate two-dimensional textures
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    // allocate GPU storage for texture image data loaded from file
    glTextureStorage2D(texture, 1, GL_RGBA32F, file->dwWidth, file->dwHeight);
#endif

#if 0
    glGenTextures(1, &texId);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texId);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, file);
#endif

#if 1
    if (file)
    {
        // copy image data from client memory to GPU texture buffer memory
        glTextureSubImage2D(texture, 0, 0, 0, file->dwWidth, file->dwHeight,
            GL_RGBA, GL_UNSIGNED_BYTE, file);

        std::cout << "Texture in buffer memory! Success! \n";
    }
    else
    {
        std::string errormsg = "Failed to load texture ";
        std::cout << errormsg << std::endl;
        std::exit(EXIT_FAILURE);

    }
#endif

    texId = texture;

    dds_free(file);
#endif

}


void GLModel::init_plane()
{
    std::array<glm::vec3, 4> pos_vtx;
    pos_vtx[0] = glm::vec3(-0.5f, -0.5f,0.0f); pos_vtx[1] = glm::vec3(0.5f, -0.5f,0.0f);
    pos_vtx[2] = glm::vec3(-0.5f, 0.5f,0.0f);  pos_vtx[3] = glm::vec3(0.5f, 0.5f,0.0f);

    std::array<glm::vec3, 4> clr_vtx;
    clr_vtx[0] = glm::vec3(1.0f, 0.f, 0.f); clr_vtx[1] = glm::vec3(0.f, 1.f, 0.f);
    clr_vtx[2] = glm::vec3(1.f, 0.f, 1.f); clr_vtx[3] = glm::vec3(0.f, 0.f, 1.f);

    std::array<glm::vec2, 4> tex_vtx;

    tex_vtx[0] = glm::vec2(0.f, 0.f); tex_vtx[1] = glm::vec2(1.f, 0.f);
    tex_vtx[2] = glm::vec2(0.f, 1.f); tex_vtx[3] = glm::vec2(1.f, 1.f);

    std::vector<VAO_AOS> vao_aos_list;
    for (int i{ 0 }; i < 4; ++i)
    {
        VAO_AOS temp;
        temp.pos_vtx = pos_vtx[i];
        temp.clr_vtx = clr_vtx[i];
        temp.tex_vtx = tex_vtx[i];
        vao_aos_list.push_back(temp);
    }

    vao_aos_vector = vao_aos_list;

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(VAO_AOS) * vao_aos_list.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vbo_hdl, 0, sizeof(VAO_AOS) * vao_aos_list.size(), vao_aos_list.data());

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    //create attribute pointer for pos vertices
    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(VAO_AOS));
    glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);
    //create attribute pointer for clr vertices
    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl,
        sizeof(glm::vec3), sizeof(VAO_AOS));
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

    //create attribute pointer for texture vertices
    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl,
        sizeof(glm::vec3) + sizeof(glm::vec3), sizeof(VAO_AOS));
    glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 2, 2);

    std::array<GLushort, 4> idx_vtx;
    idx_vtx[0] = 2;
    idx_vtx[1] = 0;
    idx_vtx[2] = 3;
    idx_vtx[3] = 1;

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_vtx.size(),
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel
    vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
    vboid = vbo_hdl;
    primitive_type = GL_TRIANGLE_STRIP;
    draw_cnt = (GLuint)idx_vtx.size();
    primitive_cnt = (GLuint)idx_vtx.size();
}

void GLModel::init_font()
{
    std::array<glm::vec2, 4> pos_vtx;
    pos_vtx[0] = glm::vec2(0.5f, -0.5f); pos_vtx[1] = glm::vec2(0.5f, 0.5f);
    pos_vtx[2] = glm::vec2(-0.5f, 0.5f); pos_vtx[3] = glm::vec2(-0.5f, -0.5f);

    unsigned int vao_hdl, vbo_hdl;
    glGenVertexArrays(1, &vao_hdl);
    glGenBuffers(1, &vbo_hdl);
    glBindVertexArray(vao_hdl);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_hdl);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::array<GLushort, 6> idx_vtx;
    idx_vtx[0] = 0; idx_vtx[1] = 1; idx_vtx[2] = 2;
    idx_vtx[3] = 2; idx_vtx[4] = 3; idx_vtx[5] = 0;

    //
    //GLuint ebo_hdl;
    //glCreateBuffers(1, &ebo_hdl);
    //glNamedBufferStorage(ebo_hdl,
    //    sizeof(GLushort) * idx_vtx.size(),
    //    reinterpret_cast<GLvoid*>(idx_vtx.data()),
    //    GL_DYNAMIC_STORAGE_BIT);

    vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
    vboid = vbo_hdl;
    primitive_type = GL_TRIANGLES;
    draw_cnt = (GLuint)idx_vtx.size();
    primitive_cnt = (GLuint)idx_vtx.size();
}
void GLModel::init_wireframe()
{
    //using GL_Lines

    std::array<glm::vec3, 24> pos_vtx
    {
        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, -0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, -0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),

        glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.5f, -0.5f,   0.5f),
        glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(0.5f, 0.5f,    0.5f),
        glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec3(-0.5f, -0.5f,  0.5f),
        glm::vec3(0.5f, 0.5f,   0.5f), glm::vec3(0.5f, -0.5f,   0.5f),

        glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.5f, -0.5f, 0.5f),
        glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.5f, 0.5f, 0.5f)
    };

    GLuint vbo_hdl;

    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(),
        reinterpret_cast<GLvoid*>(pos_vtx.data()), GL_DYNAMIC_STORAGE_BIT);

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
    glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    std::array<GLushort, 24> idx_vtx;
    for (unsigned short i = 0; i < (unsigned short)pos_vtx.size(); i++)
        idx_vtx[i] = i;


    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_vtx.size(),
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    // Return an appropriately initialized instance of GLApp::GLModel
    vaoid = vao_hdl; // set up VAO same as in GLApp::points_model
    primitive_type = GL_LINE;
    draw_cnt = (GLuint)idx_vtx.size();
    primitive_cnt = (GLuint)idx_vtx.size();
}


//GLModel::Mesh GLModel::LoadOBJMesh(char* filename)
//{
//    Mesh mesh;
//
//    const int MAX_LINE_LEN = 1024;
//    char lineBuf[MAX_LINE_LEN + 1];
//    int numLines;
//
//    FILE* fp;
//    if (fopen_s(&fp, filename, "r") != 0)
//    {
//        std::cerr << "Failed to open " << filename << "\n";
//        exit(1);
//    }
//
//    int posID = 0;
//
//    while (!feof(fp))
//    {
//        if (readDataLine(lineBuf, &numLines, fp, MAX_LINE_LEN))
//        {
//            if (lineBuf[0] == 'v')
//            {
//                char dataType[MAX_LINE_LEN + 1];
//                float x, y, z;
//                sscanf_s(lineBuf, "%s %f %f %f", dataType, (int)sizeof(dataType), &x, &y, &z);
//
//                Vertex v;
//                if (!strcmp(dataType, "v"))
//                {
//                    v.pos_vtx = glm::vec3(x, y, z);
//                    if (posID >= mesh.numVertices)
//                    {
//                        mesh.vertexBuffer.push_back(v);
//                        ++mesh.numVertices;
//                    }
//                    else
//                        mesh.vertexBuffer[posID].pos_vtx = v.pos_vtx;
//
//                    ++posID;
//                }
//            }
//            else if (lineBuf[0] == 'f')
//            {
//                ++mesh.numTris;
//
//                std::vector<char*> faceData;
//                char* tokWS, * ptrFront, * ptrRear;
//                char* ct;
//
//                tokWS = strtok_s(lineBuf, " ", &ct);
//                tokWS = strtok_s(NULL, " ", &ct);
//                while (tokWS != NULL)
//                {
//                    faceData.push_back(tokWS);
//                    tokWS = strtok_s(NULL, " ", &ct);
//                }
//
//                if (faceData.size() > 3)
//                {
//                    std::cerr << "Only triangulated mesh is accepted.\n";
//                    exit(1);
//                }
//
//                for (int i = 0; i < (int)faceData.size(); i++)
//                {
//                    int vertNum;
//
//                    ptrFront = strchr(faceData[i], '/');
//                    if (ptrFront == NULL)
//                    {
//                        vertNum = atoi(faceData[i]) - 1;
//                        mesh.indexBuffer.push_back(vertNum);
//                        ++mesh.numIndices;
//                    }
//                    else
//                    {
//                        char* tokFront, * tokRear, * cF;
//                        ptrRear = strrchr(faceData[i], '/');
//                        tokFront = strtok_s(faceData[i], "/", &cF);
//                        vertNum = atoi(tokFront) - 1;
//
//                        if (ptrRear == ptrFront)
//                        {
//                            mesh.indexBuffer.push_back(vertNum);
//                            ++mesh.numIndices;
//                        }
//                        else
//                        {
//                            if (ptrRear != ptrFront + 1)
//                            {
//                                tokRear = strtok_s(NULL, "/", &cF);
//                            }
//
//                            tokRear = strtok_s(NULL, "/", &cF);
//                            mesh.indexBuffer.push_back(vertNum);
//                            ++mesh.numIndices;
//                        }
//                    }
//                }
//            }
//
//        }
//    }
//
//    if (fp)
//        if (fclose(fp))
//        {
//            std::cerr << "Failed to close " << filename << "\n";
//            exit(1);
//        }
//
//    //ComputeNormals(mesh);
//    //ComputeUVs(mesh);
//
//    return mesh;
//}

/******************************************************************************/
/*!
\fn     bool readDataLine(char *lineBuf, int *lineNum, FILE *fp, int MAX_LINE_LEN)
\brief
        Read a process the next line in the file
\param  lineBuf
        Buffer to store the line
\param  lineNum
        The line index
\param  fp
        The file handler
\param  MAX_LINE_LEN
        The max allowed length of the line
*/
/******************************************************************************/
//bool GLModel::readDataLine(char* lineBuf, int* lineNum, FILE* fp, int MAX_LINE_LEN)
//{
//    while (!feof(fp))
//    {
//        // Read next line from input file.
//        (*lineNum)++;
//
//        //char* line = fgets(lineBuf, MAX_LINE_LEN + 1, fp);
//
//        // Check that the line is not too long.
//        int lineLen = (int)strlen(lineBuf);
//        if (lineLen == MAX_LINE_LEN && lineBuf[MAX_LINE_LEN - 1] != '\n')
//            continue;
//
//        // Skip comment and empty line.
//        if (lineLen > 1 && lineBuf[0] == '#') continue;    // Skip comment line.
//        if (lineLen == 1) continue;  // Skip empty line.
//
//        for (int i = 0; i < lineLen; i++)
//            if (!isspace(lineBuf[i])) return true;   // Return the line if it is not all spaces.
//    }
//
//    return false;  // End of file.
//}

void GLModel::init_fbxAnim(std::string inPath)
{
    ifstream infile(inPath, ifstream::binary);
    assert(infile.is_open());

    ///read object name
    char c, space;
    string obj_name;
    infile.read(&c, sizeof(char));
    infile >> space;
    int nameLength = (int)c;
    for (int i = 0; i < nameLength; i++)
    {
        infile >> c;
        obj_name.push_back(c);
    }

    infile >> space;
    model_name = obj_name;
    // read number of vertices;
    int vertNum = read_number<int>(infile);
    vector<AnimVertex> mVertices;
    try
    {
        mVertices.resize(vertNum);
    }
    catch (const std::bad_alloc& e) {
        std::cout << "Allocation failed: " << e.what() << '\n';
    }

    //read positions in
    for (int i = 0; i < vertNum; ++i)
    {
        mVertices[i].position = read_number<glm::vec3>(infile);
    }


    //read normals in
    for (int i = 0; i < vertNum; ++i)
    {
        mVertices[i].normal = read_number<glm::vec3>(infile);
    }


    //read textures in
    for (int i = 0; i < vertNum; ++i)
        mVertices[i].uv = read_number<glm::vec2>(infile);

    for (int i = 0; i < vertNum; ++i)
        mVertices[i].boneIds = read_number<glm::vec4>(infile);

    for (int i = 0; i < vertNum; ++i)
        mVertices[i].boneWeights = read_number<glm::vec4>(infile);

    //read number of indices in
    int indNum = read_number<int>(infile);
    vector<GLushort> indices(indNum);
    for (int i = 0; i < indNum; ++i)
    {
        indices[i] = read_number<GLushort>(infile);
    }

    //infile.close();
    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);
    glNamedBufferStorage(vbo_hdl, sizeof(AnimVertex) * mVertices.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferSubData(vbo_hdl, 0, sizeof(AnimVertex) * mVertices.size(), mVertices.data());

    GLuint vao_hdl;
    glCreateVertexArrays(1, &vao_hdl);

    //create attribute pointer for pos vertices
    glEnableVertexArrayAttrib(vao_hdl, 0);
    glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(AnimVertex));
    glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 0, 0);

    //create attribute pointer for normal vertices
    glEnableVertexArrayAttrib(vao_hdl, 1);
    glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl,
        sizeof(glm::vec3), sizeof(AnimVertex));
    glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 1, 1);

    //create attribute pointer for tex uv vertices
    glEnableVertexArrayAttrib(vao_hdl, 2);
    glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl,
        sizeof(glm::vec3) * 2, sizeof(AnimVertex));
    glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 2, 2);

    //create attribute pointer for boneid
    glEnableVertexArrayAttrib(vao_hdl, 3);
    glVertexArrayVertexBuffer(vao_hdl, 3, vbo_hdl,
        sizeof(glm::vec3) * 2 + sizeof(glm::vec2), sizeof(AnimVertex));
    glVertexArrayAttribFormat(vao_hdl, 3, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 3, 3);

    //create attribute pointer for bone weight
    glEnableVertexArrayAttrib(vao_hdl, 4);
    glVertexArrayVertexBuffer(vao_hdl, 4, vbo_hdl,
        sizeof(glm::vec3) * 2 + sizeof(glm::vec2) + sizeof(glm::vec4),
        sizeof(AnimVertex));
    glVertexArrayAttribFormat(vao_hdl, 4, 4, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vao_hdl, 4, 4);

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * indices.size(),
        reinterpret_cast<GLvoid*>(indices.data()),
        GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
    glBindVertexArray(0);

    vaoid = vao_hdl;
    vboid = vbo_hdl;
    primitive_type = GL_TRIANGLES;
    draw_cnt = (GLuint)indices.size();
    primitive_cnt = (GLuint)indices.size();
}

//Updates the Texture Coord for UV
void GLModel::BAR_update_uv(float newX)
{
    //[1] btm right
    vao_aos_vector[1].tex_vtx.x = newX;
    //[3] top right
    vao_aos_vector[3].tex_vtx.x = newX;

    //rebind data
    glNamedBufferSubData(vboid, 0, sizeof(VAO_AOS) * vao_aos_vector.size(), vao_aos_vector.data());
}
//Set UV back to 1-0
void GLModel::reset_uv()
{
    std::array<glm::vec2, 4> tex_vtx;

    tex_vtx[0] = glm::vec2(0.f, 0.f); tex_vtx[1] = glm::vec2(1.f, 0.f);
    tex_vtx[2] = glm::vec2(0.f, 1.f); tex_vtx[3] = glm::vec2(1.f, 1.f);

    for (int i = 0; i < 4; ++i)
    {
        vao_aos_vector[i].tex_vtx = tex_vtx[i];
    }
    //rebind data
    glNamedBufferSubData(vboid, 0, sizeof(VAO_AOS)* vao_aos_vector.size(), vao_aos_vector.data());
}