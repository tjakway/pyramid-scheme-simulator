#include "Config.hpp"
#include "Util/Unique.hpp"
#include "PopulationGraph.hpp"
#include "Util/Util.hpp"
#include "Util/AssertWithMessage.hpp"
#include "Util/NewExceptionType.hpp"
#include "Util/Strcat.hpp"

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



namespace pyramid_scheme_simulator {

[[noreturn]] void PopulationGraph::throwVertexNotFoundException(const Unique& which)
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
    std::vector<Unique> vertices;

    vertices.reserve(graphSize);

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
            //don't allow edges from a vertex to itself
            if(i != j && testEdge())
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
                    config.defaults.mkConsumer(config, rd, i)));
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
    
    //boost::connected_components assigns every vertex an integer representing its subgraph
    std::vector<int> components;
    components.reserve(boost::num_vertices(g));

    auto numSubgraphs = boost::connected_components(g, &components[0]);

    //initialize the vector of subgraphs and fill it with enough empty sets
    //use a typedef because have to frequently reference ::size_type
    using SubgraphsT = std::vector<std::unordered_set<Pop>>;
    SubgraphsT subgraphs;

    if(numSubgraphs > 0) {
        //if its >0 we can safely cast to an unsigned size type
        subgraphs.reserve(static_cast<SubgraphsT::size_type>(numSubgraphs));

        //impossible to have negative subgraphs
    } else if(numSubgraphs < 0) {
        throw ImplementationException(STRCAT("Error in ", __func__, 
                    " call to boost::connected_components, numSubgraphs is < 0"));
    }
    
    //create empty sets for the subgraphs
    for(int i = 0; i < numSubgraphs; i++)
    {
        //TODO: use fill constructor
        subgraphs.emplace_back();
    }


    //check that every vertex has been assigned to a subgraph
    assert(components.size() == boost::num_vertices(g));

    //assign each vertex to an unordered_set based on the subgraph number it was given
    //by connected_components
    for(std::vector<int>::size_type i = 0;
            i < components.size(); ++i)
    {
        //check each connected component before casting to vector::size_type
        if(components[i] < 0) {
            throw ImplementationException(STRCAT("Error in ", __func__, 
                        " components[", i, "] is ", i));
        } else {
            subgraphs[static_cast<SubgraphsT::size_type>(components[i])]
                .insert(g[i]);
        }
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

PopulationGraph::vertices_size_type PopulationGraph::numVertices()
{
    return boost::num_vertices(graph);
}

PopulationGraph::edges_size_type PopulationGraph::numEdges()
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
    return PopulationGraph::mutateVerticesOfGraph(mutate, graph);
}

