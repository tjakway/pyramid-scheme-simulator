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
    const NodeVAO nodeVAO;
    const TextureHandle nodeTexture;

    const std::shared_ptr<GraphLayout::Graph> layout;

    void drawAllNodes();

public:
    NodeRenderer(std::shared_ptr<GraphLayout::Graph>);

    void draw();
};

END_PYRAMID_GL_NAMESPACE
