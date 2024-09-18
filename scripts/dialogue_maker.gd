extends VSplitContainer

# translated from GDExtension C++ dialogue_maker.h and dialogue_maker.cpp

func _on_button_pressed():
	var text: TextEdit = get_child(0)
	
	if get_child_count() == 2 and text.get_line_count() > 0:
		var dialogue: PackedStringArray
		var characters: PackedByteArray
		var event_codes: PackedByteArray
		var event_indices: PackedByteArray

		for i in text.get_line_count():
			var split_line: PackedStringArray = text.get_line(i).split(": ", false, 1)
			var character: String = split_line[0]
			var spoken: String = split_line[1]

			# no better way around this, may just want to include numbers in text itself
			if character == "Protagonist":
				characters.push_back(0)
			elif character == "Grandpa":
				characters.push_back(1)
			else:
				printerr("Invalid character given")

			if spoken.contains(" ; "):
				var split_for_codes: PackedStringArray = spoken.split(" ; ", false, 1)
				dialogue.push_back(split_for_codes[0])
				var str_codes: PackedStringArray = split_for_codes[1].split(",")

				for j in str_codes.size():
					event_codes.push_back(str_codes[j].to_int());
					event_indices.push_back(i);
			else:
				dialogue.push_back(spoken)

		var dialogue_info: DialogueInfo = DialogueInfo.new()
		dialogue_info.text = dialogue
		dialogue_info.characters = characters
		dialogue_info.events = event_codes
		dialogue_info.event_indices = event_indices
		dialogue_info.write_to_disk("res://dialogue/test.tres");
