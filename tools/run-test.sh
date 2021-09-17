#!/bin/sh

# Note: run-qemu and run-gdb want to be invoked from within the
# "tools" dir, so we have to chdir there. But first we have to resolve
# our arguments to their absolute paths so that they remain valid:
TOOLSDIR=$(dirname $(readlink -f  $0)) # $0 is this shell script
SOURCEFILE=$(readlink -f  $1)          # $1 is the test kmain.c
SCRIPTFILE=$(readlink -f  $2)          # $2 is the gdb test script

kill_qemu() { killall -9 qemu-system-arm > /dev/null 2>&1 ;}

cd $TOOLSDIR
cd .. ; make -B KMAIN=$SOURCEFILE || exit 1

cd $TOOLSDIR

kill_qemu
./run-qemu.sh > /dev/null 2>&1 &

./run-gdb.sh $SCRIPTFILE

kill_qemu

