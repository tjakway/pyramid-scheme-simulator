#include "Config.hpp"
#include "Unique.hpp"
#include "Graph.hpp"
#include "Util.hpp"

#include <set>
#include <array>
#include <utility>
#include <memory>
#include <iterator>
#include <exception>

namespace pyramid_scheme_simulator {

PopulationGraph* PopulationGraphGenerator::
    buildGraph(rd_ptr rd, Config::GraphGenerationOptions& options)
{
    struct UndirectedEdge
    {
        const Unique v1, v2;

        UndirectedEdge(Unique a, Unique b) : v1(a), v2(b) {}

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

        bool operator<(const UndirectedEdge& other) const
        {
            //if other is the same object then we're not less than
            if(this->operator==(other)) {
                return false;
            }

            bool firstEq = v1 == other.v1;
            if(!firstEq) {
                return v1.str() < other.v1.str();
            }
            //check the second members if the first are equal
            else {
                return v2.str() < other.v2.str();
            }
        }

        //implemented in terms of < and ==
        bool operator<=(const UndirectedEdge& other) const
        {
            return this->operator==(other) || this->operator<(other);
        }

        bool operator>(const UndirectedEdge& other) const
        {
            //greater than means not less than and not equal to
            return (!this->operator==(other)) && (!this->operator<(other));
        }

        //implemented in terms of > and ==
        bool operator>=(const UndirectedEdge& other) const
        {
            return this->operator==(other) || this->operator>(other);
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
    auto testEdge = [&rd, &options]() -> bool { 
        return Util::sampleFrom(rd, 
            options.linkChance.getOption()); };

    const auto graphSize = options.graphSize.getOption();

    std::set<UndirectedEdge> edges;
    std::vector<Unique> vertices(graphSize);
    
    //generate vertices with random ids
    for(unsigned long i = 0; i < graphSize; i++)
    {
        vertices.push_back(newUnique());
    }

    //generate edges
    for(auto i : vertices)
    {
        for(auto j : vertices)
        {
            if(testEdge())
            {
                edges.insert(UndirectedEdge(i, j));
            }
        }
    }

    //TODO: check for unconnected subgraphs
    
    //

}

}
