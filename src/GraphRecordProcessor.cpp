#include "GraphRecordProcessor.hpp"
#include "Util/NewExceptionType.hpp"

BEGIN_PYRAMID_NAMESPACE

ConversionHandler::RecordType GraphRecordProcessor::applyConversions()
{
    const std::function<ConversionHandler::RecordType(std::pair<PopulationGraph::Pop, PopulationGraph::Pop>)> f = 
        [this](std::pair<PopulationGraph::Pop, PopulationGraph::Pop> pops) 
        -> ConversionHandler::RecordType {
        return this->getConversionHandler().operator()(this->when(), pops);
    };

    std::vector<ConversionHandler::RecordType> vecConversions = 
        getPopulationGraph()->forEachEdge(f);

    auto conversionRecs = 
        emptyListTransactionRecord<ConversionHandler::ElementType>().leftFold(
                std::list<ConversionHandler::RecordType>(
                    std::make_move_iterator(vecConversions.begin()),
                    std::make_move_iterator(vecConversions.end())),
                getConversionHandler().reduce);

    const auto numConversions = processConversions(conversionRecs);
    assert(numConversions == conversionRecs.records.size());
    return conversionRecs;
}

PopulationGraph::vertices_size_type 
    GraphRecordProcessor::processConversions(ConversionHandler::RecordType& recs)
{
    NEW_EXCEPTION_TYPE(ProcessConversionException);

    PopulationGraph::MutateVertexFunction mutate = 
        [&recs, this](PopulationGraph::Pop toConvert)
        {
            if(toConvert->isDistributor())
            {
                throw ProcessConversionException(STRCAT("Could not convert ",
                            toConvert->prettyPrint(), " because it is already a ",
                            "distributor"));
            }

            ConversionHandler::Conversion* thisConvertsRecord = 
                lookupConversionRecord(recs, toConvert->id);

            if(thisConvertsRecord == nullptr)
            {
                throw ProcessConversionException(
                        STRCAT("Could not find conversion record for ",
                            toConvert->prettyPrint(), 
                            " despite the predicate returning true"));
            }
            else
            {
                //look up the Distributor in the graph
                //TODO: may want to lift this into another method for synchronization reasons
                std::shared_ptr<CapitalHolder> distributor = 
                    getPopulationGraph()->findVertexPointerByUnique(thisConvertsRecord->convertedBy);

                if(!distributor)
                {
                    throw ProcessConversionException(STRCAT("Could not find the distributor ",
                                "that converted ", toConvert->prettyPrint(), ", distributor id=", 
                                thisConvertsRecord->convertedBy.prettyPrint()));
                }
                else
                {
                    //check after casting it
                    std::shared_ptr<Distributor> dPtr = std::dynamic_pointer_cast<Distributor>(distributor);
                    if(!distributor->isDistributor())
                    {
                        throw ProcessConversionException(STRCAT("distributor->isDistributor() returned false for ",
                                distributor->prettyPrint(), ", the distributor that converted ",
                                toConvert->prettyPrint()));
                    }
                    else if(dPtr == nullptr)
                    {
                        throw ProcessConversionException(STRCAT("Expected a distributor but could not cast ",
                                distributor->prettyPrint(), ", the distributor that converted ",
                                toConvert->prettyPrint()));
                    }
                    else
                    {
                        //throw an exception on error rather than sigsegv
                        return dynamic_cast<Consumer&>(*toConvert)
                            .becomeDistributor(dPtr);
                    }
                }
            }
        };

    PopulationGraph::VertexPredicate predicate = 
        [&recs](const CapitalHolder& x)
        {
            //filter for any CapitalHolder's we have conversion records for
            return lookupConversionRecord(recs, x.id) != nullptr;
        };

    return getPopulationGraph()->mutateVerticesWithPredicate(mutate, predicate);
}

ConversionHandler::Conversion* GraphRecordProcessor::lookupConversionRecord(
        ConversionHandler::RecordType& recs,
        const Unique possibleConvertId)
{
    using RecIt = ConversionHandler::RecordType::ContainerType::iterator;
    RecIt res = std::find_if(recs.records.begin(), recs.records.end(),
            [possibleConvertId](const std::unique_ptr<ConversionHandler::ElementType>& x){
                return x->id == possibleConvertId;
            });

    if(res == recs.records.end())
    {
        return nullptr;
    }
    else
    {
        return (*res).get();
    }
}

END_PYRAMID_NAMESPACE
