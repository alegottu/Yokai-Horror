#include "proximity_object.h"
#include "godot_cpp/core/object.hpp"

#include <godot_cpp/core/class_db.hpp>

void ProximityObject::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("body_entered"), &ProximityObject::_body_entered);
	ClassDB::bind_method(D_METHOD("body_exited"), &ProximityObject::_body_exited);

	ADD_SIGNAL(MethodInfo("proximity_interact", PropertyInfo(Variant::OBJECT, "node")));
}

void ProximityObject::_body_entered()
{
	active = true;	
}

void ProximityObject::_body_exited()
{
	active = false;
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

void ProximityObject::_input (const Ref<InputEvent>& event)
{
	if (event->is_action_pressed("click"))
	{
		emit_signal("proximity_interact", this);
	}
}
