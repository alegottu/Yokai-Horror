#include "player.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

void Player::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Player::set_speed);
	ClassDB::bind_method(D_METHOD("get_speed"), &Player::get_speed);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

	ClassDB::bind_method(D_METHOD("set_sensitivity", "p_sensitivity"), &Player::set_sensitivity);
	ClassDB::bind_method(D_METHOD("get_sensitivity"), &Player::get_sensitivity);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sensitivity"), "set_sensitivity", "get_sensitivity");
	
	ClassDB::bind_method(D_METHOD("set_return_rate", "p_return_rate"), &Player::set_return_rate);
	ClassDB::bind_method(D_METHOD("get_return_rate"), &Player::get_return_rate);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "return_rate", PROPERTY_HINT_RANGE, "0,1.0,0.001"), "set_return_rate", "get_return_rate");

	ClassDB::bind_method(D_METHOD("set_cam", "p_cam"), &Player::set_cam);
	ClassDB::bind_method(D_METHOD("get_cam"), &Player::get_cam);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "cam"), "set_cam", "get_cam");

	ClassDB::bind_method(D_METHOD("set_cam_pivot", "p_cam_pivot"), &Player::set_cam_pivot);
	ClassDB::bind_method(D_METHOD("get_cam_pivot"), &Player::get_cam_pivot);
	ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "cam_pivot"), "set_cam_pivot", "get_cam_pivot");
}

Player::Player()
{
	speed = 1.0;
	sensitivity = 1.0;
	return_rate = 0.1;
	spirit_active = false;
	spirit_return = false;
}

Player::~Player()
{
}

void Player::set_speed(const float p_speed)
{
	speed = p_speed;
}

float Player::get_speed() const
{
	return speed;
}

void Player::set_sensitivity(const float p_sensitivity)
{
	sensitivity = p_sensitivity;
}

float Player::get_sensitivity() const
{
	return sensitivity;
}

void Player::set_return_rate(const float p_return_rate)
{
	return_rate = p_return_rate;
}

float Player::get_return_rate() const
{
	return return_rate;
}

void Player::set_cam(const NodePath p_cam)
{
	cam = p_cam;
}

NodePath Player::get_cam() const
{
	return cam;
}

void Player::set_cam_pivot(const NodePath p_cam_pivot)
{
	cam_pivot = p_cam_pivot;
}

NodePath Player::get_cam_pivot() const
{
	return cam_pivot;
}

void Player::_ready()
{
	InputMap::get_singleton()->load_from_project_settings();

	_cam = get_node<Node3D>(cam);
	_cam_pivot = get_node<Node3D>(cam_pivot); 
	spirit = _cam->get_parent_node_3d();
	spirit_start = spirit->get_position();
}

void Player::_process(double delta)
{
	Input* Input = Input::get_singleton();

	if (Input->is_action_just_pressed("switch"))
	{
		if (!spirit_active)
			spirit_rotation = spirit->get_rotation();

		spirit_return = spirit_active;
		spirit_active = !spirit_active;
	}

	// test if this can be optimized with coroutine
	if (spirit_return)
	{
		Vector3 lerp_pos = spirit->get_position().lerp(spirit_start, return_rate);
		Vector3 lerp_rotation = spirit->get_rotation().lerp(spirit_rotation, return_rate);
		spirit->set_position(lerp_pos);
		spirit->set_rotation(lerp_rotation);

		if (lerp_pos == spirit_start && lerp_rotation == spirit_rotation)
		{
			spirit_return = false;
		}
	}

	Vector2 input = Input->get_vector("left", "right", "forward", "back");
	Vector3 delta_pos = _cam->get_basis().xform(Vector3(input.x, 0.0, input.y));
	delta_pos.normalize();
	delta_pos *= delta * speed;
	
	if (!spirit_active)
	{
		delta_pos *= Vector3(1.0, 0.0, 1.0);
		translate(delta_pos);
	}
	else 
	{
		spirit->translate(delta_pos);
	}
}

void Player::_input(const Ref<InputEvent>& event)
{
	Ref<InputEventMouseMotion> mouse_move = event;

	if (mouse_move.is_valid())
	{
		double _sensitivity = sensitivity / 100;
		Vector2 movement = mouse_move->get_relative();

		if (!spirit_active)
			_cam_pivot->rotate_y(-movement.x * _sensitivity);
		else
			spirit->rotate_y(-movement.x * _sensitivity);

		_cam->rotate_x(-movement.y * _sensitivity);
		Vector3 rotation = _cam->get_rotation();
		double clamped_x = Math::clamp<double>(rotation.x, Math::deg_to_rad(-45.0), Math::deg_to_rad(90.0));
		_cam->set_rotation(Vector3(clamped_x, rotation.y, rotation.z));
	}
}
