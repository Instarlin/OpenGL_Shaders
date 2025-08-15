#version 460
out vec4 fragColor;
uniform float time;

float sdBox(vec3 p, vec3 b) {
  vec3 q = abs(p) - b;
  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
};

mat2 rot(float r) {
  return mat2(cos(r), sin(r), -sin(r), cos(r));
};

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

float getNoiseContext(vec2 uv) {
	float f = 0.0;

	uv *= 10.0;
  mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
  f  = 0.5000*noise( uv ); uv = m*uv;
  f += 0.2500*noise( uv ); uv = m*uv;
  f += 0.1250*noise( uv ); uv = m*uv;
  f += 0.0625*noise( uv ); uv = m*uv;

	f = 0.5 + 0.5*f;

	return f;
};

float sdSphere(vec3 p, float r) {
  return length(p) - r;
};

float map(vec3 p, vec2 uv) {
  uv *= .4;
  float noise = getNoiseContext(uv + time/20);
  float ground1 = p.y + noise + .7;

  p.xz *= rot(time/12);

  vec3 boxPosition = vec3(0, .2, 0);
  float box = sdBox(p - boxPosition, vec3(2.));

  return max(ground1, box);
};

void main(void) {
  vec2 uv = vec2((gl_FragCoord.x*2.0-750)/550, (gl_FragCoord.y*2.0-550)/550);

  vec3 ro = vec3(0, 0, -4);
  vec3 rd = normalize(vec3(uv, 1));
  vec3 col = vec3(0);

  float t = 0.;

  for (int i = 0; i < 80; i++) {
    vec3 p = ro + rd * t;

    float d = map(p, uv);

    t += d;

    if (d < .0001 || t > 100.) break;
  };

  t = 0.1 * t;
  col += t;

  fragColor = vec4(col, 1);
};
