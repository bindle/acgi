#!/bin/sh

cd `dirname ${0}` || exit 1
cd .. || exit 1

for FILE in acinclude.m4 configure.ac Makefile.am;do
   test -f ${FILE} || { echo "${0}: missing \"${FILE}\""; exit 1; }
done

if test -d ./.git || test -f ./.git;then
   git submodule init
   git submodule sync
   git submodule update
fi

autoreconf -f -i -I ./contrib/bindletools/m4/ -Wall -Werror || exit 1

# end of script
