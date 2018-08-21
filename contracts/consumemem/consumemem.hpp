/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once

#include <eosiolib/eosio.hpp>
#include <string>

namespace eosio {

   using std::string;

   class consumemem : public contract {
      public:
         consumemem( account_name self ):contract(self){}

         void kilo( account_name id, uint64_t num );
         void mega( account_name id, uint64_t num );
         void giga( account_name id, uint64_t num );

      private:
         struct consume_data {
            account_name   id;
            vector<string> mem;

            account_name primary_key()const { return id; }
         };

         struct consume_info {
            account_name   id;
            uint64_t       count;

            account_name primary_key()const { return id; }
         };

         typedef eosio::multi_index<N(consumedata), consume_data> consumedata;
         typedef eosio::multi_index<N(consumeinfo), consume_info> consumeinfo;
   };

} /// namespace eosio
