#include "gl/MainRenderer.hpp"

#include "gl/ShaderDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

const std::string MainRenderer::vertexShaderSource = 
//C++11 raw string litearl
//see https://stackoverflow.com/questions/1135841/c-multiline-string-literal
R"RAW(
#version 420

layout(location = VS_MVP_LOC) uniform mat4 mvp_matrix;

layout(location = VS_POSITION_LOC) in vec3 in_vertex_position;
layout(location = VS_TEXCOORD_LOC) in vec2 in_tex_coords;
layout(location = VS_COLOR_LOC) in vec4 in_colors;

out vec2 frag_tex_coords;
out vec4 frag_colors;

void main() {
  gl_Position = vec4(vertex_position, 1.0);

  frag_tex_coords = in_tex_coords;
  frag_colors = in_colors;
}
)RAW";

const std::string MainRenderer::fragmentShaderSource = 
//note: gl_FragColor is deprecated, see https://www.opengl.org/discussion_boards/showthread.php/184313-where-has-built-in-variable-gl_FragColor-gone-in-OpenGL-4-4
R"RAW(
#version 420

layout(location = FS_TEX_LOC) uniform sampler2D tex_sampler;

in vec2 frag_tex_coords;
in vec4 frag_colors;

out vec4 colour;

void main() {
    color = texture(tex_sampler, frag_tex_coords) * frag_colors;
}
)RAW";


END_PYRAMID_GL_NAMESPACE
