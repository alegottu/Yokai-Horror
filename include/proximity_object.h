#ifndef PROXIMITY_OBJECT_H
#define PROXIMITY_OBJECT_H

#include <godot_cpp/classes/area3d.hpp>

using namespace godot;

class ProximityObject : public Area3D
{
private:
	// Whether the player is near the object;
	// ensure collision mask does not allow collisions with any entities but the player
	bool active;

protected:
	static void _bind_methods();

public:
	ProximityObject();
	~ProximityObject();

	// void _input(const Ref<InputEvent>& event) override;
	void body_entered();
	void body_exited();
};

#endif // !PROXIMITY_OBJECT_H
