#version 330 core

in vec3 colores;
out vec4 FragColor;

void main() {
    FragColor = vec4(colores, 1);
}
