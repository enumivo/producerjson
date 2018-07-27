#include <enulib/enu.hpp>
#include <enulib/asset.hpp>
#include <enulib/multi_index.hpp>
#include <enu.system/enu.system.hpp>

using enumivo::asset;
using std::string;

class producerjson_contract : public enumivo::contract {
 public:
  producerjson_contract(account_name self) :
    enumivo::contract(self),
    producerjson(_self, _self) {}


  // A simple store for a producer's json.
  void set(const account_name owner, const string json) {
    require_auth(owner);

    // Check producer info table. Owner should exist (has called regproducer) and be activated.
    typedef enumivo::multi_index<N(producers), enumivosystem::producer_info> producer_info_t;
    producer_info_t _producers(N(enumivo), N(enumivo));
    auto prod = _producers.get(owner,    "user is not a producer");
    enumivo_assert(prod.is_active == true, "user is not an active producer");
    enumivo_assert(prod.total_votes > 0.0, "user is not an active producer");

    // Quick check to remind the user the payload must be json.
    enumivo_assert(json[0] == '{',             "payload must be json");
    enumivo_assert(json[json.size()-1] == '}', "payload must be json");

    // If entry exists, update it.
    auto target_itr = producerjson.find(owner);
    if (target_itr != producerjson.end()) {
      producerjson.modify(target_itr, owner, [&](auto& j) {
        j.owner = owner;
        j.json  = json;
      });
    } else {  // Otherwise, create a new entry for them.
      producerjson.emplace(owner, [&](auto& j) {
        j.owner = owner;
        j.json  = json;
      });
    }
  }


  // Allows a producer to delete their entry.
  void del(const account_name owner) {
    require_auth(owner);
    auto target_itr = producerjson.find(owner);
    producerjson.erase(target_itr);
  }


 private:
  // @abi table producerjson i64
  struct producerjson {
    account_name owner;
    string       json;

    auto primary_key() const {  return owner;  }
    ENULIB_SERIALIZE(producerjson, (owner)(json))
  };
  typedef enumivo::multi_index<N(producerjson), producerjson> producerjson_table;
  producerjson_table producerjson;
};

ENUMIVO_ABI(producerjson_contract, (set)(del))
