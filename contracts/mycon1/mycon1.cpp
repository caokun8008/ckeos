/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#include <mycon1.hpp>
#include <eosiolib/eosio.hpp>
#include <iostream>
#include <fstream>

/**
 *  The init() and apply() methods must have C calling convention so that the blockchain can lookup and
 *  call these methods.
 */


using namespace eosio;
    struct grttime {
        account_name user;
        uint64_t hitimes;
        uint64_t byetimes;
        uint64_t primary_key()const { return user; }

        EOSLIB_SERIALIZE( grttime, (user)(hitimes)(byetimes) )
    };
    typedef eosio::multi_index<N(greeting), grttime> count;

    struct who {
        account_name user1;
        account_name user2;
        EOSLIB_SERIALIZE( who, (user1)(user2))
    };

extern "C" {

  void increase_hitimes(uint64_t receiver, uint64_t who) {
        count counttable( receiver, who );
        const auto& usr = counttable.find( who );
        if ( usr == counttable.end()) {
            counttable.emplace( receiver, [&]( auto& s ) {
                s.user = who;
                s.hitimes = 1;
                s.byetimes = 0;
            });
        } 
        else {
            counttable.modify( usr, 0, [&]( auto& s ) { s.hitimes++; });
        }
    }

    void increase_byetimes(uint64_t receiver, uint64_t who) {
        count counttable( receiver, who );
        const auto& usr = counttable.find( who );
        if ( usr == counttable.end()) {
            counttable.emplace( receiver, [&]( auto& s ) {
                s.user = who;
                s.hitimes = 0;
                s.byetimes = 1;
            });
        } 
        else {
            counttable.modify( usr, 0, [&]( auto& s ) { s.byetimes++; });
        }
    }

    // The apply method implements the dispatch of events to this contract
    void apply( uint64_t receiver, uint64_t code, uint64_t action ) {
       //print( "liyan mycon1: ", name{receiver}, "->", name{code}, "->", name{action}, "\n" );
  
  std::fstream f("txt_out.txt", std::ios_base::out);  
  if(f.bad())  
  {  
     return 0;  
  }  
    
  f << "hello and bye!" << std::endl;  
  f.close();
       uint64_t pp = 3317;
       const who &w = unpack_action_data<who>();
       switch (action) {
          case N(hi):
              increase_hitimes( receiver, w.user1 );
              increase_hitimes( receiver, w.user2 );
              for (uint64_t i = 0; i < 70000; i++){   // 76000
                  pp = ((pp * (pp % 64) + i) * 7) >> 3;
              }    
              print( "Hello liyan, ", name{w.user1}, " and ", name{w.user2}, "  ", pp, "\n");
              break;
          case N(bye):
              increase_byetimes( receiver, w.user1 );
              increase_byetimes( receiver, w.user2 );
              for (uint64_t i = 0; i < 77000; i++){
                  pp = ((pp * (pp % 64) + i) * 7) >> 3;
              }  
              print( "byebye, ", name{w.user1}, " and ", name{w.user2}, pp, "\n");
              break;
          default:
              print( "no hit, ", name{w.user1}, " and ", name{w.user2}, "\n");
              return;
       }
    }

} // extern "C"



