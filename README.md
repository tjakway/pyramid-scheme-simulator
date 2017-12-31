Wishlist:
    * make all transformations idempotent and reversible

Requires:
    * libuuid (for crossguid): install with `sudo apt-get install uuid-dev`
    * Boost::graph (libboost-graph-dev)
    * for the opengl backend on linux: libgtkmm-3.0-dev libgl1-mesa-dev libglew-dev

----------------------------------

TODO:
    * need to calculate Distributor reimbursement by walking the recruitment hierarchy

    * run a consumption check on each Distributor--if they use their own product, decrease their inventory but don't change anything else (since they already paid for it)
    * whenever a Distributor runs low on inventory they need to buy from the company again
    * model competition
    * rename SalesResult -> SaleResult
    * implement ```buyIn``` instead of just waiting for the restock phase next tick.
    * implement upstream profit transfer... this is what makes it a pyramid scheme, after all

    * disable building dependency tests (xgtest, catch_tests)

Config TODO:
    * have flags for default graph sizes: --verysmall --small --medium --big --verybig etc.
    * if we transition to database-backed transactions, have a flag for whether to commit what we have so far or rollback when interrupted


Notes on Debugging:
    * don't forget the tests are in the pyramid_scheme_simulator namespace
    * google test will mangle names and make setting breakpoints painful, for example:
(gdb) break pyramid_scheme_simulator::BasicGraphTests
BasicGraphTests                                                                  BasicGraphTests_BasicRestockTest_Test::~BasicGraphTests_BasicRestockTest_Test()
BasicGraphTests::BasicGraphTests()                                               BasicGraphTests_BasicTest_Test
BasicGraphTests::~BasicGraphTests()                                              BasicGraphTests_BasicTest_Test::BasicGraphTests_BasicTest_Test()
BasicGraphTests_BasicRestockTest_Test                                            BasicGraphTests_BasicTest_Test::TestBody()
BasicGraphTests_BasicRestockTest_Test::BasicGraphTests_BasicRestockTest_Test()   BasicGraphTests_BasicTest_Test::test_info_
BasicGraphTests_BasicRestockTest_Test::TestBody()                                BasicGraphTests_BasicTest_Test::~BasicGraphTests_BasicTest_Test()
BasicGraphTests_BasicRestockTest_Test::test_info_


Code Quality:
    * figure out how to implement PopulationGraph::mutateVerticesOfGraph with templates so we could share code between PopulationGraph::mutateVertices and PopulationGraph::mutateVerticesWithPredicate.  Basically need to figure out how to get filtered_graph<BGLPopulationGraph, VertexPredicateObject> to behave like BGLPopulationGraph, and the obvious route of trying a template function wasn't successful.
    * CapitalHolder::clone has the same obvious implementation in every subclass...
        ```std::make_shared<WhoAmI>(*this)```
       to call the copy constructor...
       but getting access to WhoAmI would require using the CRTP and it is irritating to change all existing code to deal with CapitalHolder having a template parameter
