#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include <functional>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "Config.hpp"
#include "CapitalHolder.hpp"
#include "Util/NewExceptionType.hpp"

namespace pyramid_scheme_simulator {



class PopulationGraph
{
public:
    /** Type synonyms */
    //vertex type
    using Pop = std::shared_ptr<CapitalHolder>;
    using PopEdge = std::pair<Pop, Pop>;

    using BGLPopulationGraph = 
        boost::adjacency_list<
            boost::vecS,
            boost::vecS, 
            boost::undirectedS,
            Pop,
            PopEdge>;

    using PopDescriptor = 
        boost::graph_traits<BGLPopulationGraph>::vertex_descriptor;

    using vertices_size_type = BGLPopulationGraph::vertices_size_type;
    using edges_size_type    = BGLPopulationGraph::edges_size_type;

    /** Exception types */

    NEW_EXCEPTION_TYPE(PopulationGraphException);
    NEW_EXCEPTION_TYPE_WITH_BASE(VertexNotFoundException, PopulationGraphException);
    //for internal errors AKA sanity checks
    NEW_EXCEPTION_TYPE_WITH_BASE(ImplementationException, PopulationGraphException);
    
    [[noreturn]] static void throwVertexNotFoundException(const Unique&);

private:
    /*****************/

    //TODO: *maybe* make this a GraphGenerationOptions field
    unsigned int maxTries;

    bool checkGraph(const BGLPopulationGraph&);

    BGLPopulationGraph graph;


    /**
     * std::vector<std::unordered_set<Pop>> maps the vector index to the 
     * subgraph containing those vertices
     * so:
     *
     * auto ret = getDisconnectedSubgraphs(g);
     * ret.size() // <-- number of disconnected subgraphs
     *            //     (1 if the whole graph is connected)
     * ret[0]     // <-- subgraph #1
     * ret[1]     // <-- subgraph #2
     *            // and so on
     *
     * ******************************************
     *
     * should handle this either by retrying graph generation or by generating edges
     * between the disconnected subgraphs
     */
    std::vector<std::unordered_set<Pop>> getDisconnectedSubgraphs(const BGLPopulationGraph&);

    /**
     * TODO:
     * need this step to create Distributors during graph generation
     *  -need to check that all disconnected subgraphs have at least 1 distributor
     */
    BGLPopulationGraph applyOnlyInitialOnboardingTransformation(BGLPopulationGraph&);

    BGLPopulationGraph buildGraph(rd_ptr, Config&);

protected:
    PopulationGraph(const BGLPopulationGraph&);

    static BGLPopulationGraph graphFromTuples(
            std::vector<std::pair<Pop, Pop>>);

public:
    template <class T> std::vector<T> forEachEdge(std::function<T(Pop, Pop)> f)
    {
        std::vector<T> results;

        //see http://www.boost.org/doc/libs/1_65_1/libs/graph/doc/quick_tour.html
        boost::graph_traits<BGLPopulationGraph>::edge_iterator ei, ei_end;

        for (std::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
        {
            auto arg1 = source(*ei, graph);
            auto arg2 = target(*ei, graph);

            results.push_back(f(arg1, arg2));
        }

        return results;
    }

    PopulationGraph(Config&);

    vertices_size_type numVertices();
    edges_size_type numEdges();

    std::vector<Pop> vertices();

    CapitalHolder& findVertexByUnique(const Unique&);

    using VertexPredicate = std::function<bool(const CapitalHolder&)>;
    using MutateVertexFunction = 
        std::function<const std::shared_ptr<CapitalHolder>(std::shared_ptr<CapitalHolder>)>;

protected:
    /**
     * returns the number of vertices mutated
     */
    static vertices_size_type mutateVerticesOfGraph(MutateVertexFunction, 
            BGLPopulationGraph&);

public:
    /**
     * returns the number of vertices mutated
     * 
     * NOTE: if the caller is only modifying the vertex and not replacing its
     * object with another instance of CapitalHolder then the MutateVertexFunction
     * can just return the shared_ptr it was passed
     *
     * (thin layer over mutateVerticesOfGraph that just passes our graph)
     */
    vertices_size_type mutateVertices(MutateVertexFunction);

    /**
     * filters the graph then mutates it
     * more efficiently than trying to filter in the function passed to mutateVertices
     * returns the number of vertices mutated
     */
    vertices_size_type mutateVerticesWithPredicate(MutateVertexFunction, VertexPredicate);
};


} //pyramid_scheme_simulator
