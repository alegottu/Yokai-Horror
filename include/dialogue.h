#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "dialogue_info.h"
#include "godot_cpp/variant/string_name.hpp"

#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace godot;

class Dialogue : public Label
{
GDCLASS(Dialogue, Label)

private:
	NodePath resource_preloader;
	StringName dialogue_resource_name;
	Ref<DialogueInfo> dialogue;
	Timer* timer; // uses Timer to time the pause between each character
	float text_speed;
	unsigned char current_msg;
	bool waiting;

protected:
	static void _bind_methods();

	void set_resource_preloader(const NodePath& p_resource_preloader) { resource_preloader = p_resource_preloader; }
	NodePath get_resource_preloader() { return resource_preloader; }
	void set_dialogue_resource_name(const StringName& p_dialogue_resource_name) { dialogue_resource_name = p_dialogue_resource_name; }
	StringName get_dialogue_resource_name() const { return dialogue_resource_name; }
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
