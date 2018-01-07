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

    void drawAllNodes(const glm::mat4& initialMatrix) const;

public:
    NodeRenderer(
        const GraphLayout::Graph&);

    void draw(const glm::mat4&) const;
};

END_PYRAMID_GL_NAMESPACE
