/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */

#include "consumemem.hpp"

namespace eosio {

void consumemem::kilo( account_name id, uint64_t num )
{
    require_auth( id );

    consumedata datatable( _self, id );
    auto data = datatable.find( id );
    if ( data == datatable.end() ) {
        datatable.emplace( _self, [&]( auto& b ) {
           b.id        = id;
           b.mem.reserve(10000);
           b.mem.emplace_back(1024 * num, 'a');
        });
    }
    else {
        datatable.modify( data, 0, [&]( auto& b ) {
           b.mem.emplace_back(1024 * num, 'a');
           //string aa(1024 * num, 'b');
           //b.mem.push_back(aa);
           //b.mem[0] = aa;
        });
    }

    consumeinfo infotable( _self, id );
    auto info = infotable.find( id );
    if ( info == infotable.end() ) {
        infotable.emplace( _self, [&]( auto& b ) {
           b.id    = id;
           b.count = num;
        });
    }
    else {
        infotable.modify( info, 0, [&]( auto& b ) {
           b.count += num;
        });
    }
}

void consumemem::mega( account_name id, uint64_t num )
{
    kilo( id, num * 1024);
}

void consumemem::giga( account_name id, uint64_t num )
{
    kilo( id, num * 1024 * 1024);
}

} /// namespace eosio

EOSIO_ABI( eosio::consumemem, (kilo)(mega)(giga) )
