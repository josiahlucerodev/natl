VertexShader:

namespace trig {

};

struct ConstantData {
	vec3<f32> pos;
};

layout(binding = 0) readonly ConstantBuffer<ConstantData> constBuffer;
layout(binding = 1) readonly ConstantBuffer<ConstantData> constBuffer;

struct VertexOutput {
	builtIn(position) vec3<f32> position;
	layout(location = 0) vec2<f32> uv;
};

struct VertexInput {
	layout(location = 0) vec3<f32> position;
	layout(location = 1) vec2<f32> uv;
};

layout(binding = 3) ConstantBuffer<> constBuffer {
	vec3<f32> pos;
	vec3<f32> color;
};

layout(binding = 4) Sampler2D shadowTex;

fn main() {
}

fn main(VertexInput vertexInput) -> VertexOutput {
	for (int i = 0; i < 6; i) { 

	};
	switch(the) {
		case 8:

		break 9;
		case 10: 
		break;
		case 11:
	};

	ConstantData cd; 
	cd.pos;
	trig::sin(vec4<f32>(1.0f));

};

End;



FragmentShader:

struct VertexInput {
	layout(location = 0) vec2<f32> uv;
};

fn main(readonly VertexInput vertexInput) -> void {

};

End;