#pragma once

#include <memory>
#include <random>
#include <functional>
#include <utility>
#include <limits>

#include <spdlog/spdlog.h>

#include "Types.hpp"
#include "ChanceContributor.hpp"
#include "CapitalHolderClassDecls.hpp"
#include "Util/Util.hpp"
#include "Tick.hpp"
#include "Util/Unique.hpp"
#include "BoundedOption.hpp"

namespace pyramid_scheme_simulator {
/**
 * simulation parameters
 * see doc for explanation
 *
 * TODO: CTOR with preamble
 */
class Config
{
protected:
    const rd_seed_type randomSeed;

//public config values
public:

    const rd_ptr randomGen;

    using ChanceDistribution = std::function<std::unique_ptr<ChanceContributor>(rd_ptr)>;

    class SimulationOptions
    {
    public:
        const SimulationTick maxTicks;

        /**
         * TODO: if needed can replace these with a function
         *
         * unsigned int productCost(CapitalHolder&);
         *
         * that returns how much it costs for that Pop to buy it
         * but that might require a lot of vtable lookups for
         * almost no benefit
         *
         * ********************************************
         *
         * could make these the starting values and 
         * have Transactions dynamically calculate product costs
         * based on competition (i.e. from the number of edges)
         */
        const unsigned int standardProductCost;
        const unsigned int wholesaleProductCost;


        const std::function<Money()> startingFunds;

        class DistributorOptions
        {
        public:
            //how much does a distributor get from converting someone else 
            //into a distributor?
            //bounded to 99% because it doesn't make sense 
            //if a distributor gets all of the
            //earnings of a subdistributor
            BoundedOption<double> downstreamPercent = 
                BoundedOption<double>(std::pair<double, double>(0.0, 0.99));

            //how much inventory do you need to buy from the company to become
            //a distributor?
            const unsigned int buyIn;

            //used by Consumer::becomeDistributor
            const NewDistributorFunction newDistributorFunction;

            DistributorOptions(double, const unsigned int, NewDistributorFunction);
        };
        std::unique_ptr<DistributorOptions> distributionOptions;

        SimulationOptions(std::unique_ptr<DistributorOptions>&&,
                const SimulationTick,
                const unsigned int, const unsigned int,
                const std::function<Money()>);

    };
    std::unique_ptr<SimulationOptions> simulationOptions;
    

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
        const bool allowDisconnectedSubgraphs = true;

        BoundedOption<unsigned long> graphSize =
            BoundedOption<unsigned long>(
                std::pair<unsigned long, unsigned long>(2,
                    std::numeric_limits<unsigned long>::max()),
                "Cannot have a graph with <2 vertices.");

        /**
         * how likely are people to have connections to other people?
         */
        std::unique_ptr<PercentOption> linkChance;


        //corresponding to the maximum number of people someone
        //can maintain social relationships with
        //(hard to have 1000 best friends)
        BoundedOption<unsigned long> maxEdgesPerVertex =
            BoundedOption<unsigned long>(
                std::make_pair(2,
                    std::numeric_limits<unsigned long>::max()));


        //how are sales skills distributed throughout the population?
        //random guess: normal distribution?
        const ChanceDistribution salesSkillDistribution;

        const ChanceDistribution conversionChanceDistribution;


        /**
         * no spontaneous conversions: parentless Distributors will only be created
         * during graph generation
         */
        const bool onlyInitialOnboarding;

        GraphGenerationOptions(const bool,
                const unsigned long,
                const double,
                const unsigned long,
                const ChanceDistribution,
                const ChanceDistribution,
                const bool);
    };
    std::unique_ptr<GraphGenerationOptions> graphGenerationOptions;


    class Defaults
    {
    public:
        Defaults();

        /**
        * Consumer factory
        */
        std::shared_ptr<Consumer> mkConsumer(Config&, rd_ptr, Unique);
    };
    Defaults defaults;


    Config(const rd_seed_type seed,
            std::unique_ptr<SimulationOptions>&&,
            std::unique_ptr<GraphGenerationOptions>&&);

    Config(std::unique_ptr<SimulationOptions>&&,
            std::unique_ptr<GraphGenerationOptions>&&);

    virtual ~Config() {}

    /********************/
    /******Statics*******/
    /********************/

    static const std::shared_ptr<spdlog::logger> logger;

    /********************/
};



}
