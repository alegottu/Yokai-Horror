#ifndef VISION_CONE_H
#define VISION_CONE_H

#include <godot_cpp/classes/node3D.hpp>
#include <godot_cpp/classes/ray_cast3d.hpp>

using namespace godot;

class VisionCone : public Node3D
{
	GDCLASS(VisionCone, Node3D)

private:
	// TODO: create detection rate / threshold properties, once threshold is met fire a signal
	float ray_distance;
	float ray_distance_falloff; // as the rays go farther from the center, decrease their distance by this much
	float ray_group_distance_cut; // for each group of rays, cut off this distance, multiplied by group number
	RayCast3D** rays;
	unsigned char ray_groups;
	unsigned char num_rays;
	unsigned char span; // the angle that represents how wide the cone spans
	
	void create_ray(const Vector3& target_pos);

protected:
	static void _bind_methods();
	void set_ray_distance(const float p_ray_distance) { ray_distance = p_ray_distance; }
	float get_ray_distance() const { return ray_distance; }
	void set_ray_distance_falloff(const float p_ray_distance_falloff) { ray_distance_falloff = p_ray_distance_falloff; }
	float get_ray_distance_falloff() const { return ray_distance_falloff; }
	void set_ray_group_distance_cut(const float p_ray_group_distance_cut) { ray_group_distance_cut = p_ray_group_distance_cut; }
	float get_ray_group_distance_cut() const { return ray_group_distance_cut; }
	void set_ray_groups(const unsigned char p_ray_groups) { ray_groups = p_ray_groups; }
	unsigned char get_ray_groups() const { return ray_groups; }
	void set_num_rays(const unsigned char p_num_rays) { num_rays = p_num_rays; }
	unsigned char get_num_rays() const { return num_rays; }
	void set_span(const unsigned char p_span) { span = p_span; }
	unsigned char get_span() const { return span; }

public:
	void _ready() override;
	~VisionCone();
};

#endif // !VISION_CONE_H
