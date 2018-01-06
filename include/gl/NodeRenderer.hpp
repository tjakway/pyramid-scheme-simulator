#pragma once

#include "NamespaceDefines.hpp"

#include "gl/VAOHandle.hpp"
#include "gl/TextureHandle.hpp"
#include "gl/ShaderProgramHandle.hpp"
#include "gl/NodeVAO.hpp"
#include "gl/GraphLayout.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

class NodeRenderer
{
    NodeVAO nodeVAO;
    TextureHandle nodeTexture;

    std::shared_ptr<GraphLayout::Graph> layout;

public:
    NodeRenderer(std::shared_ptr<GraphLayout::Graph>);

    void draw();
};

END_PYRAMID_GL_NAMESPACE
