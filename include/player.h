#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class Player : public CharacterBody3D
{
	GDCLASS(Player, CharacterBody3D)

private:
	// Inspector properties
	float speed;
	float sensitivity;
	float return_rate;
	NodePath spirit;
	NodePath camera;
	NodePath camera_pivot;
	NodePath sub_viewport;

	CharacterBody3D* _spirit;
	Node3D* _camera;
	Node3D* _camera_pivot;
	Ref<ShaderMaterial> sub_viewport_mat;
	Vector3 spirit_start;
	Vector3 spirit_last_rotation;
	float starting_distance;
	bool spirit_active;
	bool spirit_return;

protected:
	static void _bind_methods();

public:
	Player();
	~Player();

	// Node property functions
	void set_speed(const float p_speed) { speed = p_speed; }
	float get_speed() const { return speed; }
	void set_sensitivity(const float p_sensitivity) { sensitivity = p_sensitivity; }
	float get_sensitivity() const { return sensitivity; }
	void set_return_rate(const float p_return_rate) { return_rate = p_return_rate; }
	float get_return_rate() const { return return_rate; }
	void set_spirit(const NodePath p_spirit) { spirit = p_spirit; }
	NodePath get_spirit() const { return spirit; }
	void set_camera(const NodePath p_camera) { camera = p_camera; }
	NodePath get_camera() const { return camera; }
	void set_camera_pivot(const NodePath p_camera_pivot) { camera_pivot = p_camera_pivot; }
	NodePath get_camera_pivot() const { return camera_pivot; }
	void set_sub_viewport(const NodePath p_sub_viewport) { sub_viewport = p_sub_viewport; }
	NodePath get_sub_viewport() const { return sub_viewport; }

	void _ready() override;
	void _process(double delta) override;
	void _input(const Ref<InputEvent>& event) override;
	void _physics_process(double delta) override; 
};

#endif // PLAYER_H
