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

nworkers=5
init=111200
end=111205
root_dir='/data/root_ole/copy_to_farm'
#root_dir='/data/root_ole'
def scp(RUNNUM):
    scp =f'rsync -arv --progress {root_dir}/tritium_{RUNNUM}.root farmio:/data/11b/itabashi/root_ole/tritium_{RUNNUM}.root'
    nsub=len(glob.glob1(root_dir,f"tritium_{RUNNUM}*"))
    subprocess.run([scp],shell=True)
    for i in range(1,int(nsub)):
        scp_sub =f'rsync -arv --progress {root_dir}/tritium_{RUNNUM}_{i}.root farmio:/data/11b/itabashi/root_ole/tritium_{RUNNUM}_{i}.root &'
        subprocess.run([scp_sub],shell=True)


def rm(RUNNUM):
    nsub=len(glob.glob1(root_dir,f"tritium_{RUNNUM}*"))
    rm=f'rm -f {root_dir}/tritium_{RUNNUM}.root'
    subprocess.run([rm],shell=True)
    for i in range(1,int(nsub)):
        rm_sub=f'rm -f {root_dir}/tritium_{RUNNUM}_{i}.root &'
        subprocess.run([rm_sub],shell=True)


def cmd(RUNNUM):
    rep =f' analyzer -l \"replay_coinc_new.C({RUNNUM})\" '
    subprocess.run([rep],shell=True)

def job(RUNNUM):    
    cmd(RUNNUM)
    scp(RUNNUM)
    rm(RNNUM)
def main():
    for RUNNUM in range(init, end):
        with ProcessPoolExecutor(max_workers=nworkers) as executor:
            mappings = {executor.submit(job,RUNNUM)}
            #            mappings = {executor.submit(cmd,RUNNUM)}
            #            mappings = {executor.submit(scp,RUNNUM)}

    
main()

