#include "GraphLayout.hpp"

#include <utility>

namespace pyramid_scheme_simulator {


void GraphLayout::Layout::applyCoulombsLaw()
{
    Graph::vertex_iterator i, iEnd;
    std::tie(i, iEnd) = boost::vertices(graph);

    for(; i != iEnd; ++i)
    {
        Graph::vertex_iterator j, jEnd;
        std::tie(j, jEnd) = boost::vertices(graph);

        for(; j != jEnd; ++j)
        {
            if(graph[*i].point != graph[*j].point)
            {

            }
        }
    }
}

}
