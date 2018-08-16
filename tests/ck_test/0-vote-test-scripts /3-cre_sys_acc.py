#!/usr/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print

FIRM = 'eosio'

systemAccounts = ['.bpay', '.msig', '.names', '.ram', '.ramfee', '.saving', '.stake', '.token', '.vpay', '.uvpay','.bchain']
for name in systemAccounts:
    f('cleos create account ' + FIRM + ' ' + FIRM + name + ' EOS6An9TxNk3mqKnRX9y92UJbWXEPSUxLD2PEtD9eTZfuBeHxBDpD')
    time.sleep(0.1)
