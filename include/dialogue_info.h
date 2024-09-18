#ifndef DIALOGUE_INFO_H
#define DIALOGUE_INFO_H

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/variant/packed_string_array.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

// pipeline: DiaMaker Res Node (enter text) -> create DiaInfo -> write DiaInfo Res to disk
class DialogueInfo : public Resource
{
GDCLASS(DialogueInfo, Resource)

private:
	PackedStringArray text;
	PackedByteArray characters;
	PackedByteArray events; // unsigned char representation
	PackedByteArray event_indices; // shows for what messages events apply to

protected:
	static void _bind_methods();

	// Resource property functions
	void set_text(const PackedStringArray p_text) { text = p_text; }
	PackedStringArray get_text() { return text; }
	void set_characters(const PackedByteArray p_characters) { characters = p_characters; }
	PackedByteArray get_characters() { return characters; }
	void set_events(const PackedByteArray p_events) { events = p_events; }
	PackedByteArray get_events() { return events; }
	void set_event_indices(const PackedByteArray p_event_indices) { event_indices = p_event_indices; }
	PackedByteArray get_event_indices() { return event_indices; }

public:
	DialogueInfo() {}
	DialogueInfo(const PackedStringArray& p_text, const PackedByteArray& p_characters,
			  const PackedByteArray& p_events, const PackedByteArray& p_event_indices)
		: text(p_text), characters(p_characters), events(p_events), event_indices(p_event_indices) {}
	~DialogueInfo() {}

	const String& get_line(const unsigned char line) const { return text[line]; }
	const unsigned char get_line_count() const { return text.size(); }
	const unsigned char get_character(const unsigned char idx) const { return characters[idx]; }
	const unsigned char get_event(const unsigned char idx) const { return events[idx]; }
	const unsigned char get_event_index(const unsigned char idx) const { return event_indices[idx]; }

	void write_to_disk(const String& path) const;
};

#endif // !DIALOGUE_INFO_H
