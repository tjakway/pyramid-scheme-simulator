#include "Config.hpp"
#include "Unique.hpp"
#include "Graph.hpp"

#include <set>
#include <array>
#include <utility>
#include <memory>
#include <iterator>
#include <exception>

namespace pyramid_scheme_simulator {

PopulationGraph* PopulationGraphGenerator::
    buildGraph(rd_ptr rd, const Config::GraphGenerationOptions& options)
{
    struct UndirectedEdge
    {
        const Unique v1, v2;

        bool operator==(const UndirectedEdge& other) const
        {
            //since it's undirected it's equivalent
            //if the vertices are swapped
            return (v1 == other.v1 && v2 == other.v2) ||
                (v1 == other.v2 && v2 == other.v1);
        }

        bool operator!=(const UndirectedEdge& other) const
        {
            return !this->operator==(other);
        }

        std::pair<int, int> toIndicesPair(Unique* vertices, int length) const {
            //sanity check--should never happen
            class VertexNotFoundException : public std::exception {
                const Unique* which;
            public:
                VertexNotFoundException(const Unique* which) : which(which) {}

                const char* what() const throw() override {
                    std::string msg = "Could not find the index of vertex " 
                        + std::string(*which);
                    return msg.c_str();
                }
            };

            const Unique* begin = vertices;
            const Unique* end = begin + length;

            auto checkFind = [=](const Unique* which, const Unique* res) { 
                if(res == end) {
                    throw VertexNotFoundException(which);
                }
            };
            
            auto firstElem = std::find(begin, end, v1);
            checkFind(&v1, firstElem);
            auto secondElem = std::find(begin, end, v2);
            checkFind(&v2, secondElem);

            return std::make_pair(firstElem - begin, secondElem - begin);
        }
    };

    //check if link chance procs
    auto testEdge = [rd]() -> bool { return Util::sampleFrom(rd, options.linkChance); }

    std::set<UndirectedEdge> edges;
    std::unique_ptr<Unique> vertices;
    
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
