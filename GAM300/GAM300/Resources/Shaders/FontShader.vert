//#version 430

#version 120
attribute vec2 vp;
attribute vec2 vt;
uniform vec2 pos;
uniform mat4 proj;
varying vec2 st;

void main () 
{
  st = vt;
  //gl_Position = vec4 (pos + vp, 0.0, 1.0);
  gl_Position = proj * vec4 (pos + vp, 0.0, 1.0);
}