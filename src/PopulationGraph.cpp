#include "Config.hpp"
#include "Util/Unique.hpp"
#include "PopulationGraph.hpp"
#include "Util/Util.hpp"
#include "Util/AssertWithMessage.hpp"

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
#include <tuple>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/copy.hpp>

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
            const Unique* begin = vertices;
            const Unique* end = begin + length;

            auto checkFind = [=](const Unique* const which, const Unique* res) { 
                if(res == end) {
                    PopulationGraph::throwVertexNotFoundException(*which);
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

void PopulationGraph::throwVertexNotFoundException(const Unique& which)
{
    std::ostringstream os;
    os << "Could not find the index of vertex " << which << std::endl;
    throw PopulationGraph::VertexNotFoundException(os.str());
}


PopulationGraph::BGLPopulationGraph 
    PopulationGraph::buildGraph(rd_ptr rd, Config& config)
{

    //check if link chance procs
    auto testEdge = [&rd, &config]() -> bool { 
        return Util::sampleFrom(rd, 
            config.graphGenerationOptions->linkChance->getOption()); };

    const auto graphSize = config.graphGenerationOptions->graphSize.getOption();

    std::unordered_set<UndirectedEdge> edges;
    std::vector<Unique> vertices(graphSize);
    
    //generate vertices with random ids
    for(unsigned long i = 0; i < graphSize; i++)
    {
        vertices.push_back(Unique::newUnique());
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

    auto startingFunds = config.simulationOptions->startingFunds;

    //add vertices to the boost graph and save the descriptors
    for(auto i : vertices)
    {
        insertPop(std::shared_ptr<CapitalHolder>(
                    config.defaults->mkConsumer(rd, i)));
    }

    //add edges using the saved vertex descriptors
    for(auto e : edges)
    {
        //get the vertex descriptors for this edge
        auto u = popDescriptors[e.v1];
        auto v = popDescriptors[e.v2];

        boost::add_edge(u, v, g);
    }

    //TODO: check for disconnected subgraphs
    
    return g;
}


std::vector<std::unordered_set<PopulationGraph::Pop>> 
    PopulationGraph::getDisconnectedSubgraphs(const BGLPopulationGraph& g)
{
    //see http://www.boost.org/doc/libs/1_53_0/libs/graph/example/connected_components.cpp
    //and http://www.boost.org/doc/libs/1_53_0/libs/graph/doc/connected_components.html
    std::vector<int> components(boost::num_vertices(g));
    auto numSubgraphs = boost::connected_components(g, &components[0]);
    
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

PopulationGraph::PopulationGraph(Config& config)
{
    graph = buildGraph(config.randomGen, config);
}

PopulationGraph::PopulationGraph(const BGLPopulationGraph& otherGraph)
{
    boost::copy_graph(otherGraph, graph);
}


PopulationGraph::BGLPopulationGraph PopulationGraph::graphFromTuples(
        std::vector<std::pair<Pop, Pop>> tuples)
{
    BGLPopulationGraph g;

    std::unordered_map<Unique, BGLPopulationGraph::vertex_descriptor> popDescriptors;


    //either insert a vertex or return the existing vertex's vertex_descriptor
    const auto insertIfNeeded = [&g, &popDescriptors](Pop& p) {
        const Unique popId = p->id;

        //check if we've already inserted this vertex
        //see http://www.cplusplus.com/reference/unordered_map/unordered_map/count/

        if(popDescriptors.count(popId) == 0) {
            auto vd = boost::add_vertex(p, g);
            //insert the descriptor into the map
            popDescriptors.emplace(popId, vd);

            return vd;
        }
        else {
            return popDescriptors.at(popId);
        }
    };

    for(auto t : tuples)
    {
        auto left = t.first;
        auto right = t.second;

        //get or create the vertex descriptors corresponding to these Pops
        auto leftVd = insertIfNeeded(left);
        auto rightVd = insertIfNeeded(right);

        //use them to create an edge in the graph
        boost::add_edge(leftVd, rightVd, g);
    }

    return g;
}

int PopulationGraph::numVertices()
{
    return boost::num_vertices(graph);
}

int PopulationGraph::numEdges()
{
    return boost::num_edges(graph);
}


std::vector<PopulationGraph::Pop> PopulationGraph::vertices()
{
    //copy all shared_ptr objects into a vector and return it
    std::vector<Pop> allPops;
    allPops.reserve(boost::num_vertices(graph));

    BGLPopulationGraph::vertex_iterator vd, end;
    std::tie(vd, end) = boost::vertices(graph);

    for(; vd != end; ++vd)
    {
        allPops.push_back(graph[*vd]);
    }

    return allPops;
}


CapitalHolder& PopulationGraph::findVertexByUnique(const Unique& vert)
{
    BGLPopulationGraph::vertex_iterator begin, end;
    std::tie(begin, end) = boost::vertices(graph);

    //since we're searching for a UUID that has no relation to graph structure
    //there's no reason to use a graph-specific search function
    auto res = std::find_if(begin, end, 
        [&vert, this](BGLPopulationGraph::vertex_descriptor vd){
            return this->graph[vd]->id == vert;
        });

    if(res == end)
    {
        throwVertexNotFoundException(vert);
    }
    else
    {
        auto vd = *res;
        Pop foundVert = graph[vd];
        return *foundVert;
    }
}

/**
 * can't get this to work with templates
 */
PopulationGraph::vertices_size_type 
    PopulationGraph::mutateVerticesOfGraph(MutateVertexFunction mutate, 
        BGLPopulationGraph& g)
{
    //counter
    vertices_size_type numMutated = 0;

    BGLPopulationGraph::vertex_iterator vi, viEnd;

    std::tie(vi, viEnd) = boost::vertices(g);

    for(; vi != viEnd; ++vi)
    {
        BGLPopulationGraph::vertex_descriptor vd = *vi;

        //the proper way to access the graph using bundled properties
        //is the [] operator
        //see https://stackoverflow.com/questions/28740974/boost-graph-accessing-properties-through-vertex-descriptor
        g[vd] = mutate(g[vd]);

        numMutated++;
    }

    return numMutated;
}

PopulationGraph::vertices_size_type 
    PopulationGraph::mutateVertices(MutateVertexFunction mutate)
{
    PopulationGraph::mutateVerticesOfGraph(mutate, graph);
}

PopulationGraph::vertices_size_type
    PopulationGraph::mutateVerticesWithPredicate(
        MutateVertexFunction mutate,
        VertexPredicate predicate)
{
    /**
     * orchestrates state to call the VertexPredicate function
     * (i.e. translating between the boost::graph representation
     * of our vertices and CapitalHolder&)
     */
    class VertexPredicateObject
    {
        //TODO: should this be a shared_ptr...?
        //seems like overkill since this is a pointer to a field
        //within the same object this method is running in...
        BGLPopulationGraph* graphPtr = nullptr;

        VertexPredicate pred;

    public:
        //boost filtered_graph docs say the predicate has to have a default constructor
        VertexPredicateObject() = default;

        VertexPredicateObject(
                BGLPopulationGraph& g,
                VertexPredicate _pred
                )
            : pred(_pred), 
             graphPtr(&g)
        {}

        /**
         * the docs explicitly say vertex and edge descriptors in the 
         * filtered_graph are valid for the original graph object
         */
        bool operator()(const BGLPopulationGraph::vertex_descriptor& vd) const {
            Pop thisVertex = (*graphPtr)[vd];

            //sanity check
            ASSERT_WITH_MESSAGE(thisVertex.get() != nullptr,
                    "The vertices shouldn't have null pointers");

            //dereference the pointer and run the actual predicate
            return pred(*thisVertex);
        }
    } predicateObject(graph, predicate);

    //filter the graph using our predicate
    using FGraph = boost::filtered_graph<BGLPopulationGraph, VertexPredicateObject>;
    FGraph fgraph(graph, predicateObject);

    //***see refactor note at top of mutateVerticesOfGraph***
    vertices_size_type numMutated = 0;

    FGraph::vertex_iterator vi, viEnd;

    std::tie(vi, viEnd) = boost::vertices(fgraph);

    for(; vi != viEnd; ++vi)
    {
        FGraph::vertex_descriptor vd = *vi;

        //the proper way to access the graph using bundled properties
        //is the [] operator
        //see https://stackoverflow.com/questions/28740974/boost-graph-accessing-properties-through-vertex-descriptor
        fgraph[vd] = mutate(fgraph[vd]);
        numMutated++;
    }

    return numMutated;
}


}
