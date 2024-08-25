#ifndef DIALOGUE_H
#define DIALOGUE_H

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

// Inherits Timer to time the pause between each character
class Dialogue : public Label
{
GDCLASS(Dialogue, Label)

private:
	Timer* timer;
	float text_speed;
	unsigned char current_msg;
	bool waiting;

protected:
	static void _bind_methods();

	void set_text_speed(const float p_text_speed) { text_speed = p_text_speed; }
	float get_text_speed() { return text_speed; }

	void _on_timeout();

public:
	Dialogue();
	~Dialogue();

	void _ready() override;
	void _input(const Ref<InputEvent>& event) override;
};

#endif // !DIALOGUE_H
