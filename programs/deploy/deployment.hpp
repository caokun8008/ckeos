#pragma once
#include <iostream>

#include <fstream>
#include <fc/filesystem.hpp>
#include <fc/exception/exception.hpp>

#include <fc/io/json.hpp>
#include <fc/variant.hpp>

#include <boost/process.hpp>
#include <boost/program_options.hpp>

#include <eosio/chain/genesis_state.hpp>
#include <eosio/chain/name.hpp>

namespace eosio
{
   namespace deploy {
      namespace bp = boost::process;
      namespace bpo = boost::program_options;
      typedef std::map<std::string, std::pair<std::string,boost::any>> nodeos_cfg_map;

      enum stream_type{
         stream_type_null = 0,
         stream_type_file,
         stream_type_cout
      };

      struct producer_config{
         std::string name;
         std::string location;
         std::string http_server_port;
         std::string p2p_server_port;
         std::vector<std::string> plugins;
      };
      struct deployment_config{
         std::string primary_node_producer;
         std::vector<producer_config> producers;
      };
      struct deploy_keypair{
         std::string pub_key;
         std::string pri_key;

      };

      struct deploy_account{
         std::string name;
         deploy_keypair owner_key;
         deploy_keypair active_key;
      };
	  
      struct deploy_accounts {
         std::string copyright_uuid;
         std::vector<deploy_account> deploy_accounts;
      };

   class deployment
   {
      public:
         deployment(){
            nodeos_default_cfg.emplace(std::make_pair("blocks-dir",
                                                     std::make_pair("the location of the blocks directory (absolute path or relative to application data dir) (eosio::chain_plugin)",
                                                                    std::string("blocks"))));
            nodeos_default_cfg.emplace(std::make_pair("enable-stale-production",
                                                     std::make_pair("Enable block production, even if the chain is stale. (eosio::producer_plugin)",
                                                                    std::string("true"))));
            nodeos_default_cfg.emplace(std::make_pair("http-server-address",
                                                     std::make_pair("The local IP and port to listen for incoming http connections; set blank to disable. (eosio::http_plugin)",
                                                                    std::string("0.0.0.0:8888"))));
            nodeos_default_cfg.emplace(std::make_pair("http-alias",
                                                     std::make_pair("Additionaly acceptable values for the \"Host\" header of incoming HTTP requests, can be specified multiple times.  Includes http/s_server_address by default. (eosio::http_plugin)",
                                                                  std::string("0.0.0.0:8888"))));
            nodeos_default_cfg.emplace(std::make_pair("p2p-listen-endpoint",
                                                     std::make_pair("The actual host:port used to listen for incoming p2p connections. (eosio::net_plugin)",
                                                                    std::string("0.0.0.0:9876"))));
            nodeos_default_cfg.emplace(std::make_pair("p2p-server-address",
                                                     std::make_pair("An externally accessible host:port for identifying this node. Defaults to p2p-listen-endpoint. (eosio::net_plugin)",
                                                                    std::string("localhost:9876"))));
            nodeos_default_cfg.emplace(std::make_pair("allowed-connection",
                                                     std::make_pair("Can be 'any' or 'producers' or 'specified' or 'none'. If 'specified', peer-key must be specified at least once. If only 'producers', peer-key is not required. 'producers' and 'specified' may be combined. (eosio::net_plugin)",
                                                                    std::string("any"))));
            std::vector<std::string> p2p_peer_addr;
            nodeos_default_cfg.emplace(std::make_pair("p2p-peer-address",
                                                     std::make_pair("The public endpoint of a peer node to connect to. Use multiple p2p-peer-address options as needed to compose a network. (eosio::net_plugin)', peer-key is not required. 'producers' and 'specified' may be combined. (eosio::net_plugin)",
                                                                    p2p_peer_addr)));
            nodeos_default_cfg.emplace(std::make_pair("required-participation",
                                                     std::make_pair("required participation",
                                                                    std::string("true"))));
            std::vector<std::string> producer_names;
            nodeos_default_cfg.emplace(std::make_pair("producer-name",
                                                     std::make_pair("ID of producer controlled by this node (e.g. inita; may specify multiple times) (eosio::producer_plugin)",
                                                                    producer_names)));
            std::vector<std::string> signature_providers;
            nodeos_default_cfg.emplace(std::make_pair("signature-provider",
                                                    std::make_pair("# Key=Value pairs in the form <public-key>=<provider-spec>\r\n\
                                                                  # Where:\r\n\
                                                                  #    <public-key>       is a string form of a vaild EOSIO public key\r\n\
                                                                  #\r\n\
                                                                  #    <provider-spec>    is a string in the form <provider-type>:<data>\r\n\
                                                                  #\r\n\
                                                                  #    <provider-type>    is KEY, or KEOSD\r\n\
                                                                  #\r\n\
                                                                  #    KEY:<data>         is a string form of a valid EOSIO private key which maps to the provided public key\r\n\
                                                                  #\r\n\
                                                                  #    KEOSD:<data>       is the URL where keosd is available and the approptiate wallet(s) are unlocked (eosio::producer_plugin)",
                                                                  signature_providers)));
            std::vector<std::string> plugins={"eosio::producer_plugin",
                                                "eosio::chain_api_plugin",
                                                "eosio::wallet_plugin",
                                                "eosio::wallet_api_plugin",
                                                "eosio::http_plugin",
                                                "eosio::net_plugin",
                                                "eosio::net_api_plugin"};
            nodeos_default_cfg.emplace(std::make_pair("plugin",
                                                   std::make_pair("Plugin(s) to enable, may be specified multiple times",
                                                                  plugins)));
         }
         ~deployment();
         
