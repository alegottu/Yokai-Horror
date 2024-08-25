#ifndef ENEMY_H
#define ENEMY_H

#include <godot_cpp/classes/node3d.hpp>

using namespace godot;

class Enemy : public Node3D
{
GDCLASS(Enemy, Node3D)

protected:
	static void _bind_methods();

public:
	// void _process(const double delta) override;
};

#endif // !ENEMY_H
