// unused for now in favor of built-in billboard feature but can profile adding spritesheet calculations (for different angles to player) here
shader_type spatial;
render_mode unshaded, cull_disabled, depth_draw_opaque, depth_prepass_alpha;

uniform sampler2D tex: source_color;

void vertex() 
{
	MODELVIEW_MATRIX = VIEW_MATRIX * mat4(vec4(normalize(cross(vec3(0.0, 1.0, 0.0), INV_VIEW_MATRIX[2].xyz)), 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(normalize(cross(INV_VIEW_MATRIX[0].xyz, vec3(0.0, 1.0, 0.0))), 0.0), MODEL_MATRIX[3]);
	MODELVIEW_NORMAL_MATRIX = mat3(MODELVIEW_MATRIX);
}

void fragment()
{
	vec4 col = texture(tex, UV);
	ALBEDO = vec3(col.r, col.g, col.b);
	ALPHA = col.a;
}