         void set_deploy_cfg_file(const fc::path& deploy_cfg = "./eosio-deploy.json"){
            fc::path deploy_cfg_parm(deploy_cfg);

            if(deploy_cfg_parm.is_relative()){
               deploy_cfg_parm = fc::current_path()/deploy_cfg_parm;
            }
            /** get the real system format path */
            boost::filesystem::path temp_path(deploy_cfg_parm.string());
            deploy_cfg_parm = temp_path.lexically_normal().string();

            if(!fc::exists(deploy_cfg_parm)){
                  FC_THROW("The specified configuration file dos not exist!", ("filename", deploy_cfg_parm.string() ));
            }
            else{
               if(deploy_cfg_parm.extension().string().compare(".json") != 0){
                  FC_THROW("Config file must be \".json\"!", ("input extension", deploy_cfg_parm.extension().string() ));
               }
            }
            /** get parent dir as deploy dir */
            deploy_dir_name = deploy_cfg_parm.parent_path();
            deploy_cfg_name = deploy_cfg_parm;
            deploy_dir_work = deploy_dir_name/"deploy-work/";
            
         }
         void process_deploy(){
            /**
             * Create dependent folders and files
             * 1¡¢Create the deployed working directory; if the working directory exists, remove all.
             * 2¡¢Analysis profile and push infomation to private var deploy_cfg
             * 3¡¢Greate the genesis.gason
            */
            std::cout<<"processing-->Create the deployed working directory" <<std::endl;
            if(!fc::exists(deploy_dir_work)){
               fc::create_directories(deploy_dir_work);
            }else{
            }

            std::cout<<"processing-->Analysis profile:" <<deploy_cfg_name.string() <<std::endl;
            deploy_cfg = fc::json::from_file(deploy_cfg_name).as<deployment_config>();
            bool primary_node_check = false;
            for( auto producer : deploy_cfg.producers ){
               if(0 == deploy_cfg.primary_node_producer.compare(producer.name)){
                  primary_node_check = true;
                  break;
               }
            }
            if(false == primary_node_check){
               FC_THROW("Primary Node producer name dos not in producers list!");
            }

            /** gen eosio account keypair */
            deploy_account eosio_acc;
            eosio_acc.name = "eosio";
            eosio_acc.active_key = gen_keypair();
            eosio_acc.owner_key = gen_keypair();
            deploy_acc_list.deploy_accounts.push_back(eosio_acc);
            
            /** gen genesis.jason */
            std::cout<<"processing-->Create default nodeos's genesis file." <<std::endl;
            eosio::chain::genesis_state genesis_cfg; 
            genesis_cfg.initial_timestamp = calculate_genesis_timestamp("now");
            genesis_cfg.initial_key = fc::variant( deploy_acc_list.deploy_accounts[0].active_key.pub_key ).as<fc::crypto::public_key>();
            deploy_acc_list.copyright_uuid =  genesis_cfg.compute_chain_id();
            fc::json::save_to_file<eosio::chain::genesis_state>(genesis_cfg, deploy_dir_work/"genesis.json", true);

            /** get_specified_producer config for each producer and package them */
            for( auto producer : deploy_cfg.producers ){
               std::cout<<"processing-->Get specified producer's config file for producer "<< producer.name <<" and package them!" <<std::endl;

               /** Create the producer's folder */
               std::cout<<"       deplory-->Create the producer's folder!" <<std::endl;
               deploy_account pd_acc;
               pd_acc.name = producer.name;
               fc::path pd_path(deploy_dir_work/pd_acc.name);

               if( !fc::exists(pd_path)){
                  fc::create_directories(pd_path);
               }

               /** Create the producer's folder */
               std::cout<<"       deplory-->Get specified producer's config file for producer!" <<std::endl;
               if(0 == producer.name.compare(deploy_cfg.primary_node_producer)){
                  nodeos_cfg_map cfg_map = get_specified_producer_config(producer, eosio_acc, pd_acc, deploy_cfg.producers, true);
                  save_producer_config(pd_path/"config.ini", cfg_map);
               }else{
                  nodeos_cfg_map cfg_map = get_specified_producer_config(producer, eosio_acc, pd_acc, deploy_cfg.producers);
                  save_producer_config(pd_path/"config.ini", cfg_map);
               }
               
               deploy_acc_list.deploy_accounts.push_back(pd_acc);

               fc::copy(deploy_dir_work/"genesis.json", pd_path/"genesis.json");

               /** Package producer's files*/
               std::cout<<"       deplory-->Package producer's files!" <<std::endl;
               std::string exe = bp::search_path("tar").string();
               std::vector<std::string> args;
               args.push_back("-cvf");
               args.push_back(deploy_dir_work.string() + pd_acc.name + ".tar");
               args.push_back("-C");
               args.push_back(pd_path.string());
               args.push_back("./");
               execute_sys_cmd(exe, args);

               fc::remove_all(pd_path);
            }
            fc::remove(deploy_dir_work/"genesis.json");
            fc::json::save_to_file<deploy_accounts>( deploy_acc_list, deploy_dir_work/"producers_keys.json", true );
         }
         std::string get_deploy_cfg_fullname(){
            return deploy_cfg_name.string();
         }

