#!/bin/sh
set -e

echo > parport.out && bochs -q -rc bochs_commands.txt
