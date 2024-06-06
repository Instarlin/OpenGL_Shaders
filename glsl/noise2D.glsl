#version 460
out vec4 fragColor;
uniform float time;

vec2 hash( vec2 p ) {
	p = vec2( dot(p, vec2(127.1, 311.7)), dot(p,vec2(269.5,183.3)) );
	return -1.0 + 2.0*fract(sin(p) * 43758.5453123);
};

float noise( in vec2 p ) {
  const float K1 = 0.366025404; // (sqrt(3)-1)/2;
  const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor( p + (p.x+p.y)*K1 );
  vec2  a = p - i + (i.x+i.y)*K2;
  float m = step(a.y,a.x); 
  vec2  o = vec2(m,1.0-m);
  vec2  b = a - o + K2;
	vec2  c = a - 1.0 + 2.0*K2;
  vec3  h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
	vec3  n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
  return dot( n, vec3(70.0) );
};

void main(void) {
	vec2 uv = vec2((gl_FragCoord.x*2.0-750)/550, (gl_FragCoord.y*2.0-550)/550);

	float f = 0.0;

	uv *= 10.0;
  mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
  f  = 0.5000*noise( uv + time + 200000 ); uv = m*uv;
  f += 0.2500*noise( uv + time + 200000 ); uv = m*uv;
  f += 0.1250*noise( uv + time + 200000 ); uv = m*uv;
  f += 0.0625*noise( uv + time + 200000 ); uv = m*uv;

	f = 0.5 + 0.5*f;

	fragColor = vec4( f, f, f, 1.0 );
};