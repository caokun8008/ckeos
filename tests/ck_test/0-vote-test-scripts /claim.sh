#create system accounts
cleos create account eosio eosio.bpay EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.msig EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.names EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.ram EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.ramfee EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.saving EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.stake EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.token EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.vpay EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
cleos create account eosio eosio.uvpay EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD
#set token contract,and msig contract
cleos set contract eosio.token ~/eos/build/contracts/eosio.token/
cleos set contract eosio.msig ~/eos/build/contracts/eosio.msig/
#create and issue EOS token
cleos push action eosio.token create '["eosio", "10000000000.0000 EOS"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "1000000000.0000 EOS", "memo"]' -p eosio
#get balance for eosio
cleos get account eosio
cleos get currency balance eosio.token eosio

sleep 0.5
#set system contract and 
cleos set contract eosio ~/eos/build/contracts/eosio.system/
sleep 0.5

#set msig priv
cleos push action eosio setpriv '["eosio.msig", 1]' -p eosio@active
#create and issue SN token
cleos push action eosio.token create '["eosio", "10000000000.0000 SN"]' -p eosio.token
cleos push action eosio.token issue '["eosio", "1000000000.0000 SN", "memo"]' -p eosio

#get balance for eosio
cleos get account eosio
cleos get currency balance eosio.token eosio

#create accounts
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio cktest1  EOS7tSY5od1Hu7hMvParp3VfWzXyhvhiCPZjq1XMDWZb7s84C1n1F
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio cktest2  EOS6cmDXn5mqxe9L5n1jB4TSfgZd8b2jbtNibe8EHrygjrmLf4oRN
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio cktest3  EOS7CRLbXJNsQvUCcGggAqoqbco8tydpMYZiPLv8A1v3FFV8rwauL
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio cktest4  EOS6oxALYnrJveHMTBYPq31oZ7Hwye1Pa1PvdYgJgCDkWm167GeAn
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio voter1  EOS6537XnYata5quZyhz71QgkSmZ7h9nSR5gw15jFJppZVBavZrKq
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio voter2  EOS7kKW1pwnoQVBakuHjRNm9yHo2ovsnswqnLjod8agbQycXTL2yP
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio voter3  EOS6CV1CV1sLrp5qSMFnzznybTYaE4rqQSNQcQziVDZcW36MDjxE3
cleos system newaccount --transfer --stake-net "10.0000 EOS" --stake-cpu "10.0000 EOS" --buy-ram "10.0000 SN" eosio voter4  EOS6fXvaDopGHMAPqFdqDqjnyufTbLn7wAiBTFrAyPVsE5sSbz6LD


#get balance for eosio
cleos get currency balance eosio.token cktest1
cleos get currency balance eosio.token cktest2
cleos get currency balance eosio.token cktest3
cleos get currency balance eosio.token cktest4
cleos get currency balance eosio.token voter1
cleos get currency balance eosio.token voter2
cleos get currency balance eosio.token voter3
cleos get currency balance eosio.token voter4

#transfer tokens from eosio to test accounts
cleos transfer eosio voter1 "100000000.0000 EOS"
cleos transfer eosio voter2 "100000000.0000 EOS"
cleos transfer eosio voter3 "100000000.0000 EOS"
cleos transfer eosio voter4 "100000000.0000 EOS"
cleos transfer eosio cktest1 "10000.0000 EOS"
cleos transfer eosio cktest2 "10000.0000 EOS"
cleos transfer eosio cktest3 "10000.0000 EOS"
cleos transfer eosio cktest4 "10000.0000 EOS"

#get account test
cleos get account eosio
cleos get account cktest1
cleos get account cktest2
cleos get account cktest3
cleos get account cktest4
cleos get account voter1
cleos get account voter2
cleos get account voter3
cleos get account voter4

#get balance
cleos get currency balance eosio.token cktest1
cleos get currency balance eosio.token cktest2
cleos get currency balance eosio.token cktest3
cleos get currency balance eosio.token cktest4
cleos get currency balance eosio.token voter1
cleos get currency balance eosio.token voter2
cleos get currency balance eosio.token voter3
cleos get currency balance eosio.token voter4

#regprocuer
cleos system listproducers
cleos system regproducer cktest1 EOS7tSY5od1Hu7hMvParp3VfWzXyhvhiCPZjq1XMDWZb7s84C1n1F
cleos system regproducer cktest2 EOS6cmDXn5mqxe9L5n1jB4TSfgZd8b2jbtNibe8EHrygjrmLf4oRN
cleos system regproducer cktest3 EOS7CRLbXJNsQvUCcGggAqoqbco8tydpMYZiPLv8A1v3FFV8rwauL
cleos system regproducer cktest4 EOS6oxALYnrJveHMTBYPq31oZ7Hwye1Pa1PvdYgJgCDkWm167GeAn
cleos system listproducers
#stake
cleos system delegatebw voter1 voter1 '10000000 EOS' '10000000 EOS'
cleos system delegatebw voter2 voter2 '20000000 EOS' '20000000 EOS'
cleos system delegatebw voter3 voter3 '30000000 EOS' '30000000 EOS'
cleos system delegatebw voter4 voter4 '40000000 EOS' '40000000 EOS'
cleos get currency balance eosio.token cktest1
cleos get currency balance eosio.token cktest2
cleos get currency balance eosio.token cktest3
cleos get currency balance eosio.token cktest4
cleos get currency balance eosio.token voter1
cleos get currency balance eosio.token voter2
cleos get currency balance eosio.token voter3
cleos get currency balance eosio.token voter4

#proxy voting
cleos system voteproducer prods voter1 cktest1
cleos system voteproducer prods voter2 cktest2
cleos system voteproducer prods voter3 cktest3
cleos system voteproducer prods voter4 cktest4

#select listproducers and listbw
cleos system listproducers