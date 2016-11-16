#version 410 core

layout (location = 0) in vec3 position;
/*layout (location = 1) in vec3 color;*/
layout (location = 1) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(position, 1.0f);
    ourColor = vec3(1.0f, 1.0f, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}
