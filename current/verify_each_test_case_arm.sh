#!/bin/bash

# Copyright (c) 2023-2025 Dan O’Malley
# This file is licensed under the MIT License. See LICENSE for details.


set -e

for FILE in ./test-cases/arm/return-int/test-case-*.c
do
    make arm prog=$FILE type=return-int
    read -n 1 -s -r -p "Press any key to continue"
    make clean
    tests_performed=$((tests_performed+1))
done

for FILE in ./test-cases/arm/pass-int/test-case-*.c
do
    make arm prog=$FILE type=pass-int
    read -n 1 -s -r -p "Press any key to continue"
    make clean
    tests_performed=$((tests_performed+1))
done


echo
echo
echo " Tests performed: $tests_performed "
echo " ***** ALL TESTS PASSED!! ***** "
echo
echo