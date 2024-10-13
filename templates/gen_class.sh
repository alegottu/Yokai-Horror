#!/bin/bash

if [[ $# -eq 0 ]]; then
	echo "usage: gen_class ClassName InheritedType"
	exit 0
fi

SNAKE_CASE_1=$(echo "$1" | sed 's/[A-Z][a-z]/_\l&/g' | sed 's/_//')
FILE="../include/$SNAKE_CASE_1.h"
MACRO=$(echo "$SNAKE_CASE_1" | sed 's/[a-z]/\u&/g')
MACRO="$MACRO\_H"
SNAKE_CASE_2=$(echo "$2" | sed 's/[A-Z][a-z]/_\l&/g' | sed 's/_//')

cd '/mnt/d/Program Files (x86)/Godot/Projects/Yokai Horror/templates/'
cp header.h "../include/$SNAKE_CASE_1.h"
sed "s/HEADER_H/$MACRO/" -i "$FILE"
sed "s/Class/$1/g" -i "$FILE"
sed "s/Node/$2/g" -i "$FILE"
sed "s/node/$SNAKE_CASE_2/g" -i "$FILE"
