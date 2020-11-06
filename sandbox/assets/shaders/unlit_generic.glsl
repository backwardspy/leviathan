//! #version 460 core
//! #define FRAGMENT_SHADER

struct v2f_t {
    vec4 color;
    vec2 uv;
};

uniform mat4 Model;
uniform mat4 ViewProjection;
uniform vec4 Tint;
uniform sampler2D MainTex;

#ifdef VERTEX_SHADER
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_uv;

out v2f_t v2f;

void main() {
    gl_Position = ViewProjection * Model * vec4(in_position, 1.0);
    v2f.color = in_color * Tint;
    v2f.uv = in_uv;
}
#endif

#ifdef FRAGMENT_SHADER
in v2f_t v2f;

out vec4 out_colour;

void main() {
    vec4 tex_col = texture(MainTex, v2f.uv);
    out_colour = tex_col * v2f.color;
}
#endif
