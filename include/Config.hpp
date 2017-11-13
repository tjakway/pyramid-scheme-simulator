#pragma once

#include <memory>
#include <random>
#include <functional>
#include <utility>

#include "Types.hpp"
#include "Unique.hpp"
#include "BoundedOption.hpp"

namespace pyramid_scheme_simulator {
/**
 * simulation parameters
 * see doc for explanation
 *
 * TODO: CTOR with preamble
 * TODO: for C++ random number distributions, see  C++ concepts: RandomNumberDistribution
 *  [numeric/random/*]
 */
class Config
{
//private internal variables
    const int randomSeed;

//public config values
public:

    const unsigned int productCost;

    const rd_ptr randomGen;

    //how are sales skills distributed throughout the population?
    //random guess: normal distribution?
    const std::function<double()> salesSkillDistribution;
    
    /**
     * Distribution of funds among the general population
     * reflecting “capitalization”: how much capital can someone wield, i.e. assets + loans
     * people whose capitalization is lower than:
     *  the amount needed to become consumers can’t become consumers
     *  the amount needed to become distributors can’t become distributors
    */
    //const ??? fundsDistribution;
    

    /**
     * function of capitalization to sales
     *  presumably if people are poorer they’re less likely to buy the product…
     *  but also test the reverse: if people are poorer they’re more likely to buy the product
     */
    //const ??? salesFunction;
    
    //how much does a distributor get from converting someone else into a distributor?
    //bounded to 99% because it doesn't make sense if a distributor gets all of the
    //earnings of a subdistributor
    const BoundedOption<double> downstreamPercent = 
        BoundedOption<double>(std::pair<double, double>(0.0, 0.99));
    

    //capitalization change per tick
    //const ??? capitalizationChange;


    class GraphGenerationOptions
    {
        const bool allowUnconnectedSubgraphs = true;

        const unsigned long graphSize;

        /**
         * how likely are people to have connections to other people?
         * AKA link chance
         */
        const PercentOption graphConnectedness;


        //corresponding to the maximum number of people someone
        //can maintain social relationships with
        //(hard to have 1000 best friends)
        const unsigned int maxEdgesPerNode;
    };
};



}
