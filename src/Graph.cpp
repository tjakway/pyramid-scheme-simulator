#include "Config.hpp"
#include "Unique.hpp"
#include "Graph.hpp"
#include "Util.hpp"

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>
#include <algorithm>
#include <array>
#include <utility>
#include <memory>
#include <iterator>
#include <exception>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

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

PopulationGraph::BGLPopulationGraph 
    PopulationGraph::buildGraph(rd_ptr rd, Config& options)
{

    //check if link chance procs
    auto testEdge = [&rd, &options]() -> bool { 
        return Util::sampleFrom(rd, 
            options.graphGenerationOptions.linkChance.getOption()); };

    const auto graphSize = options.graphGenerationOptions.graphSize.getOption();

    std::unordered_set<UndirectedEdge> edges;
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
                edges.emplace(i, j);
            }
        }
    }

    //TODO: check for unconnected subgraphs
    

    //build the boost graph
    BGLPopulationGraph g;

    //map a Pop's Unique -> that Pop's vertex descriptor
    std::unordered_map<Unique, BGLPopulationGraph::vertex_descriptor> popDescriptors;

    auto insertPop = [&g, &popDescriptors](Pop p){
        //add it to the graph
        auto v = boost::add_vertex(p, g);
        //then use its id as the key
        popDescriptors.emplace(p->id, v);
    };

    auto startingFunds = options.simulationOptions.startingFunds;

    //add vertices to the boost graph and save the descriptors
    for(auto i : vertices)
    {
        insertPop(std::make_shared<CapitalHolder>(i, startingFunds()));
    }

    for(auto e : edges)
    {
        //get the vertex descriptors for this edge
        auto u = popDescriptors[e.v1];
        auto v = popDescriptors[e.v2];

        boost::add_edge(u, v, g);
    }
    
    return g;
}


std::vector<std::unordered_set<PopulationGraph::Pop>> 
    PopulationGraph::getDisconnectedSubgraphs(const BGLPopulationGraph& g)
{
    //see http://www.boost.org/doc/libs/1_53_0/libs/graph/example/connected_components.cpp
    //and http://www.boost.org/doc/libs/1_53_0/libs/graph/doc/connected_components.html
    std::vector<int> components(boost::num_vertices(g));
    int numSubgraphs = boost::connected_components(g, &components[0]);
    
    //initialize the vector of subgraphs and fill it with enough empty sets
    std::vector<std::unordered_set<Pop>> subgraphs(numSubgraphs);
    for(int i = 0; i < numSubgraphs; i++)
    {
        subgraphs.emplace_back();
    }


    //check that every vertex has been assigned to a subgraph
    assert(components.size() == boost::num_vertices(g));

    for(std::vector<int>::size_type i = 0;
            i < components.size(); ++i)
    {
        subgraphs[components[i]].insert(g[i]);
    }
    
    return subgraphs;
}

}
