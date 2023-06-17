#version 430
layout(location = 0) in vec3 position;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

out vec4 varyingColor;
// out vec3 outPosition;

void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    // outPosition = vec3(gl_Position);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5);
}