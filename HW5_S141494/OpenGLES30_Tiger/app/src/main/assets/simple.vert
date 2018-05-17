#version 300 es

uniform mat4 ModelViewProjectionMatrix;
uniform vec3 primitive_color;

layout (location = 0) in vec4 v_position;

out vec4 color;

void main(void) {
 	color = vec4(primitive_color, 1.0f);
    gl_Position =  ModelViewProjectionMatrix * v_position;
}
