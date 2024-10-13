#ifndef DETECTION_SPHERE_H
#define DETECTION_SPHERE_H

#include <godot_cpp/classes/node3D.hpp>

#include "vision_cone.h"

using namespace godot;

class DetectionSphere : public Node3D
{
	GDCLASS(DetectionSphere, Node3D)

private:
	float radius;
	VisionCone* vision; // should be 1 and only child of this node
	Node3D* player;
	unsigned char sound_threshold; // if the sound meter goes above this value, the overflow goes towards detection
	unsigned char sound_meter; // goes up by a certain value for respective events where the player creates sound
	unsigned char sound_damp; // divide an addition to the sound meter by this amount if no line of sight

protected:
	static void _bind_methods();

	// Node property functions
	void set_radius(const float p_radius) { radius = p_radius; }
	float get_radius() const { return radius; }
	void set_player(Node3D* const p_player) { player = p_player; }
	Node3D* get_player() const { return player; }
	void set_sound_threshold(const unsigned char p_sound_threshold) { sound_threshold = p_sound_threshold; }
	unsigned char get_sound_threshold() const { return sound_threshold; }
	void set_sound_meter(const unsigned char p_sound_meter) { sound_meter = p_sound_meter; }
	unsigned char get_sound_meter() const { return sound_meter; }
	void set_sound_damp(const unsigned char p_sound_damp) { sound_damp = p_sound_damp; }
	unsigned char get_sound_damp() const { return sound_damp; }

	// Signal responses
	void _noise_made(unsigned char noise_level);

public:
	void _ready() override;
};

#endif // !DETECTION_SPHERE_H
