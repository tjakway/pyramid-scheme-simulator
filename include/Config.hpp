#pragma once

#include <memory>
#include <random>
#include <functional>
#include <utility>
#include <limits>

#include "Types.hpp"
#include "CapitalHolderClassDecls.hpp"
#include "Util.hpp"
#include "Tick.hpp"
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

    const rd_ptr randomGen;

    class SimulationOptions
    {
    public:
        const SimulationTick maxTicks;

        /**
         * chance of a Consumer spontaneously becoming a Distributor
         * (i.e. being recruited directly by the company)
         */
        const double onboardingChance;
        const unsigned int productCost;


        const std::function<Money()> startingFunds;

        class DistributorOptions
        {
            //how much does a distributor get from converting someone else 
            //into a distributor?
            //bounded to 99% because it doesn't make sense 
            //if a distributor gets all of the
            //earnings of a subdistributor
            BoundedOption<double> downstreamPercent = 
                BoundedOption<double>(std::pair<double, double>(0.0, 0.99));
        } distributorOptions;
    } simulationOptions;
    

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
    
    

    //capitalization change per tick
    //const ??? capitalizationChange;


    class GraphGenerationOptions
    {
    public:
        const bool allowUnconnectedSubgraphs = true;

        BoundedOption<unsigned long> graphSize =
            BoundedOption<unsigned long>(
                std::pair<unsigned long, unsigned long>(2,
                    std::numeric_limits<unsigned long>::max()),
                "Cannot have a graph with <2 vertices.");

        /**
         * how likely are people to have connections to other people?
         */
        PercentOption linkChance;


        //corresponding to the maximum number of people someone
        //can maintain social relationships with
        //(hard to have 1000 best friends)
        BoundedOption<unsigned long> maxEdgesPerVertex =
            BoundedOption<unsigned long>(
                std::make_pair(2,
                    std::numeric_limits<unsigned long>::max()));


        //how are sales skills distributed throughout the population?
        //random guess: normal distribution?
        const std::function<double()> salesSkillDistribution;


        /**
         * no spontaneous conversions: parentless Distributors will only be created
         * during graph generation
         */
        const bool onlyInitialOnboarding;
    } graphGenerationOptions;


    class Defaults
    {
    private:
        static const double defaultSalesChance;
        static const double defaultConversionChance;

    protected:
        Defaults(Config* config);

    public:
        /**
        * Consumer factory
        */
        static const std::function<std::shared_ptr<Consumer>(Unique, Money)> 
            mkDefaultConsumer;
    };
    std::unique_ptr<Config::Defaults> defaults = make_unique<Config::Defaults>(this);
};



}
