#version 430
layout(location = 0) in vec3 position;
// uniform mat4 mv_matrix; 
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;

out vec4 varyingColor;
// out vec3 outPosition;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void) {
    float i = gl_InstanceID + tf;
    float a = sin(.120 * i) * 103.0;
    float b = sin(.140 * i) * 201.0;
    float c = sin(.180 * i) * 305.0;

    mat4 localRotX = buildRotateX(4 * i);
    mat4 localRotY = buildRotateY(4 * i);
    mat4 localRotZ = buildRotateZ(4 * i);
    mat4 localTrans = buildTranslate(a, b, c);

    mat4 newM_matrix = localTrans * localRotX * localRotY * localRotZ;
    mat4 mv_matrix = v_matrix * newM_matrix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    // outPosition = vec3(gl_Position);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5);
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, x, y, z, 1.0);
    return trans;
}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0, 0.0, 0.0, 0.0, cos(rad), -sin(rad), 0.0, 0.0, sin(rad), cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0);
    return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(cos(rad), 0.0, sin(rad), 0.0, 0.0, 1.0, 0.0, 0.0, -sin(rad), 0.0, cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0);
    return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0, sin(rad), cos(rad), 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
    return zrot;
}

// builds and returns a scale matrix
mat4 buildScale(float x, float y, float z) {
    mat4 scale = mat4(x, 0.0, 0.0, 0.0, 0.0, y, 0.0, 0.0, 0.0, 0.0, z, 0.0, 0.0, 0.0, 0.0, 1.0);
    return scale;
}
