#ifndef HELPERS_H
#define HELPERS_H

#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

// profile is it would be better to create a member variable for this
// as this creates a new vector every time it's used
#define VECTOR3_ZERO Vector3(0.0f, 0.0f, 0.0f)
#define GRAVITY Vector3(0.0f, -9.8f, 0.0f)
#define GRAVITY_Y -9.8f

#include <string>

template <typename N, typename M>
void add_property(const StringName& type, const N setter, const M getter, const PropertyInfo& properties) 
{
	std::string name = properties.name.get_basename().utf8().get_data();
	std::string setter_name = "set_" + name;
	std::string getter_name = "get_" + name;
	std::string param_name = "p_" + name;
	StringName _setter_name = setter_name.c_str();
	StringName _getter_name = getter_name.c_str();

	ClassDB::bind_method(D_METHOD( _setter_name, StringName(param_name.c_str()) ), setter);
	ClassDB::bind_method(D_METHOD(_getter_name), getter);
	ClassDB::add_property(type, properties, _setter_name, _getter_name);
}

#define ADD_PRPRTY(setter, getter, properties) add_property(get_class_static(), setter, getter, properties)

#endif // HELPERS_H
