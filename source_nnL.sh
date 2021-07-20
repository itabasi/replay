
########################################
###### nnL machine path setting ########
########################################
export ANALYZER=/usr/local/jlab/Ole/analyzer-install
export ROOTSYS=/usr/local/cern/root/root-6.18.00
export GCC=/bin/gcc
export LD_LIBRARY_PATH=$ANALYZER/lib64:$ROOTSYS/lib:$GCC/lib:$GCC/lib64:$LD_LIBRARY_PATH:/usr/local/lib
export PATH=$ANALYZER/bin:$ROOTSYS/bin:$GCC/bin:$PATH
export DB_DIR='pwd'/DB

