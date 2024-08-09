#include "player.h"
#include "helpers.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/core/math.hpp>

using namespace godot;

void Player::_bind_methods()
{
	ADD_PRPRTY(&Player::set_speed, &Player::get_speed, PropertyInfo(Variant::FLOAT, "speed"));
	ADD_PRPRTY(&Player::set_sensitivity, &Player::get_sensitivity, PropertyInfo(Variant::FLOAT, "sensitivity"));
	ADD_PRPRTY(&Player::set_return_rate, &Player::get_return_rate, PropertyInfo(Variant::FLOAT, "return_rate"));
	ADD_PRPRTY(&Player::set_spirit, &Player::get_spirit, PropertyInfo(Variant::NODE_PATH, "spirit"));
	ADD_PRPRTY(&Player::set_camera, &Player::get_camera, PropertyInfo(Variant::NODE_PATH, "camera"));
	ADD_PRPRTY(&Player::set_camera_pivot, &Player::get_camera_pivot, PropertyInfo(Variant::NODE_PATH, "camera_pivot"));
}

Player::Player()
	: speed(1.0f), sensitivity(1.0f), return_rate(0.1f),
	spirit_active(false), spirit_return(false),
	spirit(NodePath()), camera(NodePath()), camera_pivot(NodePath()),
	_spirit(nullptr), _camera(nullptr), _camera_pivot(nullptr)
{
}

Player::~Player()
{
}

void Player::_ready()
{
	InputMap::get_singleton()->load_from_project_settings();

	_spirit = spirit.is_empty() ? this : get_node<CharacterBody3D>(spirit);
	spirit_start = _spirit->get_position();
	_camera = camera.is_empty() ? Object::cast_to<Node3D>(this) : get_node<Node3D>(camera);
	_camera_pivot = camera_pivot.is_empty() ? Object::cast_to<Node3D>(this) : get_node<Node3D>(camera_pivot);
}

void Player::_process(double delta)
{
	Input* Input = Input::get_singleton();

	if (Input->is_action_just_pressed("switch"))
	{
		if (!spirit_active)
			spirit_rotation = _spirit->get_rotation();

		spirit_return = spirit_active;
		spirit_active = !spirit_active;
	}

	// test if this can be optimized with coroutine
	if (spirit_return)
	{
		Vector3 lerp_pos = _spirit->get_position().lerp(spirit_start, return_rate);
		Vector3 lerp_rotation = _spirit->get_rotation().lerp(spirit_rotation, return_rate);
		_spirit->set_position(lerp_pos);
		_spirit->set_rotation(lerp_rotation);

		if (lerp_pos == spirit_start && lerp_rotation == spirit_rotation)
		{
			spirit_return = false;
		}
	}

	Vector2 input = Input->get_vector("left", "right", "forward", "back");
	Vector3 delta_pos = _camera->get_global_basis().xform(Vector3(input.x, 0.0f, input.y));
	delta_pos.normalize();
	delta_pos *= speed;
	
	if (!spirit_active)
	{
		delta_pos = Vector3(delta_pos.x, GRAVITY_Y, delta_pos.z);
		_spirit->set_velocity(VECTOR3_ZERO);
		set_velocity(delta_pos);
		move_and_slide();
	}
	else 
	{
		set_velocity(VECTOR3_ZERO);
		_spirit->set_velocity(delta_pos);
		_spirit->move_and_slide();
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
			_camera_pivot->rotate_y(-movement.x * _sensitivity);
		else
			_spirit->rotate_y(-movement.x * _sensitivity);

		_camera->rotate_x(-movement.y * _sensitivity);
		Vector3 rotation = _camera->get_rotation();
		double clamped_x = Math::clamp<double>(rotation.x, Math::deg_to_rad(-45.0), Math::deg_to_rad(90.0));
		_camera->set_rotation(Vector3(clamped_x, rotation.y, rotation.z));
	}
}
