/**********************************************************************************
* \file Graphics.cpp
* \brief File contains the function definitions for all functions that are used
         to provide graphics.
* \author Nur Syazwan, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include <iostream>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include "Graphics/GLmodel/GLmodel.h"

//SOIL texture loader
#include <SOIL.h>

namespace modelLoader
{
    class mLoader
    {
    public:
        static void LoadModel();
        static GLuint LoadTextureDDS(const char* filename);
        static GLuint SOIL_LoadTextureDDS(const char* filename);
        void setTextureDDS(std::string filename);
        static bool loadCubeMapTextureDDS(const char* filename, unsigned& num);
    };

}; // namespace modelLoader


