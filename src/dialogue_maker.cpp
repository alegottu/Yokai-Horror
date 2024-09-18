#include "dialogue_maker.h"
#include "dialogue_info.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void DialogueMaker::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("_on_enter_pressed"), &DialogueMaker::_on_enter_pressed);
}

void DialogueMaker::_ready()
{
	if (get_child_count() == 2)
	{
		text = (TextEdit*)get_child(0);
		enter = (Button*)get_child(1);
		enter->connect("pressed", Callable(this, "_on_enter_pressed"));
	}
}

void DialogueMaker::_on_enter_pressed()
{
	if (get_child_count() == 2 && text->get_line_count() > 0)
	{
		PackedStringArray dialogue;
		PackedByteArray characters; 
		PackedByteArray event_codes;
		PackedByteArray event_indices;

		for (unsigned char i = 0; i < text->get_line_count(); ++i)
		{
			PackedStringArray split_line = text->get_line(i).split(": ", false, 1);
			String character = split_line[0];

			// no better way around this, may just want to include numbers in text itself
			if (character == "Protagonist")
			{
				characters.push_back(0);
			}
			else if (character == "Grandpa")
			{
				characters.push_back(1);
			}
			else 
			{
				UtilityFunctions::printerr("Invalid character given");
			}

			if (split_line[1].contains(" ; "))
			{
				PackedStringArray split_for_codes = split_line[1].split(" ; ", false, 1);
				dialogue.push_back(split_for_codes[0]);
				PackedStringArray str_codes = split_for_codes[1].split(",");

				for (unsigned char j = 0; j < str_codes.size(); ++j)
				{
					event_codes.push_back(str_codes[j].to_int());
					event_indices.push_back(i);
				}
			}
			else
			{
	   			dialogue.push_back(split_line[0]);
	   		}
		}

		DialogueInfo dialogue_info;
		dialogue_info.set("text", dialogue);
		dialogue_info.set("characters", characters);
		dialogue_info.set("events", event_codes);
		dialogue_info.set("event_indices", event_indices);
		UtilityFunctions::print(dialogue_info.get("text"));
		UtilityFunctions::print(dialogue_info.get_property_list());
		Ref<DialogueInfo> dialogue_ref(&dialogue_info);
		// Can't get properties to be visible to ResourceSaver, pivot to GDScript
		// dialogue_info.write_to_disk(dialogue_ref, "res://dialogue/test.tres");
	}
}
