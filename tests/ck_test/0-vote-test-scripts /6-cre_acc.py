#!/usr/local/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

for line in open('acc_keys'):
    l = line.strip().split(',')
    f('''cleos system newaccount --transfer --stake-net "100.0000 EOS" --stake-cpu "100.0000 EOS" --buy-ram "100.0000 SN" eosio ''' + l[0] + ' ' + l[2] + ' ' + l[4])
    print("create account "+ l[0] +" ok!")
    time.sleep(0.2)



    