      private:
         nodeos_cfg_map get_specified_producer_config(producer_config producer,deploy_account acc_eosio, deploy_account& acc_producer,
                                                        std::vector<producer_config> producers, bool is_primary_node = false){
            nodeos_cfg_map cfg_map = nodeos_default_cfg;

            /** update p2p-peer-address config */
            std::vector<std::string> p2p_peer_addrs;
            for(auto it : producers){
               if(0 == producer.name.compare(it.name)){
                  continue;
               }else{
                     p2p_peer_addrs.push_back(it.location + ":" + it.p2p_server_port);
               }

            }
            cfg_map["p2p-peer-address"].second = p2p_peer_addrs;

            /** update http-server-address config */
            std::string address = boost::any_cast<std::string>(cfg_map["http-server-address"].second);
            address = address.replace(address.find(':')+1, address.length()-address.find(':'), producer.http_server_port);
            cfg_map["http-server-address"].second = address;

            /** update http-alias config */
            address =  producer.location + ":" + producer.http_server_port;
            cfg_map["http-alias"].second = address;


            /** update p2p-listen-endpoint config */
            address = boost::any_cast<std::string>(cfg_map["p2p-listen-endpoint"].second);
            address = address.replace(address.find(':')+1, address.length()-address.find(':'), producer.p2p_server_port);
            cfg_map["p2p-listen-endpoint"].second = address;

            /** update p2p-server-address config */
            address = boost::any_cast<std::string>(cfg_map["p2p-server-address"].second);
            address = address.replace(address.find(':')+1, address.length()-address.find(':'), producer.p2p_server_port);
            cfg_map["p2p-server-address"].second = address;

            /** update plugin config */
            std::vector<std::string> pulgins = boost::any_cast<std::vector<std::string>>(cfg_map["plugin"].second);
            for(auto it : producer.plugins){
               if(find( pulgins.begin(), pulgins.end(), it ) == pulgins.end()){
                     pulgins.push_back(it);
               }
            }
            cfg_map["plugin"].second = pulgins;

            /** update producer-name config */
            std::vector<std::string> producernames;
            if(true == is_primary_node){
               producernames.push_back(acc_eosio.name);
            }
            producernames.push_back(acc_producer.name);
            cfg_map["producer-name"].second = producernames;

            /** update signature-provider config */
            std::vector<std::string> signature_providers;
            acc_producer.owner_key = gen_keypair();
            acc_producer.active_key = gen_keypair();
            if(true == is_primary_node){
               signature_providers.push_back(acc_eosio.active_key.pub_key + "=KEY:" + acc_eosio.active_key.pri_key);
            }
            signature_providers.push_back(acc_producer.active_key.pub_key + "=KEY:" + acc_producer.active_key.pri_key);
            cfg_map["signature-provider"].second = signature_providers;

            return cfg_map;
         }
         void save_producer_config(const fc::path& cfg_file, nodeos_cfg_map& cfg_map){
            std::ofstream out_cfg(cfg_file.string());

            for(auto cfg_it : cfg_map){
               if("signature-provider" == cfg_it.first){
                  continue;
               }else if("p2p-peer-address" == cfg_it.first){
                  out_cfg<< "# " <<cfg_it.second.first <<std::endl;
                  std::vector<std::string> p2p_peer_addrs = boost::any_cast<std::vector<std::string>>(cfg_map["p2p-peer-address"].second);
                  for(auto it : p2p_peer_addrs){
                        out_cfg<<cfg_it.first <<" = " << it <<std::endl;
                  }
                  out_cfg<<std::endl;
               }else if("plugin" == cfg_it.first){
                  out_cfg<< "# " <<cfg_it.second.first <<std::endl;
                  std::vector<std::string> plugin = boost::any_cast<std::vector<std::string>>(cfg_map["plugin"].second);
                  for(auto it : plugin){
                        out_cfg<<cfg_it.first <<" = " << it <<std::endl;
                  }
                  out_cfg<<std::endl;

               }else if("producer-name" == cfg_it.first){
                     out_cfg<< "# " <<cfg_it.second.first <<std::endl;
                     std::vector<std::string> pdnames = boost::any_cast<std::vector<std::string>>(cfg_map["producer-name"].second);
                     std::vector<std::string> pdkeys = boost::any_cast<std::vector<std::string>>(cfg_map["signature-provider"].second);
                     auto pdkeys_it = pdkeys.begin();
                     for(auto pdname_it : pdnames){
                           out_cfg<<"#account detail for " << pdname_it <<std::endl;
                           out_cfg<<cfg_it.first <<"=" << pdname_it <<std::endl;
                           out_cfg<<"signature-provider = " << *pdkeys_it <<std::endl;
                           ++pdkeys_it;
                           out_cfg<<std::endl;
                     }
               }
               else{
                  out_cfg<< "# " <<cfg_it.second.first <<std::endl;
                  out_cfg<<cfg_it.first <<" = " <<  boost::any_cast<std::string>(cfg_it.second.second) <<std::endl;
                  out_cfg<<std::endl;
               }
            }
            out_cfg.close();
         }

