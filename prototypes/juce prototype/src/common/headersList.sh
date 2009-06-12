#!/bin/bash
ls *.h | cat | grep -v -e common.h$ | while read x; do echo \#include \"${x}\"; done;
