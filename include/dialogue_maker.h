#ifndef DIALOGUE_MAKER_H
#define DIALOGUE_MAKER_H

#include <godot_cpp/classes/split_container.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/button.hpp>

using namespace godot;

class DialogueMaker : public SplitContainer
{
GDCLASS(DialogueMaker, SplitContainer)

private:
	TextEdit* text;
	Button* enter;

protected:
	static void _bind_methods();

	void _on_enter_pressed();

public:
	void _ready() override;
};

#endif // !DIALOGUE_MAKER_H
