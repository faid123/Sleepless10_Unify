/**********************************************************************************
* \file FontManager.cpp
* \brief File contains the function definitions for all functions that are used
         to manage fonts.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "FontManager.h"


//TEST
#include <glm/gtc/type_ptr.hpp>

GLModel FontManager::font_model;
Graphics::shader_iterator FontManager::shader_ref;
std::list<TextData> FontManager::_textData;
std::map<char, Character> FontManager::Characters;
unsigned int FontManager::texture;

void FontManager::LoadFont()
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        Logger::LoggerError("ERROR::FREETYPE: Could not init FreeType Library");
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    FT_Face face;
    //Load specific font
    if (FT_New_Face(ft, "./Assets/Fonts/arial.ttf", 0, &face))
    {
        Logger::LoggerError("ERROR::FREETYPE: Failed to load font");
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    //SetFontSize
    FT_Set_Pixel_Sizes(face, 0, 48);
    
    unsigned int textureWidth = 0, textureHeight = 0;
    
    //First loop to compute textureWidth and textureHeight
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        // tell FreeType to create an 8-bit grayscale bitmap image for use
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        //In this sample we are building a one-row texture (all character's bufers are concatenated in
        // one row
        textureWidth += face->glyph->bitmap.width;

        // Getting the max possible height
        textureHeight = std::max(textureHeight, face->glyph->bitmap.rows);

        //Now store character for later use
        Character character = {
               texture,
               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
               (unsigned int)(face->glyph->advance.x)
            };
        Characters.insert(std::pair<char, Character>(c, character));
    }

    // generate texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        textureWidth,
        textureHeight,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        NULL // can be NULL
        );
  
    // We require 1 byte alignment when uploading texture data 
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

    // Second loop to build one atlas-texture map for the whole font
    int UJumps = 0, VJumps = 0;
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        
        //startUV
        float uS = (float)(UJumps) / (float)(textureWidth);
        float vS = (float)(VJumps) / (float)(textureHeight);
        
        //endUV
        float uE = (float)(UJumps + (int)face->glyph->bitmap.width);
        uE = uE / (float)(textureWidth);
        float vE = (float)(VJumps + face->glyph->bitmap.rows);
        vE = vE / (float)(textureHeight);
        
        //CharacterGlyph structure has now 2 additional members: 
        //(Vector2D startUV, Vector2D endUV)
        Characters[c].startUV = glm::vec2(uS, vS);
        Characters[c].endUV = glm::vec2(uE,vE);
        
        if(face->glyph->bitmap.width != 0)
        {
            glTexSubImage2D(
                GL_TEXTURE_2D,
                0,
                UJumps,
                VJumps,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
                );
        }
        //offset along u
        UJumps += face->glyph->bitmap.width;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    //Free after done
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


void FontManager::Init()
{
    LoadFont();
    font_model.init_font();

    SystemManager* sys = ECSEngine::GetSystemManager();
    std::shared_ptr<Graphics> graphics = std::dynamic_pointer_cast<Graphics>(sys->GetSystem("GraphicsSystem"));
    //Change to font shader
    Graphics::shader_iterator temp = graphics->GetShaderRefByKey("font_shader");
    shader_ref = temp;
}

void FontManager::Exit()
{
    glDeleteVertexArrays(1, &font_model.vaoid);
    glDeleteBuffers(1, &font_model.vboid);

    glDeleteTextures(1, &texture);
}

void FontManager::RenderAllText()
{
    
    for (auto& textData : _textData)
    {
        //have a check condition if to render/display the text or not.

        //std::string delimiter = "\n";
        //size_t pos = 0;
        //
        //while ((pos = textData.text->GetFontString().find(delimiter)) != std::string::npos) 
        //{
        //    std::string new_temp;
        //    new_temp = textData.text->GetFontString().substr(0, pos);

        //    FontComponent temp{ *textData.text };
        //    temp.SetFontString(new_temp);

        //    RenderText(&temp, textData.trans);
        //    textData.trans->Position(Vector2{ textData.trans->Position().x, textData.trans->Position().y - 5.f });

        //    std::string erase = textData.text->GetFontString().erase(0, pos + delimiter.length());
        //    textData.text->SetFontString(erase);
        //}

       RenderText(textData.text, textData.trans);


        //while (textData.text->GetFontString().find("\n"))
        //{
        //    ////Get text from start to end.
        //    //std::string start_till_newline;
        //    //FontComponent temp = textData.text;
        //    //temp.SetFontString(start_till_newline);
        //    //Transform temp = textData.trans;
        //    ////Increase y
        //    //RenderText(textData.text, textData.trans);

        //    std::string new_temp = textData.text->GetFontString().substr(0, textData.text->GetFontString().find("\n"));

        //}
     
    }
}

//DEBUG STUFF
void printMat4(glm::mat4 mat)
{
    std::cout << mat[0][0] << "\t " << mat[1][0] << "\t " << mat[2][0] << "\t " << mat[3][0] << std::endl;
    std::cout << mat[0][1] << "\t " << mat[1][1] << "\t " << mat[2][1] << "\t " << mat[3][1] << std::endl;
    std::cout << mat[0][2] << "\t " << mat[1][2] << "\t " << mat[2][2] << "\t " << mat[3][2] << std::endl;
    std::cout << mat[0][3] << "\t " << mat[1][3] << "\t " << mat[2][3] << "\t " << mat[3][3] << std::endl;
}

void printmat3(glm::mat3 mat)
{
    std::cout << mat[0][0] << "\t " << mat[1][0] << "\t " << mat[2][0] << std::endl;
    std::cout << mat[0][1] << "\t " << mat[1][1] << "\t " << mat[2][1] << std::endl;
    std::cout << mat[0][2] << "\t " << mat[1][2] << "\t " << mat[2][2] << std::endl;

}



void FontManager::RenderText(FontComponent* fontComp, Transform* trans)
{
    if (fontComp->IsActive() == false)
        return;

    GLSLShader& shader = shader_ref->second;
    // activate corresponding render state	
    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font_model.vaoid);


    //Get world to cam ndc transform and get the inverse of it
    Matrix3 m3Trans = trans->GetWorldtoCamxForm();
    Matrix3 newMat;
    float det;
    newMat.SetInverse(&det, m3Trans);

    glm::mat3 normCamMat = m3Trans.ConvertToGLMat3();

    Vector3 fontPos;
    fontPos.x = trans->Position().x + fontComp->posOffset().x;
    fontPos.y = trans->Position().y + fontComp->posOffset().y;
    fontPos.z = 1;

    float x, y;

    if (fontComp->isAnchored)
    {
        //get position of the world from the inverse of the cam
        // matrix
        fontPos = newMat * fontPos;
        x = fontPos.x;
        y = fontPos.y;
    }
    else
    {
        //Simply have the world position of the object for the font's pos
        x = fontPos.x;
        y = fontPos.y;
    }

    //Scale needs to fixed here 
    float scale = trans->Scale().x * fontComp->Scale() * 0.01f;

    Vector3 color = fontComp->GetFontClr();

    GLuint color_loc = glGetUniformLocation(shader.GetHandle(), "textColor");
    if (color_loc >= 0)
    {
        glUniform3f(color_loc, color.x, color.y, color.z);
    }
    else
    {
        std::cout << "Color_loc not found" << std::endl;
    }

    ////Test
    //glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1080), 0.0f, static_cast<float>(1920));

    //glUniformMatrix4fv(glGetUniformLocation(shader.GetHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    GLint uniform_var_loc2 = glGetUniformLocation(shader.GetHandle(), "wrld_to_NDC");
    if (uniform_var_loc2 >= 0)
    {
        glUniformMatrix3fv(uniform_var_loc2, 1, GL_FALSE,
            glm::value_ptr(normCamMat));
    }

    
    //float text_alpha = fontComp->alpha;
    float text_alpha = fontComp->alpha;

    GLuint alpha_loc = glGetUniformLocation(shader.GetHandle(), "alpha");
    if (alpha_loc >= 0)
    {
        glUniform1f(alpha_loc, text_alpha);
    }
    else
    {
        LogError("Font alpha not found");
    }

    // iterate through all characters
    std::string::const_iterator c;
    std::string text = fontComp->GetFontString();

    std::string delimiter = "\n";
    size_t pos = 0;
    std::vector<std::string> SplitStrings;

    while ((pos = text.find(delimiter)) != std::string::npos)
    {
        std::string new_temp;
        new_temp = text.substr(0, pos);

        SplitStrings.push_back(new_temp);

        std::string erase = text.erase(0, pos + delimiter.length());
        text = erase;
    } 
    SplitStrings.push_back(text);


    std::vector<FontVertex> vArray;

    float oldx = x, y_linespacing = 50;
    for (auto& i : SplitStrings)
    {
        x = oldx;
        for (c = i.begin(); c != i.end(); c++)
        {
            Character ch = Characters[*c];
            float xpos = x + ch.Bearing.x * scale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            float w = ch.Size.x * scale;
            float h = ch.Size.y * scale;
            // update VBO for each character

            vArray.push_back(FontVertex(xpos, ypos + h,     ch.startUV.x,   ch.startUV.y));
            vArray.push_back(FontVertex(xpos, ypos,         ch.startUV.x,   ch.endUV.y));
            vArray.push_back(FontVertex(xpos + w, ypos,     ch.endUV.x,     ch.endUV.y));
            vArray.push_back(FontVertex(xpos, ypos + h,     ch.startUV.x,   ch.startUV.y));
            vArray.push_back(FontVertex(xpos + w, ypos,     ch.endUV.x,     ch.endUV.y));
            vArray.push_back(FontVertex(xpos + w, ypos + h, ch.endUV.x,     ch.startUV.y));

            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        y -= y_linespacing * scale;

    }

    // update content of VBO memory

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, texture);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, font_model.vboid);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * vArray.size(), vArray.data(),GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FontVertex), 0);
    glEnableVertexAttribArray(0);

    // render quad
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vArray.size()));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader.UnUse();
    glDisable(GL_BLEND);
}

void FontManager::RenderText(std::string text, float posx, float posy, float _scale)
{
    GLSLShader& shader = shader_ref->second;
    // activate corresponding render state	
    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(font_model.vaoid);

    float x = posx;
    float y = posy;
    float scale = _scale;

    glm::vec3 color{ 1.0, 1.0, 1.0 };

    GLuint color_loc = glGetUniformLocation(shader.GetHandle(), "textColor");
    if (color_loc >= 0)
    {
        glUniform3f(color_loc, color.x, color.y, color.z);
    }
    else
    {
        std::cout << "Color_loc not found" << std::endl;
    }

    //Test
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(1080), 0.0f, static_cast<float>(1920));

    glUniformMatrix4fv(glGetUniformLocation(shader.GetHandle(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, font_model.vboid);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}



void FontManager::AddDataById(EntityId id)
{
    TextData temp;
    ComponentManager* CompManager = ECSEngine::GetComponentManager();
    IComponent* trans = CompManager->GetComponent(id, ComponentType::CT_Transform);
    IComponent* text = CompManager->GetComponent(id, ComponentType::CT_Font);

    if (trans == nullptr || text == nullptr)
        return;

    temp.trans = dynamic_cast<Transform*>(trans);
    temp.text = dynamic_cast<FontComponent*>(text);
    _textData.push_back(temp);
}


void FontManager::RemoveDataById(EntityId id)
{
    for (auto it = _textData.begin(); it != _textData.end(); ++it)
    {
        if (it->text->GetEntityId() != id)
            continue;

        //else if entity id matches
        _textData.erase(it);

        return;
    }
}

void FontManager::UpdateTransform(Matrix3 camMat)
{
    for (auto& textdata : _textData)
    {
        textdata.trans->ComputeTransform(camMat);
    }
}

