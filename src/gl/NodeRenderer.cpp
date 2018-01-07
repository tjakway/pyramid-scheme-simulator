#include "gl/NodeRenderer.hpp"

#include "gl/Resources.hpp"
#include "gl/GLMatrix.hpp"

#include "Util/Util.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <utility>
#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

NodeRenderer::NodeRenderer(
        const GraphLayout::Graph& _layout)
    : nodeVAO(),
    nodeTexture(TextureHandle::loadTextureFromPNG(Resources::getNodeTexturePath())),
    layout(GraphLayout::Layout::copyGraph(_layout))
{}

void NodeRenderer::draw(const glm::mat4& initialMatrix) const
{
    nodeVAO.bind();
    nodeTexture.bind();

    drawAllNodes(initialMatrix);
}

void NodeRenderer::drawAllNodes(const glm::mat4& initialMatrix) const
{
    //TODO: draw consumer and distributor nodes differently

    GraphLayout::Graph::vertex_iterator vIt, vEnd;
    std::tie(vIt, vEnd) = boost::vertices(*layout);

    for(; vIt != vEnd; ++vIt)
    {
        const GraphLayout::Node& thisNode = (*layout)[*vIt];
        const GraphLayout::Point& thisPoint = thisNode.getPoint();

        glm::mat4 translatedMat = glm::translate(
                initialMatrix,
                glm::vec3(thisPoint.position.x,
                    thisPoint.position.y,
                    0.0));

        GLMatrix::updateMVPUniform(translatedMat);

        nodeVAO.draw();
    }
}

END_PYRAMID_GL_NAMESPACE
