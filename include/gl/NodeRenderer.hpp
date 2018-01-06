#pragma once

#include "NamespaceDefines.hpp"

#include "gl/VAOHandle.hpp"
#include "gl/TextureHandle.hpp"
#include "gl/ShaderProgramHandle.hpp"
#include "gl/NodeVAO.hpp"
#include "gl/GraphLayout.hpp"
#include "gl/MatrixState.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

class NodeRenderer
{
    const NodeVAO nodeVAO;
    const TextureHandle nodeTexture;
    const std::shared_ptr<GraphLayout::Graph> layout;

    const glm::mat4 initialMatrix;

    void drawAllNodes();

public:
    NodeRenderer(glm::mat4, std::shared_ptr<GraphLayout::Graph>);

    void draw();
};

END_PYRAMID_GL_NAMESPACE
