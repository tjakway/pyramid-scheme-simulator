#pragma once

#include "NamespaceDefines.hpp"
#include "HasTime.hpp"
#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"

BEGIN_PYRAMID_NAMESPACE

class GraphRecordProcessor : protected HasTime
{
    PopulationGraph::vertices_size_type 
        processConversions(ConversionHandler::RecordType&);
protected:
    /**
     * used to get a non-owned pointer to mutate and observe the graph
     * TODO: need to add a lock method to this class if we ever implement multithreaded access to the graph
     */
    virtual PopulationGraph* getPopulationGraph() = 0;
    virtual const ConversionHandler& getConversionHandler() = 0;
    virtual const RestockHandler& getRestockHandler() = 0;

    ConversionHandler::RecordType applyConversions();

    static ConversionHandler::Conversion* lookupConversionRecord(
            ConversionHandler::RecordType&,
            Unique);
};

END_PYRAMID_NAMESPACE
