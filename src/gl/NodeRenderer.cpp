#include "gl/NodeRenderer.hpp"

#include "gl/Resources.hpp"

#include <utility>

BEGIN_PYRAMID_GL_NAMESPACE

NodeRenderer::NodeRenderer(
        glm::mat4 currentMatrix,
        std::shared_ptr<GraphLayout::Graph> _layout)
    : nodeVAO(),
    nodeTexture(TextureHandle::loadTextureFromPNG(Resources::getNodeTexturePath())),
    layout(_layout)
{
    matrixState.setCurrentMatrix(currentMatrix);
}

void NodeRenderer::draw()
{
    nodeVAO.bind();
    nodeTexture.bind();

    drawAllNodes();
}

void NodeRenderer::drawAllNodes()
{
    //TODO: draw consumer and distributor nodes differently

    GraphLayout::Graph::vertex_iterator vIt, vEnd;
    std::tie(vIt, vEnd) = boost::vertices(*layout);

    for(; vIt != vEnd; ++vIt)
    {
        const GraphLayout::Node& thisNode = (*layout)[*vIt];
        const GraphLayout::Point& thisPoint = thisNode.getPoint();

    }
}

END_PYRAMID_GL_NAMESPACE
