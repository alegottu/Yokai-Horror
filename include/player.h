#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

namespace godot 
{

class Player : public Node3D
{
	GDCLASS(Player, Node3D)

private:
	// Inspector properties
	float speed;
	float sensitivity;
	float return_rate;
	NodePath cam;
	NodePath cam_pivot;

	Node3D* _cam;
	Node3D* _cam_pivot;
	Node3D* spirit;
	Vector3 spirit_start;
	Vector3 spirit_rotation;
	bool spirit_active;
	bool spirit_return;

protected:
	static void _bind_methods();

public:
	Player();
	~Player();

	// Node property functions
	void set_speed(const float p_speed);
	float get_speed() const;
	void set_sensitivity(const float p_sensitivity);
	float get_sensitivity() const;
	void set_return_rate(const float p_return_rate);
	float get_return_rate() const;
	void set_cam(const NodePath p_cam);
	NodePath get_cam() const;
	void set_cam_pivot(const NodePath p_cam_pivot);
	NodePath get_cam_pivot() const;

	void _ready() override;
	void _process(double delta) override;
	void _input(const Ref<InputEvent>& event) override;
};

}

#endif // !PLAYER_H
