#ifndef LIGHT_PUZZLE_H
#define LIGHT_PUZZLE_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/classes/light3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class LightPuzzle : public Node3D
{
GDCLASS(LightPuzzle, Node3D)

private:
	// Inspector properties
	Array lights; // array of Light3D or NodePaths, can possibly be replace with PackedArray of strings
	
	Light3D** _lights;
	// unsigned char active_light; // index to the light that the player is near
	// unsigned char next_light; // index to the light that should be turned on next

protected:
	static void _bind_methods();

public:
	LightPuzzle();
	~LightPuzzle();

	// Node property functions
	void set_lights(const Array p_lights) { lights = p_lights; }
	Array get_lights() const { return lights; }

	void _ready() override;
};

#endif // !LIGHT_PUZZLE_H
