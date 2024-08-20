#include "light_puzzle.h"
#include "godot_cpp/variant/callable.hpp"
#include "helpers.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

void LightPuzzle::_bind_methods()
{
	ADD_PRPRTY(&LightPuzzle::set_lights, &LightPuzzle::get_lights, PropertyInfo(Variant::ARRAY, "lights"));
}

LightPuzzle::LightPuzzle()
{
}

LightPuzzle::~LightPuzzle()
{
	delete[] _lights;
}

void LightPuzzle::_ready()
{
	_lights = new Light3D*[lights.size()];

	for (int i = 0; i < lights.size(); ++i)
	{
		_lights[i] = get_node<Light3D>((NodePath)lights.pop_back());
		Area3D* area = (Area3D*)_lights[i]->get_child(0); // Ensure the right parent-child hierarchy
		area->connect("body_entered", Callable(this, "body_entered"));
	}
}
