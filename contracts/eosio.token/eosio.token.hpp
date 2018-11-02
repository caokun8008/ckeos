/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/singleton.hpp>

#include <string>

namespace eosiosystem {
   class system_contract;
}

namespace eosio {

   using std::string;

   class token : public contract {
      public:
         token( account_name self ):contract(self){}

         void create( account_name issuer,
                      asset        maximum_supply);

         void issue( account_name to, asset quantity, string memo );

         void transfer( account_name from,
                        account_name to,
                        asset        quantity,
                        string       memo );

         void consume( account_name from, asset quantity, string memo );

         void exchange( account_name payer, asset quantity );

         void setrate( double rate );

         void setfee( uint32_t fee );
      
         inline asset get_supply( symbol_name sym )const;
         
         inline asset get_balance( account_name owner, symbol_name sym )const;

      private:
         struct account {
            asset    balance;

            uint64_t primary_key()const { return balance.symbol.name(); }
         };

         struct currency_stats {
            asset          supply;
            asset          max_supply;
            account_name   issuer;

            uint64_t primary_key()const { return supply.symbol.name(); }
         };

         struct token_config {
            double      rate = 1.0; /// exchange rate, 'rate' EOS per SN
            uint32_t    fee = 50; /// exchange fee, unit: 1/10000

            EOSLIB_SERIALIZE( token_config, (rate)(fee) )
         };

         typedef eosio::multi_index<N(accounts), account> accounts;
         typedef eosio::multi_index<N(stat), currency_stats> stats;
         typedef eosio::singleton<N(config), token_config> config;

         void sub_balance( account_name owner, asset value );
         void add_balance( account_name owner, asset value, account_name ram_payer );

      public:
         struct transfer_args {
            account_name  from;
            account_name  to;
            asset         quantity;
            string        memo;
         };
   };

   asset token::get_supply( symbol_name sym )const
   {
      stats statstable( _self, sym );
      const auto& st = statstable.get( sym );
      return st.supply;
   }

   asset token::get_balance( account_name owner, symbol_name sym )const
   {
      accounts accountstable( _self, owner );
      const auto& ac = accountstable.get( sym );
      return ac.balance;
   }

} /// namespace eosio
