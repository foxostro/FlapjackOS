#!/bin/sh
gcc -P -E - $3 -o "$2" < "$1"
exit $?
