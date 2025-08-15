#version 460
out vec4 fragColor;
uniform float time;

vec3 pallete(float t) {
  vec3 a = vec3(0.5, 0.5, 0.5); 
  vec3 b = vec3(0.5, 0.5, 0.5);
  vec3 c = vec3(1.0, 1.0, 1.0);
  vec3 d = vec3(0.263, 0.416, 0.557);

  return a + b*cos(6.28318*(c*t+d));
};

void main(void) {
  vec2 uv = vec2((gl_FragCoord.x * 2.0 - 600) / 600, (gl_FragCoord.y * 2.0 - 600) / 600);
  vec2 uv0 = uv;
  vec3 finalColor = vec3(0.0);

  for (float val = 0.0; val < 4.0; val++) {
    uv = fract(uv * 1.5) - 0.5;

    float d = length(uv) * exp(-length(uv0));
    d = sin(d*8.0 + val * time * 0.3)/8.0;
    d = abs(d);

    vec3 col = pallete(d + time/3 + length(uv0) + val*.4);

    d = 0.01 / d;

    col *= d;

    finalColor += col * d;
  };

  fragColor = vec4(finalColor, 1.0);
};