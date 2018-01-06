#pragma once

#include "NamespaceDefines.hpp"

#include "gl/VAOHandle.hpp"
#include "gl/TextureHandle.hpp"
#include "gl/ShaderProgramHandle.hpp"
#include "gl/NodeVAO.hpp"

#include "gl/GraphLayout.hpp"
#include "gl/NodeRenderer.hpp"

#include "gl/MatrixState.hpp"

#include <glm/matrix.hpp>

#include <memory>
#include <utility>
#include <vector>
#include <deque>

BEGIN_PYRAMID_GL_NAMESPACE

class MainRenderer
{
    MatrixState matrixState;
    const NodeRenderer renderer;

    std::shared_ptr<GraphLayout::Graph> layout;

public:
    //TODO: pass relevant state
    MainRenderer(std::pair<std::unique_ptr<GraphLayout::Graph>, GraphLayout::BoundingBox>);

    void draw();
};

END_PYRAMID_GL_NAMESPACE
