#include "proximity_object.h"

#include <godot_cpp/core/class_db.hpp>

void ProximityObject::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("body_entered"), &ProximityObject::body_entered);
	ClassDB::bind_method(D_METHOD("body_exited"), &ProximityObject::body_exited);
}

ProximityObject::ProximityObject()
	: active(false)
{
	this->connect("body_entered", Callable(this, "body_entered"));
	this->connect("body_exited", Callable(this, "body_exited"));
}

ProximityObject::~ProximityObject()
{
}

void ProximityObject::body_entered()
{
	active = true;	
}

void ProximityObject::body_exited()
{
	active = false;
}
