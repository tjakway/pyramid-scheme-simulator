#include "gl/NodeRenderer.hpp"

#include "gl/Resources.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

NodeRenderer::NodeRenderer(
        std::shared_ptr<GraphLayout::Graph> _layout)
    : nodeVAO(),
    nodeTexture(TextureHandle::loadTextureFromPNG(Resources::getNodeTexturePath())),
    layout(_layout)
{}

void NodeRenderer::draw()
{
    nodeVAO.bind();
    nodeTexture.bind();

    drawAllNodes();
}

void NodeRenderer::drawAllNodes()
{
}

END_PYRAMID_GL_NAMESPACE
