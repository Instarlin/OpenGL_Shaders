#version 460
out vec4 fragColor;

uniform float time;
uniform int width;
uniform int height;
uniform int menuCase;
uniform float posOffset;
uniform int renderSteps;
uniform float mouseX;
uniform float mouseY;

float mandelbulb(vec3 p) {
    vec3 z = p;
    float dr = 1.0;
    float r = 0.0;

    const int ITER = 8; // iteration depth
    const float Power = time; // bulb power

    for (int i = 0; i < ITER; i++) {
        r = length(z);
        if (r > 2.0) break;

        // Spherical coordinates
        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        dr = pow(r, Power - 1.0) * Power * dr + 1.0;

        // Scale and rotate the point
        float zr = pow(r, Power);
        theta *= Power;
        phi *= Power;

        // Convert back to cartesian
        z = zr * vec3(sin(theta) * cos(phi),
                      sin(theta) * sin(phi),
                      cos(theta));
        z += p;
    }
    return 0.5 * log(r) * r / dr;
}


float smoothUnion(float d1, float d2, float k) {
  float h = clamp(0.5 + 0.5*(d2-d1)/k, 0.0, 1.0);
  return mix(d2, d1, h) - k*h*(1.0-h);
};

float smoothSubstraction(float d1, float d2, float k) {
  float h = clamp(0.5 - 0.5*(d2+d1)/k, 0.0, 1.0);
  return mix(d2, -d1, h) + k*h*(1.0-h);
};

float smoothIntersection(float d1, float d2, float k) {
  float h = clamp(0.5 - 0.5*(d2-d1)/k, 0.0, 1.0);
  return mix(d2, d1, h) + k*h*(1.0-h);
};

float sdSphere(vec3 p, float r) {
  return length(p) - r;
};

float sdBox(vec3 p, vec3 b) {
  vec3 q = abs(p) - b;
  return length(max(q, 0.0)) + min(max(q.x, max(q.y, q.z)), 0.0);
};

float sdCrosscube(vec3 p, vec3 s) {
  vec3 v = vec3(1./3., 1./3., 4./3.);

  float d1 = sdBox(p, s*v.xyz);
  float d2 = sdBox(p, s*v.yzx);
  float d3 = sdBox(p, s*v.zxy);
  
  return min(min(d1, d2), d3);
};

float rand(vec2 p) {
  return fract(sin(dot(p.xy,vec2(12.9898,78.233))) * 43758.5453);
};

mat2 rot(float r) {
  return mat2(cos(r), sin(r), -sin(r), cos(r));
};

float rotationScale = 0.01;
float angleY = -mouseX * rotationScale;
float angleX = -mouseY * rotationScale;

mat3 rotX = mat3(
  1.0,       0.0,        0.0,
  0.0, cos(angleX), -sin(angleX),
  0.0, sin(angleX),  cos(angleX)
);

mat3 rotY = mat3(
  cos(angleY),  0.0, sin(angleY),
  0.0,    1.0,      0.0,
  -sin(angleY), 0.0, cos(angleY)
);

mat3 rotationMatrix = rotY * rotX;

