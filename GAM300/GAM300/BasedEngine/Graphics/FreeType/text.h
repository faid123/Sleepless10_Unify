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

#ifndef _TEXT_H_
#define _TEXT_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Renderable_Text {
	GLuint points_vbo, texcoords_vbo;
	GLuint vao;

	// top-left screen space coords
	float tl_x, tl_y;

	// bottom-right screen space coords. useful for background box sizing
	float br_x, br_y;
	float size_px;
	float r, g, b, a;
	int point_count;
	bool visible;
};

// call before main loop to load shaders and stuff
bool init_text_rendering (
	const char* font_image_file,
	const char* font_meta_data_file,
	int viewport_width,
	int viewport_height
);

//
// add a string of text to render on-screen
// returns an integer to identify it with later if we want to change the text
// returns <0 on error
// x,y are position of the bottom-left of the first character in clip space
// size_is_px is the size of maximum-sized glyph in pixels on screen
// r, g, b, a is the colour of the text string
int add_text (
	const char* str,
	float x,
	float y,
	float size_in_px,
	float r,
	float g,
	float b,
	float a
);

void init_texts();

void destroy_text();

void free_text();

// x,y are position of the bottom-left of the first character in clip space
void move_text (int id, float x, float y);
void centre_text (int id, float x, float y);

// change text string in any previously added text
bool update_text (int id, const char* str);

// change colour of any previously added text
bool change_text_colour (int id, float r, float g, float b, float a);

// draw all the texts to screen //proj mat4 from graphics
void draw_texts (float* proj);

#endif
