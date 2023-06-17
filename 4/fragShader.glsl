#version 430

// in vec3 outPosition;
in vec4 varyingColor;

out vec4 color;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

// uniform int windowWidth;
// uniform int windowHeight;

void main(void) {
    // color = vec4(gl_FragCoord.xy / vec2(windowWidth, windowHeight), 0.0, 1) * 0.5 + vec4(0.5);
    // color = vec4(outPosition, 1.0) * 0.5 + vec4(0.5);
    color = varyingColor;
}