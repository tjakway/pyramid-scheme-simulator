#include "Config.hpp"
#include "Unique.hpp"
#include "Graph.hpp"
#include "Util.hpp"

#include <unordered_set>
#include <string>
#include <functional>
#include <array>
#include <utility>
#include <memory>
#include <iterator>
#include <exception>

namespace {
    using namespace pyramid_scheme_simulator;
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

        Unique getLt() const
        {
            if(v1.str() < v2.str())
                return v1;
            else
                return v2;
        }

        Unique getGt() const
        {
            if(v2.str() > v1.str())
                return v2;
            else
                return v1;
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
}

//see https://stackoverflow.com/questions/8157937/how-to-specialize-stdhashkeyoperator-for-user-defined-type-in-unordered
namespace std {
  template <> struct hash<UndirectedEdge>
  {
    size_t operator()(const UndirectedEdge & x) const
    {
        return hash<std::string>{}(x.getLt().str() + x.getGt().str());
    }
  };
}


namespace pyramid_scheme_simulator {

PopulationGraph* PopulationGraphGenerator::
    buildGraph(rd_ptr rd, Config& options)
{

    //check if link chance procs
    auto testEdge = [&rd, &options]() -> bool { 
        return Util::sampleFrom(rd, 
            options.graphGenerationOptions.linkChance.getOption()); };

    const auto graphSize = options.graphGenerationOptions.graphSize.getOption();

    std::unordered_set<UndirectedEdge> edges = std::unordered_set<UndirectedEdge>();
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
    
    //convert the set of undirected edges
    //to Consumer objects
    //auto consumers = std::<


}

}
