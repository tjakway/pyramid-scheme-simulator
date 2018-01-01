#include "gl/GLContext.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

const std::string GLContext::vertexShaderSource = 
//C++11 raw string litearl
//see https://stackoverflow.com/questions/1135841/c-multiline-string-literal
R"RAW(
#version 420

in vec3 vertex_position;

void main() {
  gl_Position = vec4(vertex_position, 1.0);
}
)RAW";

const std::string GLContext::fragmentShaderSource = 
//note: gl_FragColor is deprecated, see https://www.opengl.org/discussion_boards/showthread.php/184313-where-has-built-in-variable-gl_FragColor-gone-in-OpenGL-4-4
R"RAW(
#version 420

out vec4 fragColour;

void main() {
  fragColour = vec4(1.0, 0.0, 0.0, 1.0);
}
)RAW";


END_PYRAMID_GL_NAMESPACE
