shader_type canvas_item;

render_mode unshaded;

#define MAXCOLORS 16


uniform float max_player_distance = 1.0;
uniform float static_speed : hint_range(0.0, 1.0) = 0.001;
uniform float negative_speed = 1.0;
uniform float negative_threshold : hint_range(0.0, 1.0) = 0.75;
uniform float noise_damp : hint_range(0.0, 1.0) = 1.0;
uniform float distance_margin : hint_range(0.0, 1.0) = 0.0;
uniform bool dithering = true;
uniform int colors : hint_range(1, MAXCOLORS) = 12;
uniform int dither_size : hint_range(1, 8) = 1;

uniform float player_distance = 0;

float dithering_pattern(ivec2 fragcoord) {
	const float pattern[] = {
		0.00, 0.50, 0.10, 0.65, 
		0.75, 0.25, 0.90, 0.35, 
		0.20, 0.70, 0.05, 0.50, 
		0.95, 0.40, 0.80, 0.30
	};
	
	int x = fragcoord.x % 4;
	int y = fragcoord.y % 4;
	
	return pattern[y * 4 + x];
}

float reduce_color(float raw, float dither, int depth) {
	float div = 1.0 / float(depth);
	float val = 0.0;
	int i = 0;

	while (i <= MAXCOLORS)
	{
		if (raw > div * (float(i + 1))) {
			i = i + 1;
			continue;
		}

		if (raw * float(depth) - float(i) <= dither * 0.999)
		{
			val = div * float(i);
		} 
		else
		{
			val = div * float(i + 1);
		}
		return val;

		i = i+1;
	}

	return val;
}

float Random(in vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

vec4 tether_effect(vec4 raw, vec2 uv) {
	float new_dist = max(0.0, floor(player_distance - distance_margin));
	float prog = new_dist / max_player_distance; // assumes that player can actually not go past max distance
    vec4 neg = vec4(1.0, 1.0, 1.0, 0.0) - raw;
	float prog_thresholded = prog * negative_speed * ceil(max(0.0, prog - negative_threshold));
	raw = mix(raw, neg, prog_thresholded);

	uv *= TIME * static_speed;
    vec4 noise = vec4(Random(uv.yy), Random(uv.xy), Random(uv.xx), 1.0);
	return mix(raw, noise, prog * noise_damp);
}

void fragment() {
	vec4 raw = texture(TEXTURE, SCREEN_UV);
	ivec2 uv = ivec2(FRAGCOORD.xy / float(dither_size));
	float dithering_value = 1.0;

	if (dithering)
	{
		dithering_value = dithering_pattern(uv);
	}

	raw = tether_effect(raw, FRAGCOORD.xy);
	COLOR.r = reduce_color(raw.r, (dithering_value - 0.5) * dithering_value + 0.5, colors - 1);
	COLOR.g = reduce_color(raw.g, (dithering_value - 0.5) * dithering_value + 0.5, colors - 1);
	COLOR.b = reduce_color(raw.b, (dithering_value - 0.5) * dithering_value + 0.5, colors - 1);
}