float distCase(vec3 p) {
  float sphere, sphere1, box, ground; vec3 q, size, spherePosition, boxPosition;

  switch (menuCase) {
    case 0:

      // p.xy = (mat3(
      //           1.0, 0.0, 0.0,
      //           0.0, 1.0, 0.0,
      //           -mouseX / 50.0, mouseY / 50.0, 1.0
      //         ) * vec3(p.xy, 1.0)).xy;

      p = rotationMatrix * p;
      
      p.xz *= rot(time);
      p.yz *= rot(time);
      // p.xy = p.xy - posOffset;

      size = vec3(1.);
      float d = sdBox(p, size);
      float cd = sdCrosscube(p, size);
      d = max(d, -cd);

      for (int i = 0; i < 6; i++) {
        size *= 1./3.;
        float m = size.x*2.;
        p = mod(p-0.5*m, m)-0.5*m;
        cd = sdCrosscube(p, size);
        d = max(d, -cd);
      };

      return d;
    case 1: 
      q = p;

      // 0.02 * 120 
      if (sin(time * .5) > 0) {
        q.z += mod(time, 10.0) * .1;
      } else {
        q.x += mod(time, 6.0) * .1;
      }
      
      q = fract(q) - .5;

      size = vec3(.8);
      spherePosition = vec3(-1.5*sin(time/4), 0.25, -1.5*cos(time/4));
      sphere = sdSphere(p - spherePosition, .5);

      vec3 boxPosition = vec3(0, 0, 0);
      float box = sdBox(q, vec3(.15));
      box = sdBox(q, size);
      float cb = sdCrosscube(q, size);
      box = max(-cb, box);

      for (int i = 0; i < 4; i++) {
        size *= 1./3.;
        float m = size.x*2.;
        q = mod(q-0.5*m, m)-0.5*m;
        cb = sdCrosscube(q, size);
        box = max(box, -cb);
      };

      return smoothUnion(box, sphere, .4);
    case 2: 
      q = p;
      q.yz += time/4;
      q = fract(q) - .5;

      spherePosition = vec3(-1.5*sin(time/4), 0.25, -1.5*cos(time/4));
      spherePosition.z = spherePosition.z + posOffset;
      sphere = sdSphere(p - spherePosition, .5);

      boxPosition = vec3(0, 0, 0);
      box = sdBox(q, vec3(.15));

      ground = p.y + .75;

      return smoothUnion(ground, smoothUnion(box, sphere, .4), .2);
    case 3:
      p = p * rotationMatrix;
      q = p;

      q.xy += .5;
      q.yz += .15 * time;
      q.x += .1 * time;
      q = fract(q) - .5;

      spherePosition = vec3(-1.5, 0.25, -1.5);
      sphere = sdSphere(q, 0.65);

      boxPosition = vec3(0, 0, 0);
      box = sdBox(q, vec3(.5));

      return max(-sphere, box);
 case 4:
      p = p * rotationMatrix;
      q = p;

      // Twist deformation along Y-axis
      float twist = 1.5 * sin(time * 0.5);
      float ca = cos(twist * q.y);
      float sa = sin(twist * q.y);
      mat2 twistMat = mat2(ca, -sa, sa, ca);
      q.xz = twistMat * q.xz;

      // Wave displacement to make it feel alive
      q.x += 0.2 * sin(3.0 * q.y + time * 2.0);
      q.z += 0.2 * cos(3.0 * q.y + time * 2.0);

      // Central pulsating sphere
      float pulsate = 0.4 + 0.1 * sin(time * 4.0);
      sphere = sdSphere(q, pulsate);

      // Layered shells for the wormhole walls
      float ring1 = sdSphere(q * vec3(1.0, 0.5, 1.0), 1.0);
      float ring2 = sdSphere(q * vec3(1.0, 0.5, 1.0), 1.5);
      float ring3 = sdSphere(q * vec3(1.0, 0.5, 1.0), 2.0);

      // // Blend the rings
      float wormhole = smoothUnion(ring1, ring2, 0.3);
      wormhole = smoothUnion(wormhole, ring3, 0.3);

      // Subtract the core to make it hollow
      sphere1 = sdSphere(q, 2.3);
      return max(wormhole, -sphere1);
    case 5:
      p = p * rotationMatrix;
      return mandelbulb(p);
  }
};

vec3 getNormal(vec3 p) {
    float eps = 0.0005;
    return normalize(vec3(
        distCase(p + vec3(eps,0,0)) - distCase(p - vec3(eps,0,0)),
        distCase(p + vec3(0,eps,0)) - distCase(p - vec3(0,eps,0)),
        distCase(p + vec3(0,0,eps)) - distCase(p - vec3(0,0,eps))
    ));
};

void main(void) {
  vec2 uv = vec2((gl_FragCoord.x*2.0-width)/height, (gl_FragCoord.y*2.0-height)/height);

  vec3 ro = vec3(0.0, 0.0, -3.0 - posOffset);     // origin
  vec3 rd = normalize(vec3(uv, 1));               // direction
  vec3 col = vec3(0);                             // final color

  float t = 0.;

  for (int i = 0; i < renderSteps; i++) {
    vec3 p = ro + rd * t;

    float d = distCase(p);

    t += d;

    //* COLORING ACCORDING TO RENDER STEPS NUMBER
    col = vec3(i)/(80 + renderSteps/80 * 20.0);

    //* SIMPLE LAMBERT SHADING
    // vec3 normal = getNormal(p);
    // float diff = max(dot(normal, (vec3(0, 0, 1))), 0.0);
    // col = vec3(diff);

    // float smoothIter = float(i) - log2(log(r)/log(2.0));
    // float tCol = smoothIter * 0.1 + time * 0.1;
    // col = 0.5 + 0.5*cos(6.2831*(vec3(0.3, 0.5, 0.7) + tCol));


    if (d < .0001 || t > renderSteps * 2.25) break;
  };

  fragColor = vec4(col, 1);
};

//* rough operations
// min(d1, d2) Union
// max(d1, d2) Intersection
// max(-d1, d2) Substraction