         deploy_keypair gen_keypair(){
            auto pri_key = fc::crypto::private_key::generate<fc::ecc::private_key_shim>();
            auto pub_key = pri_key.get_public_key();

            deploy_keypair kp;
            kp.pub_key = std::string(pub_key);
            kp.pri_key = std::string(pri_key);
         
            return kp;   
         }

         std::string get_signature_provider(deploy_keypair& keypair){
            return keypair.pub_key + "=KEY:" + keypair.pri_key;
         }

         void execute_sys_cmd(std::string exe, std::vector<std::string> args, stream_type stm = stream_type_null, std::string output_path = "")
         {
            try{
               if(stream_type_null == stm){
                  bp::child c(bp::exe = exe, bp::args=args,bp::std_out>bp::null);
                  c.wait();
               }else
               if(stream_type_cout == stm){
                  bp::child c(bp::exe = exe, bp::args=args, bp::std_out>stdout);
                  c.wait();
               }else{
                  bp::child c(bp::exe = exe, bp::args=args, bp::std_out>output_path);
                  c.wait();
               }
            }catch (...) {
               FC_THROW( "Error executing system command ${exe}.", 
                  ("exe", exe)
                  ("inner", fc::except_str() )
               );
            }
         }
		 
         fc::time_point calculate_genesis_timestamp( std::string tstr ) {
            fc::time_point genesis_timestamp;
            if( strcasecmp (tstr.c_str(), "now") == 0 ) {
               genesis_timestamp = fc::time_point::now();
            } else {
               genesis_timestamp = chain::time_point::from_iso_string( tstr );
            }

            auto epoch_us = genesis_timestamp.time_since_epoch().count();
            auto diff_us = epoch_us % chain::config::block_interval_us;
            if (diff_us > 0) {
               auto delay_us = (chain::config::block_interval_us - diff_us);
               genesis_timestamp += fc::microseconds(delay_us);
            }

            return genesis_timestamp;
         }

         fc::path deploy_dir_name;
         fc::path deploy_cfg_name;
         fc::path deploy_dir_work;
         deployment_config deploy_cfg;
         deploy_accounts deploy_acc_list;
         nodeos_cfg_map nodeos_default_cfg;
   };
}

}

FC_REFLECT(eosio::deploy::producer_config,
           (name)(location)(http_server_port)(p2p_server_port)(plugins))
FC_REFLECT(eosio::deploy::deployment_config,
           (primary_node_producer)(producers))
FC_REFLECT(eosio::deploy::deploy_account,
           (name)(owner_key)(active_key))
FC_REFLECT(eosio::deploy::deploy_keypair,
           (pub_key)(pri_key))
FC_REFLECT(eosio::deploy::deploy_accounts, (copyright_uuid)(deploy_accounts))
