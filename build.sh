#!/bin/bash

target="template_debug"

if [ "$1" = "debug" ]
then
	target="template_release"
fi

py.exe -m SCons platform=windows target="$target"
rm src/*.obj
