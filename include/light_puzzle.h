#ifndef LIGHT_PUZZLE_H
#define LIGHT_PUZZLE_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class LightPuzzle : public Node3D
{
GDCLASS(LightPuzzle, Node3D)

private:
	// Inspector properties
	Array light_areas; // array of Light3D or NodePaths, can possibly be replace with PackedArray of strings
	
	Area3D** _light_areas;
	unsigned char num_lights;
	unsigned char active_light; // index to the light that the player is near
	unsigned char next_light; // index to the light that should be turned on next
	unsigned char lights_on;

protected:
	static void _bind_methods();

	// Node property functions
	void set_light_areas(const Array p_light_areas) { light_areas = p_light_areas; }
	Array get_light_areas() const { return light_areas; }

	void _body_entered(const Node3D* node);
	void _body_exited(const Node3D* _);

public:
	LightPuzzle();
	~LightPuzzle();

	void _ready() override;
	void _input(const Ref<InputEvent>& event) override;
};

#endif // !LIGHT_PUZZLE_H
