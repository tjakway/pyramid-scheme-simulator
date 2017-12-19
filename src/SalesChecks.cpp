#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"
#include "SalesResult.hpp"
#include "Util/Either.hpp"

#include <string>
#include <exception>
#include <memory>
#include <set>
#include <utility>

namespace pyramid_scheme_simulator {

SaleHandler::SaleHandler(RestockHandler::RestockSet&& restockSet)
    : restockSet(restockSet)
{ }

/**
 * this class exists to prevent dynamic_cast'ing twice
 * if the sale failed it just returns SalesResult,
 * otherwise it will return the downcasted pointers
 */
class SaleHandler::SaleIsPossibleResult
{
public:
    const SalesResult result;
    const std::shared_ptr<Distributor> seller;
    const std::shared_ptr<Consumer> buyer;
private:
    SaleIsPossibleResult(SalesResult _result, 
            const std::shared_ptr<Distributor> _seller,
            const std::shared_ptr<Consumer> _buyer)
        : result(_result), seller(_seller), buyer(_buyer),
            success(_result.success)
    {}

public:
    class SaleIsPossibleResultException :  public std::exception {
        std::string msg;
    public:
        SaleIsPossibleResultException(SalesResult res) {
            std::ostringstream ss;
            ss << "Tried to access downcasted pointers in SaleIsPossibleResult"
                << " when SalesResult was " << res << std::endl; 
            msg = ss.str();
        }
        SaleIsPossibleResultException(std::string msg) : msg(msg) {}

        virtual const char* what() const throw() override { 
            return msg.c_str();
        }
    };

    explicit operator bool() const {
        return result.success;
    }

    bool operator!() const {
        return !result.success;
    }

    const bool success;


    static const std::shared_ptr<Distributor> 
        checkSellerPointer(std::shared_ptr<Distributor>);
    static const std::shared_ptr<Consumer> checkBuyerPointer(std::shared_ptr<Consumer>);


    //thin wrappers over the static methods
    const std::shared_ptr<Distributor> checkSellerPointer() {
        return SaleIsPossibleResult::checkSellerPointer(seller);
    }
    const std::shared_ptr<Consumer> checkBuyerPointer() {
        return SaleIsPossibleResult::checkBuyerPointer(buyer);
    }

    static SaleIsPossibleResult good(SalesResult r, 
            const std::shared_ptr<Distributor> seller,
            const std::shared_ptr<Consumer> buyer)
    {
        if(!r)
        {
            throw SaleIsPossibleResultException(
                    "SalesResult passed to SaleIsPossibleResult::good \
                     was not SUCCESS");
        }
        else
        {
            return SaleIsPossibleResult(r, 
                    checkSellerPointer(seller), 
                    checkBuyerPointer(buyer));
        }
    }

