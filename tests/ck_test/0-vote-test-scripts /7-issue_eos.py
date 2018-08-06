#!/usr/local/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print


bpaccList = []

for line in open('bp_keys'):
    bpaccName = line[:line.find(',')]
    bpaccList.append(bpaccName)


#transfer EOT to bp accounts from eosio

amount = "20000.0000 EOS"
for bpaccount in bpaccList:
    f('cleos transfer eosio '+ bpaccount +''' "'''+ amount+'''"''')
    time.sleep(0.2)



voaccList = []

for line1 in open('vo_keys'):
    voaccName = line1[:line1.find(',')]
    voaccList.append(voaccName)



# transfer EOT to voters account from eosio

amount = "20000000.0000 EOS"
for voaccount in voaccList:
    f('cleos transfer eosio '+ voaccount +''' "'''+ amount+'''"''')
    time.sleep(0.2)