#!/usr/local/bin/python3

import os
import time

debug = 0
if debug == 0:
    f = os.system
else:
    f = print


for line in open('bp_keys'):
    l = line.strip().split(',')
    f('cleos system regproducer ' + l[0] +' ' + l[4])
    print("producer: "+l[0]+ " register ok!")
    time.sleep(0.2)