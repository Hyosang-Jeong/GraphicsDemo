#version 450 core

layout(vertices = 3)out;

in vec3 vpos[];

out vec3 tcPosition[];

uniform float depth;
float map(float input_, float input_start, float input_end, float output_start, float output_end)
{
	float t = (input_ - input_start) / (input_end - input_start);
	float result = mix(output_start, output_end, t);
	return result;
}
void main(void)
{

	if(gl_InvocationID == 0)
	{
		const float min_tess_level = 1;
		const float max_tess_level = 64;
		const float min_depth = -30;
		const float max_depth= -3;
		float tess_level = map(depth,min_depth,max_depth,min_tess_level,max_tess_level);
		gl_TessLevelInner[0] = tess_level;

		gl_TessLevelOuter[0] = tess_level;
		gl_TessLevelOuter[1] = tess_level;
		gl_TessLevelOuter[2] = tess_level;
	}

	tcPosition[gl_InvocationID] = vpos[gl_InvocationID];
}