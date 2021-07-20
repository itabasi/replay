#!/usr/bin/python3



# Toshiyuki Gogami
# Nov 2, 2018
# Modified by itabashi

import sys
import time, os.path
from subprocess import call
#import concurrent.futures
from logging import StreamHandler, Formatter, INFO, getLogger
from concurrent.futures import ThreadPoolExecutor
from concurrent.futures.process import ProcessPoolExecutor 
import numpy as np
import shlex
import subprocess
import re
import sys
import os,os.path
import glob

nworkers=6

init=111111
end=111843

#####################################
root_dir="/data3/root_ole/root3"
file_init="tritium_"
#####################################

def check(RUNNUM):
    i=0
    for f in glob.glob(f"{root_dir}/tritium_{RUNNUM}*"):
        i=i+1
    return i



def cmd(RUNNUM):
    if check(RUNNUM)>0 :
        print(f"skipped analyzer in #{RUNNUM}")
    else:
        print(f"go anlayzer #{RUNNUM}")
        rep =f' analyzer -l \"replay_coinc_new.C({RUNNUM})\" '
        subprocess.run([rep],shell=True)

    

def main():


    with ProcessPoolExecutor(max_workers=nworkers) as executor:
        for RUNNUM in range(init, end):
            mappings = {executor.submit(cmd,RUNNUM)}

    
main()


