#ifndef PROXIMITY_OBJECT_H
#define PROXIMITY_OBJECT_H

#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class ProximityObject : public Area3D
{
GDCLASS(ProximityObject, Area3D)

protected:
	// Whether the player is near the object;
	// ensure collision mask does not allow collisions with any entities but the player
	bool active;

	static void _bind_methods();

	void _body_entered();
	void _body_exited();

public:
	ProximityObject();
	~ProximityObject();

	void _input(const Ref<InputEvent>& event) override;

	bool get_active() { return active; }
};

#endif // !PROXIMITY_OBJECT_H
