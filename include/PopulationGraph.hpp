#pragma once

#include <vector>
#include <unordered_set>
#include <memory>
#include <functional>
#include <utility>
#include <algorithm>
#include <string>

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

    /*****************/
protected:
    BGLPopulationGraph graph;

private:

    //TODO: *maybe* make this a GraphGenerationOptions field
    unsigned int maxTries;

    bool checkGraph(const BGLPopulationGraph&);


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

    static std::pair<Pop, Pop> getVerticesForEdge(BGLPopulationGraph::edge_descriptor, 
            BGLPopulationGraph&);

public:
    template <typename T>
    std::vector<T> forEachEdge(std::function<T(std::pair<Pop, Pop>)> f)
    {
        //see http://www.boost.org/doc/libs/1_65_1/libs/graph/doc/quick_tour.html
        boost::graph_traits<BGLPopulationGraph>::edge_iterator ei, ei_end;

        std::tie(ei, ei_end) = boost::edges(graph);
        auto& g = graph;

        return Util::accumulateWithVector<std::function<T(BGLPopulationGraph::edge_descriptor)>,
               T,
               BGLPopulationGraph::edge_iterator
               >
            (ei, ei_end, 
                [&f, &g](BGLPopulationGraph::edge_descriptor e) -> T {

                    return f(getVerticesForEdge(e, g));
                });
    }

    
    template <typename T> 
    std::vector<T> forEachVertex(std::function<T(Pop)> f)
    {
        boost::graph_traits<BGLPopulationGraph>::vertex_iterator vi, vi_end;

        std::tie(vi, vi_end) = boost::vertices(graph);
        auto& g = graph;

        return Util::accumulateWithVector<
            std::function<T(BGLPopulationGraph::vertex_descriptor)>, 
               T, 
               BGLPopulationGraph::vertex_iterator>
            (vi, vi_end, 
                [&f, &g](BGLPopulationGraph::vertex_descriptor v){
                    return f(g[v]);
                });
    }
    

    PopulationGraph(Config&);

    vertices_size_type numVertices();
    edges_size_type numEdges();

    std::vector<Pop> vertices();

    CapitalHolder& findVertexByUnique(const Unique&);

    /** TODO: use templates to reduce duplication between edge and vertex functions */
    using VertexPredicate = std::function<bool(const CapitalHolder&)>;
    using MutateVertexFunction = 
        std::function<const std::shared_ptr<CapitalHolder>(std::shared_ptr<CapitalHolder>)>;

    using EdgePredicate = std::function<bool(
            const CapitalHolder&, const CapitalHolder&)>;
    using MutateEdgeFunction = 
        std::function<const std::pair<const Pop, const Pop>(std::pair<Pop, Pop>)>;
protected:

    /**
     * returns the number of vertices mutated
     */
    static vertices_size_type mutateVerticesOfGraph(MutateVertexFunction, 
            BGLPopulationGraph&);

    static edges_size_type mutateEdgesOfGraph(MutateEdgeFunction,
            BGLPopulationGraph&);


    void auditGraph();

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
     * returns the number of edges mutated
     * ***see above***
     */
    edges_size_type mutateEdges(MutateEdgeFunction);

    /**
     * filters the graph then mutates it
     * more efficiently than trying to filter in the function passed to mutateVertices
     * returns the number of vertices mutated
     */
    vertices_size_type mutateVerticesWithPredicate(MutateVertexFunction, VertexPredicate);

    edges_size_type mutateEdgesWithPredicate(MutateEdgeFunction, EdgePredicate);

protected:
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

    template <typename T> friend struct std::hash;
};

} //pyramid_scheme_simulator



//see https://stackoverflow.com/questions/8157937/how-to-specialize-stdhashkeyoperator-for-user-defined-type-in-unordered
namespace std {
  template <> struct hash<pyramid_scheme_simulator::PopulationGraph::UndirectedEdge>
  {
    size_t operator()(const pyramid_scheme_simulator::PopulationGraph::UndirectedEdge & x) const
    {
        return hash<std::string>{}(x.getLt().str() + x.getGt().str());
    }
  };
}
