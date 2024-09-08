#include "dialogue_info.h"

#include <godot_cpp/classes/resource_saver.hpp>

void DialogueInfo::write_to_disk(Ref<DialogueInfo> dialogue, const char path[]) const
{
	ResourceSaver::get_singleton()->save(dialogue, path);
}
