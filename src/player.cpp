#include "player.h"
#include "helpers.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/canvas_item.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/core/math.hpp>

void Player::_bind_methods()
{
	ADD_PRPRTY(&Player::set_speed, &Player::get_speed, PropertyInfo(Variant::FLOAT, "speed"));
	ADD_PRPRTY(&Player::set_sensitivity, &Player::get_sensitivity, PropertyInfo(Variant::FLOAT, "sensitivity"));
	ADD_PRPRTY(&Player::set_return_rate, &Player::get_return_rate, PropertyInfo(Variant::FLOAT, "return_rate"));
	ADD_PRPRTY(&Player::set_return_margin, &Player::get_return_margin, PropertyInfo(Variant::FLOAT, "return_margin"));
	ADD_PRPRTY(&Player::set_spirit, &Player::get_spirit, PropertyInfo(Variant::NODE_PATH, "spirit"));
	ADD_PRPRTY(&Player::set_camera, &Player::get_camera, PropertyInfo(Variant::NODE_PATH, "camera"));
	ADD_PRPRTY(&Player::set_camera_pivot, &Player::get_camera_pivot, PropertyInfo(Variant::NODE_PATH, "camera_pivot"));
	ADD_PRPRTY(&Player::set_sub_viewport, &Player::get_sub_viewport, PropertyInfo(Variant::NODE_PATH, "sub_viewport"));
}

Player::Player()
	: speed(1.0f), sensitivity(1.0f), return_rate(0.1f),
	return_margin(0.1f), spirit_active(false), spirit_return(false)
{
}

Player::~Player()
{
}

void Player::_ready()
{
	if (Engine::get_singleton()->is_editor_hint())
		set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
	else
	 	set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);

	InputMap::get_singleton()->load_from_project_settings();

	_spirit = spirit.is_empty() ? this : get_node<CharacterBody3D>(spirit);
	spirit_start = _spirit->get_position();
	starting_distance = _spirit->get_position().length();
	_camera = camera.is_empty() ? Object::cast_to<Node3D>(this) : get_node<Node3D>(camera);
	_camera_pivot = camera_pivot.is_empty() ? Object::cast_to<Node3D>(this) : get_node<Node3D>(camera_pivot);

	if (!sub_viewport.is_empty())
	{
		sub_viewport_mat = get_node<CanvasItem>(sub_viewport)->get_material();
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

	if (event->is_action_pressed("switch"))
	{
		if (!spirit_active)
			spirit_last_rotation = _spirit->get_rotation();

		spirit_return = spirit_active;
		spirit_active = !spirit_active;
	}
}

void Player::_physics_process(const double delta)
{
	Vector3 spirit_pos = _spirit->get_position();

	if (!sub_viewport_mat.is_null())
	{
		float distance = spirit_pos.length() - starting_distance;
		sub_viewport_mat->set_shader_parameter("player_distance", distance);
	}

	// test if this can be optimized with coroutine
	if (spirit_return)
	{
		Vector3 lerp_pos = spirit_pos.lerp(spirit_start, return_rate);
		Vector3 lerp_rotation = _spirit->get_rotation().lerp(spirit_last_rotation, return_rate);
		_spirit->set_position(lerp_pos);
		_spirit->set_rotation(lerp_rotation);

		if (lerp_pos.distance_to(spirit_start) <= return_margin
			&& Math::floor(lerp_rotation.distance_to(spirit_last_rotation)) <= return_margin)
		{
			_spirit->set_position(spirit_start);
			_spirit->set_rotation(spirit_last_rotation);
			spirit_return = false;
		}

		// without this order, there was previously a bug where you could remove while returning;
		// this allowed you to change your camera position while moving as the player, which could be interesting
		return;
	}

	Vector2 input = Input::get_singleton()->get_vector("left", "right", "forward", "back");
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
