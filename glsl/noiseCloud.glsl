#version 460
out vec4 fragColor;
uniform float time;
uniform int width;
uniform int height;
uniform int hash;

float sdBox(vec3 p, vec3 b) {
  vec3 q = abs(p) - b;
  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
};

mat2 rot(float r) {
  return mat2(cos(r), sin(r), -sin(r), cos(r));
};

vec2 genHash(vec2 p) {
	p = vec2( dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)) );
	return -1.0 + 2.0*fract(sin(p) * 43758.5453123);
};

float noise(vec2 p) {
  const float K1 = 0.366025404; // (sqrt(3)-1)/2;
  const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2  i = floor(p + (p.x + p.y) * K1);
  vec2  a = p - i + (i.x + i.y) * K2;
  float m = step(a.y, a.x);
  vec2  o = vec2(m, 1.0 - m);
  vec2  b = a - o + K2;
	vec2  c = a - 1.0 + hash/98754325*K2;
  vec3  h = max(0.5 - vec3(dot(a, a), dot(b, b), dot(c, c)), 0.0);
	vec3  n = h*h*h*h * vec3(dot(a, genHash(i)), dot(b, genHash(i + o)), dot(c, genHash(i + 1.0)));
  return dot(n, vec3(70.0));
};

float getNoiseContext(vec2 uv) {
	float f = 0.0;

	uv *= 10.0;
  mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
  f  = 0.5000*noise( uv ); uv = m*uv;
  f += 0.2500*noise( uv ); uv = m*uv;
  f += 0.1250*noise( uv ); uv = m*uv;
  f += 0.0625*noise( uv ); uv = m*uv;

	f = .5 + .5*f;
  
	return f;
};

float sdSphere(vec3 p, float r) {
  return length(p) - r;
};

float map(vec3 p) {
  vec2 uv = vec2((gl_FragCoord.x*2.0-width)/height, (gl_FragCoord.y*2.0-height)/height)*.4;
  float noise = clamp(getNoiseContext(uv + time/15), .1, 1.);
  float ground = p.y - .6*noise + 1.;

  p.xz *= rot(time/12);

  vec3 boxPosition = vec3(0., .4, 0.);
  float box = sdBox(p - boxPosition, vec3(1.75));

  return max(ground, box);
  //* Sphere like cloud
  // vec3 spherePosition = vec3(0, 1*sin(time), 0);

  // float sphere = sdSphere(p, 1.) + clamp(noise, .1, .5);

  // return sphere;
};

void main(void) {
  vec2 uv = vec2((gl_FragCoord.x*2.0-width)/height, (gl_FragCoord.y*2.0-height)/height);
  const vec3 sundir = normalize(vec3(2., 2., -2.));

  vec3 ro = vec3(0., 0., -3.6);
  vec3 rd = normalize(vec3(uv, 1));
  vec3 col = vec3(0.);

  float t = 0.;

  for (int i = 0; i < 80; i++) {
    vec3 p = ro + rd * t;

    float d = map(p);

    if (d < .001) {
      float dif = clamp((d - map(p + .12*sundir))/.25, .0, 1. );
      vec3 lin = vec3(.65, .65, .75)*1.1 + .8*vec3(.9, .6, .3)*dif;
      vec3 op = p;
      int counter = 0;
      for (int j = 1; j < 50; j++) {
        t += 0.01;
        counter += 1;
        float s = map(p);
        if (s > 0) break;
        p = ro + rd * t;
      };
      col += 0.08 + 0.0005*counter;
      col *= lin;
    };

    t += d;

    if (t > 100.) break;
  };

  fragColor = vec4(col, 1);
};

// vec4 mainLoop(vec3 col, vec3 ro, vec3 rd, vec2 uv) {
//   vec4 sum = vec4(0.0);

//   float t = 0.;

//   for (int i = 0; i < 80; i++) {
//     float dt = max(0.05, 0.02*t);

//     vec3 p = ro + rd * t;

//     float d = map(p, uv);

//     if (d > 0.01) {
//       float dif = clamp((d - map(p + .3*sundir, uv))/0.25, 0., 1.);
//       vec3 lin = vec3(0.65,0.65,0.75)*1.1 + 0.8*vec3(1.0,0.6,0.3)*dif;
//       vec4 res = vec4( mix( vec3(1.0, 0.93, 0.84), vec3(0.25, 0.3 ,0.4), d ), d );
//       res.xyz *= lin;
//       res.xyz = mix(res.xyz, col, 1.0-exp2(-0.1*t));
//       res.w = min(res.w*8.0*d,1.0);
//       res.rgb *= res.a;
//       sum += res*(1.0-sum.a);
//     };

//     t += d;

//     if (t > 100.) break;
//   };

//   return clamp( sum, 0.0, 1.0 );
// }

// void main(void) {
//   vec2 uv = vec2((gl_FragCoord.x*2.0-750)/550, (gl_FragCoord.y*2.0-550)/550);

//   vec3 ro = vec3(0, 0, -4);
//   vec3 rd = normalize(vec3(uv, 1));
//   float sun = clamp( dot(sundir,rd), 0.0, 1.0 );
//   vec3 col = vec3(0.76,0.75,0.95);
//   // col -= 0.6*vec3(0.90,0.75,0.95)*rd.y;
// 	// col += 0.2*vec3(1.00,0.60,0.10)*pow( sun, 8.0 );

//   vec4 res = mainLoop(col, ro, rd, uv);

//   col = col*(1.0-res.w) + res.xyz;

//   col = smoothstep(0.15,1.1,col);

//   fragColor = vec4(col, 1.0);
// };