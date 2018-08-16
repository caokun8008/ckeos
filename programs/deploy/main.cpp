/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 *  @defgroup eosclienttool EOSIO Command Line Client Reference
 *  @brief Tool for sending transactions and querying state from @ref nodeos
 *  @ingroup eosclienttool
 */
/**
  @defgroup eosdeploytool

  @section intro Introduction to deploy

  `deploy` is a command line tool that deploy the producers. In order to use `deploy` you will need to
  have a configuration file, This configuration file contains basic information that needs to be deployed..

   deploy contains documentation for all of its commands. For a list of all commands known to deploy, simply run it with no arguments:
```
$ ./deploy
Command Line Interface to deploy tool
Usage: programs/deploy/deploy [OPTIONS] SUBCOMMAND

Options:
  -h,--help                   Print this help message and exit
  -f,--config-file            Configuration file with full path 

Subcommands:
  version                     Retrieve version information
  gen_producers               Generate the producer's profile and genesis profile                      

```
To get help with any particular subcommand, run it with no arguments as well:
```
$ ./cleos version

*/

#include <string>
#include <vector>
#include <iostream>
#include <fc/variant.hpp>
#include <fc/exception/exception.hpp>
#include <fc/variant_object.hpp>

#include <fc/io/fstream.hpp>

#include "CLI11.hpp"
#include "localize.hpp"
#include "config.hpp"
#include "deployment.hpp"

using namespace std;
using namespace eosio;
using namespace eosio::deploy::localize;
using namespace eosio::deploy::config;
using namespace eosio::deploy;

string config_file_path = "";

static deployment dep;

int main( int argc, char** argv ) 
{
   setlocale(LC_ALL, "");
   bindtextdomain(locale_domain, locale_path);
   textdomain(locale_domain);

   /** set default cfg file name */
   auto file_name = fc::home_path().string() + "/eosio-deploy/eosio-deploy.json";
   
   config_file_path = file_name;

   CLI::App app{"Command Line Interface to EOSIO Client"};
   
   app.require_subcommand();

   app.add_subcommand("version", localized("Retrieve version information"))->set_callback([] 
   {
      std::cout << localized("Build version: ${ver}", ("ver", eosio::deploy::config::version_str)) << std::endl;
   });

   /** Generate the producer's profile and genesis profile */
   auto gen_producers_subcommand = app.add_subcommand("gen_producers", localized("Generate the producer's profile and genesis profile"))->set_callback([] 
   {
      dep.set_deploy_cfg_file( config_file_path );
    
      std::cout << "Generate the producer's profile and genesis profile strating......" << std::endl;
      dep.process_deploy();
   });
   /** gen_producers subcommand has one option '-f' */
   gen_producers_subcommand->add_option( "-f,--config-file", config_file_path, localized("Configuration file with full path"), true );
   
   try {
       app.parse(argc, argv);
   } catch (const CLI::ParseError &e) {
       return app.exit(e);
   } catch (const fc::exception& e) {
      elog("${e}", ("e",e.to_detail_string()));
   }
   return 0;
}

