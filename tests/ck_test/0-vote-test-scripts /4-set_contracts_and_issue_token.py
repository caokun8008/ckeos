#!/usr/local/bin/python3

import os
import time

debug = 1
if debug == 1:
    f = os.system
else:
    f = print

FIRM = 'eosio'
CONTRACT_PATH = ' ~/eos/build/contracts/'
CONTRACT_MSIG = FIRM + '.msig'
CONTRACT_TOKEN = FIRM + '.token'

f('cleos set contract ' + CONTRACT_TOKEN + CONTRACT_PATH + CONTRACT_TOKEN + '/')
f('cleos set contract ' + CONTRACT_MSIG + CONTRACT_PATH + CONTRACT_MSIG + '/')
f('cleos push action ' + CONTRACT_TOKEN + ''' create '["''' + FIRM + '''", "10000000000.0000 EOS"]' -p ''' + CONTRACT_TOKEN)
f('cleos push action ' + CONTRACT_TOKEN + ''' issue '["''' + FIRM + '''", "1000000000.0000 EOS", "memo"]' -p ''' + FIRM)
time.sleep(0.5)
f('cleos set contract ' + FIRM + CONTRACT_PATH + FIRM + '.system/')
time.sleep(0.5)
f('cleos push action ' + FIRM + ''' setpriv '["''' + CONTRACT_MSIG + '''", 1]' -p ''' + FIRM + '@active')