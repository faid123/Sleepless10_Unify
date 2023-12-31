//#version 430

#version 120
varying vec2 st;
uniform sampler2D tex;
uniform vec4 text_colour;

void main () {
  gl_FragColor = texture2D (tex, st) * text_colour;
}