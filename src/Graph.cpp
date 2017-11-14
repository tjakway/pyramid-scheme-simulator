#include "Config.hpp"
#include "Unique.hpp"
#include "Graph.hpp"

#include <set>
#include <array>
#include <utility>

namespace pyramid_scheme_simulator {

PopulationGraph* PopulationGraphGenerator::
    buildGraph(rd_ptr rd, const Config::GraphGenerationOptions& options)
{
    struct UndirectedEdge
    {
        Unique v1, v2;

        bool operator==(const Edge& other) const
        {
            //since it's undirected it's equivalent
            //if the vertices are swapped
            return (v1 == other.v1 && v2 == other.v2) ||
                (v1 == other.v2 && v2 == other.v1);
        }

        bool operator!=(const Edge& other) const
        {
            return !this->operator==(other);
        }

        std::pair<int, int> toIndicesPair(std::array<Unique> vertices) const {
            auto firstElem = std::find(vertices.begin(), vertices.end(), v1);
            auto secondElem = std::find(vertices.begin(), vertices.end(), v2);
            return std::make_pair(*firstElem, *secondElem);
        }
    };

    //check if link chance procs
    auto testEdge = [rd]() { return Util::sampleFrom(rd, options.linkChance); }

    std::set<UndirectedEdge> edges;
    std::array<Unique> vertices;
    
    //generate vertices with random ids
    for(unsigned long i = 0; i < options.graphSize; i++)
    {
        vertices.insert(newUnique());
    }

    for(auto i : vertices)
    {
        for(auto j : vertices)
        {
            if(testEdge())
            {
                edges.insert({ i, j });
            }
        }
    }


}

}