PopulationGraph::edges_size_type 
    PopulationGraph::mutateEdges(MutateEdgeFunction mutate)
{
    return PopulationGraph::mutateEdgesOfGraph(mutate, graph);
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
            : graphPtr(&g),
            pred(_pred)
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

PopulationGraph::edges_size_type
    PopulationGraph::mutateEdgesOfGraph(MutateEdgeFunction mutate, 
        BGLPopulationGraph& g)
{
    edges_size_type numMutated = 0;

    BGLPopulationGraph::edge_iterator ei, eiEnd;

    std::tie(ei, eiEnd) = boost::edges(g);

    for(; ei != eiEnd; ++ei)
    {
        BGLPopulationGraph::edge_descriptor ed = *ei;
        
        Pop s, t;
        std::tie(s, t) = mutate(getVerticesForEdge(ed, g));

        //get the vertex descriptors so we can mutate the vertices in the edge we're
        //currently modifying
        BGLPopulationGraph::vertex_descriptor vdS = boost::source(ed, g);
        BGLPopulationGraph::vertex_descriptor vdT = boost::target(ed, g);
        
        g[vdS] = s;
        g[vdT] = t;
        numMutated++;
    }

    return numMutated;
}

std::pair<PopulationGraph::Pop, PopulationGraph::Pop> 
    PopulationGraph::getVerticesForEdge(
        PopulationGraph::BGLPopulationGraph::edge_descriptor ed,
        BGLPopulationGraph& g)
{
    Pop source = g[boost::source(ed, g)];
    Pop target = g[boost::target(ed, g)];
    return std::make_pair(source, target);
}



PopulationGraph::edges_size_type
    PopulationGraph::mutateEdgesWithPredicate(
        MutateEdgeFunction mutate,
        EdgePredicate predicate)
{
    class EdgePredicateObject
    {
        BGLPopulationGraph* graphPtr;

        EdgePredicate pred;

    public:
        //boost filtered_graph docs say the predicate has to have a default constructor
        EdgePredicateObject() = default;

        EdgePredicateObject(
                BGLPopulationGraph* _graphPtr,
                EdgePredicate _pred
                )
            : graphPtr(_graphPtr),
              pred(_pred)
        {}

        bool operator()(const BGLPopulationGraph::edge_descriptor& ed) const {

            Pop s, t;
            std::tie(s, t) = PopulationGraph::getVerticesForEdge(ed, *graphPtr);

            //sanity check
            ASSERT_WITH_MESSAGE(s.get() != nullptr && t.get() != nullptr,
                    "The edges shouldn't have null pointers");

            //dereference the pointer and run the actual predicate
            return pred(*s, *t);
        }
    } predicateObject(&graph, predicate);

    //filter the graph using our predicate
    using FGraph = boost::filtered_graph<BGLPopulationGraph, EdgePredicateObject>;
    FGraph fgraph(graph, predicateObject);

    edges_size_type numMutated = 0;

    FGraph::edge_iterator ei, eiEnd;

    std::tie(ei, eiEnd) = boost::edges(fgraph);

    for(; ei != eiEnd; ++ei)
    {
        FGraph::edge_descriptor ed = *ei;
        
        Pop s, t;
        std::tie(s, t) = mutate(getVerticesForEdge(ed, graph));

        //get the vertex descriptors so we can mutate the vertices in the edge we're
        //currently modifying
        FGraph::vertex_descriptor vdS = boost::source(ed, graph);
        FGraph::vertex_descriptor vdT = boost::target(ed, graph);
        
        fgraph[vdS] = s;
        fgraph[vdT] = t;
        numMutated++;
    }

    return numMutated;
}


void PopulationGraph::auditGraph()
{
    NEW_EXCEPTION_TYPE(AuditGraphException);

    //audit iteration functions
    {
        //setup expected values and checks
        const auto expectedNumVertices = boost::num_vertices(graph);
        const auto expectedNumEdges = boost::num_edges(graph);

        BGLPopulationGraph::vertices_size_type numVertices = 0;
        BGLPopulationGraph::edges_size_type    numEdges = 0;


        /***vertex iteration checks***/
        const auto checkNumVertices = 
            [&numVertices, expectedNumVertices](std::string fName) {
                if(numVertices != expectedNumVertices)
                {
                    throw AuditGraphException(STRCAT(
                                "Graph failed audit because expected != actual in ", 
                                fName, " expected = ", expectedNumVertices, 
                                "actual = ", numVertices));
                }
        };

        const std::function<Pop(Pop)> incNumVertices = [&numVertices](Pop x)
            { numVertices++; return x; };

        //run vertex iteration functions and assert against the results
        forEachVertex(incNumVertices);
        checkNumVertices("forEachVertex");
        numVertices = 0;

        mutateVertices(incNumVertices);
        checkNumVertices("mutateVertices");
        numVertices = 0;

        mutateVerticesWithPredicate(incNumVertices, 
                //trivial predicate
                [](const CapitalHolder&) { return true; });
        checkNumVertices("mutateVerticesWithPredicate");
        numVertices=0;
        /*****************************/


        /***edge iteration checks*****/
        const auto checkNumEdges =
            [&numEdges, expectedNumEdges](std::string fName) {
                if(numEdges != expectedNumEdges) 
                {
                    throw AuditGraphException(STRCAT(
                                "Graph failed audit because expected != actual in ", 
                                fName, " expected = ", expectedNumEdges, 
                                "actual = ", numEdges));
                }
            };

        const std::function<std::pair<Pop, Pop>(std::pair<Pop, Pop>)> incNumEdges = 
            [&numEdges](std::pair<Pop, Pop> p)
            { numEdges++; return p; };

        //run edge iteration functions and assert against the results
        forEachEdge<std::pair<Pop, Pop>>(incNumEdges);
        checkNumEdges("forEachEdge");
        numEdges = 0;

        mutateEdges(incNumEdges);
        checkNumEdges("mutateEdges");
        numEdges = 0;

        mutateEdgesWithPredicate(incNumEdges, 
                //trivial predicate
                [](const CapitalHolder&, const CapitalHolder&) { return true; });
        checkNumEdges("mutateEdgesWithPredicate");
        numEdges=0;

        /*****************************/
    }
}


}
