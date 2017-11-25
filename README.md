Wishlist:
    * make all transformations idempotent and reversible

Requires:
    * libuuid (for crossguid): install with `sudo apt-get install uuid-dev`


----------------------------------

TODO:
    * need to calculate Distributor reimbursement by walking the recruitment hierarchy

    * run a consumption check on each Distributor--if they use their own product, decrease their inventory but don't change anything else (since they already paid for it)
    * whenever a Distributor runs low on inventory they need to buy from the company again
    * model competition

Config TODO:
    * have flags for default graph sizes: --verysmall --small --medium --big --verybig etc.
    * if we transition to database-backed transactions, have a flag to commit or rollback when interrupted


Notes on Debugging:
    * don't forget the tests are in the pyramid_scheme_simulator namespace
    * google test will mangle the names, example:
(gdb) break pyramid_scheme_simulator::BasicGraphTests
BasicGraphTests                                                                  BasicGraphTests_BasicRestockTest_Test::~BasicGraphTests_BasicRestockTest_Test()
BasicGraphTests::BasicGraphTests()                                               BasicGraphTests_BasicTest_Test
BasicGraphTests::~BasicGraphTests()                                              BasicGraphTests_BasicTest_Test::BasicGraphTests_BasicTest_Test()
BasicGraphTests_BasicRestockTest_Test                                            BasicGraphTests_BasicTest_Test::TestBody()
BasicGraphTests_BasicRestockTest_Test::BasicGraphTests_BasicRestockTest_Test()   BasicGraphTests_BasicTest_Test::test_info_
BasicGraphTests_BasicRestockTest_Test::TestBody()                                BasicGraphTests_BasicTest_Test::~BasicGraphTests_BasicTest_Test()
BasicGraphTests_BasicRestockTest_Test::test_info_

