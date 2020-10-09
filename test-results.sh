#!/bin/bash
set -eux
out=$PWD/TestCase_Results.txt

rm -f $out

for script in $@
do
    pushd $(dirname $script)

    if [[ -r Makefile ]]
    then
	make
    fi

    printf "$script:\n" >>$out
    ./$(basename $script) >>$out
    printf "\n" >>$out
    git clean -dfx
    popd
done
