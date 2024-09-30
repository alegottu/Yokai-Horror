#ifndef HEADER_H
#define HEADER_H

#include <godot_cpp/classes/node.hpp>

using namespace godot;

class Class : public Node
{
	GDCLASS(Class, Node)

private:

protected:
	static void _bind_methods();

public:
	Class();
	Class~();

	void _ready() override;
	void _process(const double delta) override;
}

#endif // !HEADER_H
