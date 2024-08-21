#include "light_puzzle.h"
#include "helpers.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/light3d.hpp>

using namespace godot;

void LightPuzzle::_bind_methods()
{
	ADD_PRPRTY(&LightPuzzle::set_light_areas, &LightPuzzle::get_light_areas, PropertyInfo(Variant::ARRAY, "light_areas"));
	ClassDB::bind_method(D_METHOD("_body_entered"), &LightPuzzle::_body_entered, PropertyInfo(Variant::OBJECT, "node"));
	ClassDB::bind_method(D_METHOD("_body_exited"), &LightPuzzle::_body_exited, PropertyInfo(Variant::OBJECT, "_"));
}

LightPuzzle::LightPuzzle()
	: active_light(0), next_light(1), lights_on(0)
{
}

LightPuzzle::~LightPuzzle()
{
	delete[] _light_areas;
}

void LightPuzzle::_ready()
{
	// profile if num_lights needed
	num_lights = light_areas.size();
	_light_areas = new Area3D*[num_lights];

	for (int i = 0; i < num_lights; ++i)
	{
		Area3D* area = get_node<Area3D>((NodePath)light_areas[i]);
		_light_areas[i] = area;
		area->connect("body_entered", Callable(this, "_body_entered"));
		area->connect("body_exited", Callable(this, "_body_exited"));
	}
}

void LightPuzzle::_input(const Ref<InputEvent>& event) 
{
	if (active_light > 0 && event->is_action_pressed("click"))
	{
		Area3D* area = _light_areas[active_light-1];
		bool visible = area->is_visible();
		area->set_visible(true);

		if (!visible)
		{
			if (active_light == next_light && next_light++ == num_lights)
			{
				for (int i = 0; i < num_lights; ++i)
				{
					// ensure correct parent-child setup
					Light3D* light = (Light3D*)_light_areas[i]->get_child(1);
					light->set_color(Color(0.0f, 1.0f, 0.0f, 1.0f));
				}
			}
			else if (++lights_on == num_lights)
			{
				for (int i = 0; i < num_lights; ++i)
				{
					_light_areas[i]->set_visible(false);
				}

				lights_on = 0;
				next_light = 1;
			}
		}
	}
}

void LightPuzzle::_body_entered(const Node3D* node)
{
	// should be small amount of lights, otherwise handle using signal from ProximityObject
	for (int i = 0; i < num_lights; ++i)
  	{
		// choose whether to use (Node) or Object::cast_to
		if (_light_areas[i]->overlaps_body((Node*)node))
		{
			active_light = i+1;
		}
  	}
}

void LightPuzzle::_body_exited(const Node3D* _)
{
	active_light = 0;
}
