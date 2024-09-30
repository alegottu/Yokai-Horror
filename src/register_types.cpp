#include "register_types.h"
#include "godot_cpp/core/class_db.hpp"
#include "player.h"
//#include "proximity_object.h"
#include "light_puzzle.h"
#include "dialogue.h"
#include "dialogue_info.h"
//#include "dialogue_maker.h"
#include "vision_cone.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

void initialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(Player);
	//GDREGISTER_CLASS(ProximityObject);
	GDREGISTER_CLASS(LightPuzzle);
	GDREGISTER_CLASS(Dialogue);
	GDREGISTER_CLASS(DialogueInfo);
	//GDREGISTER_CLASS(DialogueMaker);
	GDREGISTER_CLASS(VisionCone);
}

void uninitialize_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT yokai_horror_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_module);
	init_obj.register_terminator(uninitialize_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
