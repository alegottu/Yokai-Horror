#include "detection_sphere.h"
#include "player.h"

#include <godot_cpp/classes/ray_cast3d.hpp>

void DetectionSphere::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_radius", "p_radius"), &DetectionSphere::set_radius);
	ClassDB::bind_method(D_METHOD("get_radius"), &DetectionSphere::get_radius);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius"), "set_radius", "get_radius");

	// Later change this to find node from group
	ClassDB::bind_method(D_METHOD("set_player", "p_player"), &DetectionSphere::set_player);
	ClassDB::bind_method(D_METHOD("get_player"), &DetectionSphere::get_player);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "player"), "set_player", "get_player");

	ClassDB::bind_method(D_METHOD("set_sound_threshold", "p_sound_threshold"), &DetectionSphere::set_sound_threshold);
	ClassDB::bind_method(D_METHOD("get_sound_threshold"), &DetectionSphere::get_sound_threshold);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sound_threshold"), "set_sound_threshold", "get_sound_threshold");
	
	ClassDB::bind_method(D_METHOD("set_sound_meter", "p_sound_meter"), &DetectionSphere::set_sound_meter);
	ClassDB::bind_method(D_METHOD("get_sound_meter"), &DetectionSphere::get_sound_meter);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sound_meter"), "set_sound_meter", "get_sound_meter");
	
	ClassDB::bind_method(D_METHOD("set_sound_damp", "p_sound_damp"), &DetectionSphere::set_sound_damp);
	ClassDB::bind_method(D_METHOD("get_sound_damp"), &DetectionSphere::get_sound_damp);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "sound_damp"), "set_sound_damp", "get_sound_damp");
}

void DetectionSphere::_noise_made(unsigned char noise_level)
{
	RayCast3D rays[2]; // Check at foot and head level
	// Change these later to get the position of empty nodes at set levels
	rays[0].set_position(get_position() - Vector3(0, 1, 0));
	rays[0].set_position(get_position() + Vector3(0, -1, 0));
	bool line_of_sight = true;
	 
	for (unsigned char i = 0; i < 2; ++i)
	{
		// ignore everything first; this includes sound passthrough layer for thin objects
		rays[i].set_collision_mask(0); 
		rays[i].set_collision_mask_value(1, true); // allow player layer
		rays[i].set_collision_mask_value(0, true); // allow default layer, potentially blocking player
		rays[i].set_target_position(player->get_position());
	
		if (rays[i].is_colliding())
		{
			line_of_sight = false;
			break;
		}
	}

	if (!line_of_sight)
		noise_level /= sound_damp;

	sound_meter += noise_level;

	/*
	if (sound_meter > sound_threshold)
		do something in specific enemy class, or fire signal
	*/
}

void DetectionSphere::_ready()
{
	vision = (VisionCone*)get_child(0); // vision should be 1 and only child of this node
	Player* _player = (Player*)player;
	_player->connect("noise_made", Callable(this, "_noise_made"));
}
