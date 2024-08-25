#include "dialogue.h"
#include "helpers.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/engine.hpp>

constexpr char text[3][29] = { "Press E to transfer control.", "Press W to move forward.", "Nothing further." };

void Dialogue::_bind_methods()
{
	ADD_PRPRTY(&Dialogue::set_text_speed, &Dialogue::get_text_speed, PropertyInfo(Variant::FLOAT, "text_speed"));

	ClassDB::bind_method(D_METHOD("_on_timeout"), &Dialogue::_on_timeout);
}

void Dialogue::_on_timeout()
{
	unsigned int visible_chars = get_visible_characters();

	if (visible_chars == get_text().length())
	{
		waiting = true;
	}
	else 
	{
		set_visible_characters(visible_chars + 1);
		timer->start(text_speed);	
		waiting = false;
	}
}

Dialogue::Dialogue()
	: text_speed(0.1f), current_msg(0), waiting(false)
{
}

Dialogue::~Dialogue()
{
}

void Dialogue::_ready()
{
	if (Engine::get_singleton()->is_editor_hint())
	{
		set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
		return;
	}
	else
	 	set_process_mode(Node::ProcessMode::PROCESS_MODE_INHERIT);

	if (get_child_count() > 0)
	{
		// ensure correct parent-child hierarchy
		timer = (Timer*) get_child(0);
		timer->connect("timeout", Callable(this, "_on_timeout"));
		timer->set_one_shot(true);
		set_text(text[current_msg]);
		set_visible_characters(0);
		timer->start();
	}
	else
		timer = nullptr;
}

void Dialogue::_input(const Ref<InputEvent>& event)
{
	if (event->is_action_pressed("esc"))
	{
		set_visible(false);
		waiting = false;
		// same as below
	}

	if (waiting && event->is_action_pressed("click"))
	{
		if (++current_msg >= 3)
		{
			set_visible(false);
			waiting = false;
			// destroy self?
		}
		else 
		{
			set_text(text[current_msg]);
			set_visible_characters(0);
			timer->start();
		}
	}
}
