#!/bin/bash

target="template_debug"
opts=""

if [[ "$1" = "release" ]]
then
	target="template_release"
elif [[ "$1" = "--slow" || "$1" = "-s" ]]
then
	opts="-j 1"
fi

py.exe -m SCons platform=windows target="$target" "$opts"
rm src/*.obj
