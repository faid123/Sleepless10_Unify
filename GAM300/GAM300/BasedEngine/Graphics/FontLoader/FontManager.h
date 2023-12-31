/**********************************************************************************
* \file FontManager.h
* \brief File contains the function declarations for all functions that are used
         to manage fonts.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once


#include "Graphics/framework.h"
#include "Graphics/Graphics/Graphics.h"


#include <map>

struct Character
{
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
    glm::vec2   startUV = glm::vec2(0, 0);
    glm::vec2   endUV = glm::vec2(0, 0);
};

struct FontVertex
{
    float xPos;
    float yPos;
    float u;
    float v;

    FontVertex(float x, float y,float U,float V) :
        xPos{x},
        yPos{y},
        u{U},
        v{V}
    {}
};


class FontComponent;

struct TextData
{
    Transform* trans;
    FontComponent* text;
};

class FontManager
{

    static std::list<TextData> _textData;
    static GLModel font_model;
    static Graphics::shader_iterator shader_ref;

    static std::map<char, Character> Characters;
    static unsigned int texture;
public:
    static void LoadFont();
    static void Init();
    static void Exit();
    static void RenderAllText();
    static void RenderText(FontComponent* text, Transform* trans);
    static void RenderText(std::string text, float posx, float posy, float _scale);

    static void AddDataById(EntityId id);
    static void RemoveDataById(EntityId id);
    static void UpdateTransform(Matrix3 camMat);

};

