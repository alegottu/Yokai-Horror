#!/bin/bash

if [[ $# -eq 0 ]]; then
	echo "usage: gen_property property_name type : for header file code 
gen_property -s class_name property_name type : for source file code"
	exit 0
fi

if [[ "$1" == "--src" ]] || [[ "$1" == "-s" ]]; then
	TYPE_CAPS=$(echo "$4" | sed 's/[A-Z][a-z]/_\l&/g' | sed 's/_//')
	PARAM_NAME="\"p_$3\""
	SETTER_NAME="\"set_$3\""
	GETTER_NAME="\"get_$3\""
	SETTER_SIG="$2::set_$3"
	GETTER_SIG="$2::get_$3"

	if [[ "$TYPE_CAPS" == "$4" ]]; then
		TYPE_CAPS=$(echo "$4" | sed 's/[a-z]/\u&/g')
	fi

	echo "void $2::_bind_methods()
{
	ClassDB::bind_method(D_METHOD($SETTER_NAME, $PARAM_NAME), &$SETTER_SIG);
	ClassDB::bind_method(D_METHOD($GETTER_NAME), &$GETTER_SIG);
	ADD_PROPERTY(PropertyInfo(Variant::$TYPE_CAPS, \"$3\"), $SETTER_NAME, $GETTER_NAME);
}"
else
	echo "void set_$1(const $2 p_$1) { $1 = p_$1; }
$2 get_$1() const { return $1; }"
fi
