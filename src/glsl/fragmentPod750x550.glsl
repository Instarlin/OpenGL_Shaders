#version 460
out vec4 fragColor;
uniform float time;
void main(void) {
  vec2 uv = vec2((gl_FragCoord.x*2.0-750)/550, (gl_FragCoord.y*2.0-550)/550);

  float d = length(uv);
  d = sin(d*8.0 - time)/8.0;
  d = abs(d);

  vec3 col = vec3(
    1/(length(uv)+0.3)*0.5, 
    0.1*sin(time)+0.2*length(uv),
    0.55+0.1*sin(time)+0.5*length(uv)
  );

  d = 0.03 / d;

  col *= d;

  fragColor = vec4(col, 1.0);
};

  // dor = vec4(
  //   gl_FragCoord.x*0.001+0.1, 
  //   0.1, 
  //   gl_FragCoord.y*0.001+0.1, 1.0
  // );