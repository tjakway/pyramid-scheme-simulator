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
