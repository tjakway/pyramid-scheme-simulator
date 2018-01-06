#pragma once

#include "NamespaceDefines.hpp"

#include "gl/VAOHandle.hpp"
#include "gl/TextureHandle.hpp"
#include "gl/ShaderProgramHandle.hpp"
#include "gl/NodeVAO.hpp"

#include "gl/GraphLayout.hpp"
#include "gl/NodeRenderer.hpp"

#include "glm/matrix.hpp"

#include <memory>
#include <utility>
#include <vector>
#include <deque>

BEGIN_PYRAMID_GL_NAMESPACE

class MainRenderer
{
    class MatrixState
    {
        void sizeCheck() const;
        std::deque<glm::mat4> matrixStack;
    public:
        MatrixState();

        glm::mat4 getCurrentMatrix() const;
        void setCurrentMatrix(const glm::mat4&);

        glm::mat4 popMatrix();
        void pushMatrix(const glm::mat4&);
    };

    const NodeRenderer renderer;

    std::shared_ptr<GraphLayout::Graph> layout;

public:
    //TODO: pass relevant state
    MainRenderer(std::pair<std::unique_ptr<GraphLayout::Graph>, GraphLayout::BoundingBox>);

    void draw();
};

END_PYRAMID_GL_NAMESPACE
