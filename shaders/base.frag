#version 330 core

out vec4 FragColor;
uniform float time;
float rand(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
void main() {
    FragColor = vec4(abs(sin(time/3)), abs(cos(time/3)), abs(sin(time/3)), 1.0) * rand(gl_FragCoord.xy);
}