    static SaleIsPossibleResult bad(SalesResult r)
    {
        if(r)
        {
            throw SaleIsPossibleResultException(
                    "SUCCESS was passed to SaleIsPossibleResult::bad");
        }
        else
        {
            return SaleIsPossibleResult(r, nullptr, nullptr);
        }
    }

};

const std::shared_ptr<Distributor> 
    SaleHandler::SaleIsPossibleResult::checkSellerPointer(std::shared_ptr<Distributor> seller)

{
    if(seller.get() == nullptr)
    {
        throw SaleIsPossibleResultException(
                "Passed null Distributor pointer to SaleIsPossibleResult::good");
    }
    else
    {
        return seller;
    }
}

const std::shared_ptr<Consumer> 
    SaleHandler::SaleIsPossibleResult::checkBuyerPointer(std::shared_ptr<Consumer> buyer)
{
    if(buyer.get() == nullptr)
    {
        throw SaleIsPossibleResultException(
                "Passed null Consumer pointer to SaleIsPossibleResult::good");
    }
    else
    {
        return buyer;
    }
}

SaleHandler::SaleIsPossibleResult 
    SaleHandler::processPotentialSale(
            Money /*price*/,
            rd_ptr rd,
            CapitalHolder& _seller,
            CapitalHolder& _buyer)
{
    auto saleIsPossibleResult = saleIsPossible(_seller, _buyer);
    if(saleIsPossibleResult)
    {
        auto chanceProc = sampleSalesChance(rd, _seller, _buyer);
        if(chanceProc)
        {
            
            return SaleHandler::SaleIsPossibleResult::good(
                    SalesResult::Reason::SUCCESS,
                    saleIsPossibleResult.seller,
                    saleIsPossibleResult.buyer
                    );
        }
        else
        {
            return SaleHandler::SaleIsPossibleResult::bad(chanceProc);
        }
    }
    else
    {
        return saleIsPossibleResult;
    }
}

SalesResult SaleHandler::sampleSalesChance(rd_ptr rd, CapitalHolder& seller, CapitalHolder& buyer)
{
    const bool saleOccurred = (seller.getSalesChanceContribution() + 
                buyer.getSalesChanceContribution())->sampleFrom(rd);
    if(saleOccurred)
    {
        return SalesResult(SalesResult::Reason::SUCCESS);
    }
    else
    {
        return SalesResult(SalesResult::Reason::CHANCE_FAILED);
    }
}



SaleHandler::SaleIsPossibleResult SaleHandler::saleIsPossible(
        CapitalHolder& _seller, CapitalHolder& _buyer)
{
    Distributor* seller = dynamic_cast<Distributor*>(&_seller);
    if(!seller) {
        return SaleHandler::SaleIsPossibleResult::bad(
                SalesResult(SalesResult::Reason::SELLER_NOT_DISTRIBUTOR));
    }
    else {
        //check the seller
        const auto needsRestockResult = needsRestock(*seller);
        if(needsRestockResult) {
            return SaleHandler::SaleIsPossibleResult::bad(needsRestockResult);
        }
        else if(!seller->hasInventory())
        {
            return SaleHandler::SaleIsPossibleResult::bad(
                    SalesResult(SalesResult::Reason::NO_INVENTORY));
        }
        //finally check the buyer
        else {

            Consumer* buyer = dynamic_cast<Consumer*> (&_buyer);
            if(!buyer) {
                return SaleHandler::SaleIsPossibleResult::bad(
                        SalesResult(SalesResult::Reason::BUYER_NOT_CONSUMER));
            }
            //make sure they're not also a distributor
            else if(buyer->isDistributor()){
                return SaleHandler::SaleIsPossibleResult::bad(
                        SalesResult(SalesResult::Reason::BOTH_DISTRIBUTORS));
            }
            else if(!seller->hasInventory()){
                return SaleHandler::SaleIsPossibleResult::bad(
                        SalesResult::Reason::NO_INVENTORY);
            }
            else {
                return SaleHandler::SaleIsPossibleResult::good(
                        SalesResult(SalesResult::Reason::SUCCESS),
                        std::shared_ptr<Distributor>(seller),
                        std::shared_ptr<Consumer>(buyer)
                        );
            }
        }

    }
}


SalesResult SaleHandler::needsRestock(Distributor& seller)
{
    //check if the set has this distributor's id
    if(restockSet.find(seller.id) == restockSet.end()) {
        return SalesResult(SalesResult::Reason::NEEDS_RESTOCK);
    }
    else {
        return SalesResult(SalesResult::Reason::SUCCESS);
    }
}


SaleHandler::RecordType 
    SaleHandler::doSale(
            SimulationTick when, 
            Money price,
            rd_ptr rd,
            CapitalHolder& seller,
            CapitalHolder& buyer)
{
    SaleIsPossibleResult result = 
        SaleHandler::processPotentialSale(price, rd, seller, buyer);

    if(!result)
    {
        return singleElementListTransactionRecord<SaleHandler::ElementType>(
                make_unique<Either<SalesResult, Sale>>(
                        Either<SalesResult, Sale>::left(
                make_unique<SalesResult>(result.result))));
    }
    else
    {
        auto seller = result.seller;
        auto buyer = result.buyer;

        std::unique_ptr<Sale> sale = make_unique<Sale>(Sale(when, price, seller, buyer));

        CapitalHolder::atomicallyDoSaleSideEffects(price, seller.get(), buyer.get());

        return singleElementListTransactionRecord<SaleHandler::ElementType>(
            make_unique<Either<SalesResult, Sale>>(
                    Either<SalesResult, Sale>::right(std::move(sale))));
    }
}

SaleHandler::RecordType 
    SaleHandler::operator()(
            SimulationTick when, 
            Money price,
            rd_ptr rd,
            CapitalHolder& seller,
            CapitalHolder& buyer)
{
    return doSale(when, price, rd, seller, buyer);
}


SaleHandler::SaleHandler(
        const Money _wholesalePrice,
        std::shared_ptr<Company> _company, 
        RestockHandler::RestockSet&& _restockSet)
    : company(_company),
    wholesalePrice(_wholesalePrice),
    restockSet(_restockSet)
{ }

SaleHandler::RecordType SaleHandler::processRestocks(const Money price, 
        std::set<PopulationGraph::Pop> restockPops, 
        rd_ptr rd)
{

    emptyListTransactionRecord<ConversionHandler::ElementType>().leftFold(
            std::list<ConversionHandler::RecordType>(
                std::make_move_iterator(vecConversions.begin()),
                std::make_move_iterator(vecConversions.end())),
            conversionHandler.reduce);
}


}
