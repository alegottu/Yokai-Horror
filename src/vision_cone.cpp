#include "vision_cone.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/math.hpp>

// TODO: change the rest of the src files to use the gen_property.sh script code rather than helpers.h
void VisionCone::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("set_ray_distance", "p_ray_distance"), &VisionCone::set_ray_distance);
	ClassDB::bind_method(D_METHOD("get_ray_distance"), &VisionCone::get_ray_distance);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ray_distance"), "set_ray_distance", "get_ray_distance");

	ClassDB::bind_method(D_METHOD("set_ray_distance_falloff", "p_ray_distance_falloff"), &VisionCone::set_ray_distance_falloff);
	ClassDB::bind_method(D_METHOD("get_ray_distance_falloff"), &VisionCone::get_ray_distance_falloff);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ray_distance_falloff"), "set_ray_distance_falloff", "get_ray_distance_falloff");

	ClassDB::bind_method(D_METHOD("set_ray_group_distance_cut", "p_ray_group_distance_cut"), &VisionCone::set_ray_group_distance_cut);
	ClassDB::bind_method(D_METHOD("get_ray_group_distance_cut"), &VisionCone::get_ray_group_distance_cut);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "ray_group_distance_cut"), "set_ray_group_distance_cut", "get_ray_group_distance_cut");

	ClassDB::bind_method(D_METHOD("set_ray_groups", "p_ray_groups"), &VisionCone::set_ray_groups);
	ClassDB::bind_method(D_METHOD("get_ray_groups"), &VisionCone::get_ray_groups);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "ray_groups"), "set_ray_groups", "get_ray_groups");

	ClassDB::bind_method(D_METHOD("set_num_rays", "p_num_rays"), &VisionCone::set_num_rays);
	ClassDB::bind_method(D_METHOD("get_num_rays"), &VisionCone::get_num_rays);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "num_rays"), "set_num_rays", "get_num_rays");

	ClassDB::bind_method(D_METHOD("set_span", "p_span"), &VisionCone::set_span);
	ClassDB::bind_method(D_METHOD("get_span"), &VisionCone::get_span);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "span"), "set_span", "get_span");
}

void VisionCone::create_ray(const Vector3& target_pos)
{
	RayCast3D* ray = new RayCast3D;
	ray->set_collision_mask(0); // ignore everything first
	ray->set_collision_mask_value(2, true); // allow the layer 2 (Player layer)
	ray->set_target_position(target_pos);
	add_child(ray);
}

void VisionCone::_ready()
{
	ray_groups = (ray_groups == 0) ? 1 : ray_groups; // avoid division by 0
	rays = new RayCast3D*[num_rays];
	unsigned char rays_adjusted = num_rays - 1; // - 1 for center ray
	unsigned char left_rays, right_rays;
	left_rays = right_rays = rays_adjusted / 2; 
	// add 1 to left_rays if num_rays is uneven, makes up for the difference lost after the division by 2
	left_rays += 1 * (rays_adjusted % 2);
	create_ray(get_position() + get_basis().xform(Vector3(0.0, 0.0, ray_distance))); // create center ray
	float half_span = span / ((float)rays_adjusted / left_rays);
	// span_split can be pow(half_span, 1.0 / left_rays) for current_angle to grow exponentially
	float span_split = half_span / left_rays;
	float current_angle = span_split;
	float current_distance = ray_distance;

	// create raycast nodes attached to this node in a cone pattern;
	// start with sweeping to the left after the center ray
	for (unsigned char i = 0; i < left_rays; ++i)
	{
		float current_rads = Math::deg_to_rad(current_angle);
		float distance_mod = current_distance - ray_group_distance_cut * (i % ray_groups);
		float delta_x = -sin(current_rads) * distance_mod;
		float delta_z = cos(current_rads) * distance_mod;
		Vector3 delta = get_basis().xform(Vector3(delta_x, 0.0, delta_z));
		Vector3 target_pos = get_position() + delta;
		create_ray(target_pos);
		current_angle += span_split; 
		current_distance -= ray_distance_falloff;
	}

	// now sweep to the right
	half_span = span / ((float)rays_adjusted / right_rays);
	span_split = half_span / right_rays;
	current_angle = span_split;
	current_distance = ray_distance;

	for (unsigned char i = 0; i < right_rays; ++i)
	{
		float current_rads = Math::deg_to_rad(current_angle);
		float distance_mod = current_distance - ray_group_distance_cut * (i % ray_groups);
		float delta_x = sin(current_rads) * distance_mod;
		float delta_z = cos(current_rads) * distance_mod;
		Vector3 delta = get_basis().xform(Vector3(delta_x, 0.0, delta_z));
		Vector3 target_pos = get_position() + delta;
		create_ray(target_pos);
		current_angle += span_split;
		current_distance -= ray_distance_falloff;
	}
}

VisionCone::~VisionCone()
{
	for (unsigned char i = 0; i < num_rays; ++i)
	{
		delete rays[i];
	}

	delete[] rays;
}

void VisionCone::_physics_process(const double delta)
{
	// for now rely on enabling Visible Collision Shapes in Debug menu
}
