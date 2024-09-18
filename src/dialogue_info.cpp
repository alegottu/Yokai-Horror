#include "dialogue_info.h"
#include "helpers.h"

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_saver.hpp>

void DialogueInfo::_bind_methods()
{
	ADD_PRPRTY(&DialogueInfo::set_text, &DialogueInfo::get_text, PropertyInfo(Variant::PACKED_STRING_ARRAY, "text"));
	ADD_PRPRTY(&DialogueInfo::set_characters, &DialogueInfo::get_characters, PropertyInfo(Variant::PACKED_BYTE_ARRAY, "characters"));
	ADD_PRPRTY(&DialogueInfo::set_events, &DialogueInfo::get_events, PropertyInfo(Variant::PACKED_BYTE_ARRAY, "events"));
	ADD_PRPRTY(&DialogueInfo::set_event_indices, &DialogueInfo::get_event_indices, PropertyInfo(Variant::PACKED_BYTE_ARRAY, "event_indices"));
	ClassDB::bind_method(D_METHOD("write_to_disk"), &DialogueInfo::write_to_disk, PropertyInfo(Variant::STRING, "path"));
}

void DialogueInfo::write_to_disk(const String& path) const
{
	Ref<DialogueInfo> resource(this);
	ResourceSaver::get_singleton()->save(resource, path);
}
