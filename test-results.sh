#!/bin/bash
set -eux
out=$PWD/TestCase_Results.txt

rm -f $out

for dir in $@
do
    pushd $dir
    make
    printf "$dir:\n" >>$out
    ./test-$(basename $dir).sh >>$out
    printf "\n" >>$out
    make clean
    popd
done
