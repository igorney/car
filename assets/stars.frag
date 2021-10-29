#version 410

in vec4 fragColor;

out vec4 outColor;

void main() {
  float intensity = 1.0;
  outColor = fragColor * intensity;
}