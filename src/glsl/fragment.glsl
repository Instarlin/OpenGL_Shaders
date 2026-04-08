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

float sdPlane(vec3 p, vec3 n, float h) {
  return dot(p, n) + h;
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

vec3 spherePlaneCenter() {
  return vec3(0.0, 0.4 + 0.08 * sin(time * 1.35), 0.15 * cos(time * 0.75));
}

vec2 mapSpherePlane(vec3 p) {
  float sphere = sdSphere(p - spherePlaneCenter(), 0.65);
  float plane = sdPlane(p, vec3(0.0, 1.0, 0.0), 0.35);

  if (sphere < plane) {
    return vec2(sphere, 1.0);
  }

  return vec2(plane, 2.0);
}

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
    case 6:
      return mapSpherePlane(p).x;
  }

  return mapSpherePlane(p).x;
};

vec3 getNormal(vec3 p) {
    float eps = 0.0005;
    return normalize(vec3(
        distCase(p + vec3(eps,0,0)) - distCase(p - vec3(eps,0,0)),
        distCase(p + vec3(0,eps,0)) - distCase(p - vec3(0,eps,0)),
        distCase(p + vec3(0,0,eps)) - distCase(p - vec3(0,0,eps))
    ));
};

mat3 cameraBasis(vec3 ro, vec3 target) {
  vec3 forward = normalize(target - ro);
  vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), forward));
  vec3 up = cross(forward, right);
  return mat3(right, up, forward);
}

vec3 skyColor(vec3 rd) {
  float horizon = clamp(rd.y * 0.5 + 0.5, 0.0, 1.0);
  vec3 sky = mix(vec3(0.22, 0.31, 0.46), vec3(0.82, 0.9, 0.98), horizon);
  float sun = pow(max(dot(rd, normalize(vec3(-0.6, 0.75, -0.4))), 0.0), 64.0);
  return sky + vec3(1.0, 0.87, 0.7) * sun * 0.2;
}

float softShadow(vec3 ro, vec3 rd, float tMin, float tMax) {
  float shadow = 1.0;
  float t = tMin;

  for (int i = 0; i < 48; i++) {
    float h = mapSpherePlane(ro + rd * t).x;
    if (h < 0.0005) {
      return 0.0;
    }

    shadow = min(shadow, 12.0 * h / t);
    t += clamp(h, 0.02, 0.3);

    if (t > tMax) {
      break;
    }
  }

  return clamp(shadow, 0.0, 1.0);
}

float ambientOcclusion(vec3 p, vec3 n) {
  float occ = 0.0;
  float scale = 1.0;

  for (int i = 1; i <= 5; i++) {
    float h = 0.06 * float(i);
    float d = mapSpherePlane(p + n * h).x;
    occ += (h - d) * scale;
    scale *= 0.6;
  }

  return clamp(1.0 - occ, 0.0, 1.0);
}

vec3 shadeSpherePlane(vec3 ro, vec3 rd) {
  float t = 0.0;
  float material = 0.0;
  bool hit = false;

  for (int i = 0; i < renderSteps; i++) {
    vec3 p = ro + rd * t;
    vec2 hitInfo = mapSpherePlane(p);

    if (hitInfo.x < 0.0005) {
      material = hitInfo.y;
      hit = true;
      break;
    }

    t += hitInfo.x;
    if (t > 40.0) {
      break;
    }
  }

  if (!hit) {
    return skyColor(rd);
  }

  vec3 p = ro + rd * t;
  vec3 normal = getNormal(p);
  vec3 lightDir = normalize(vec3(-0.6, 0.75, -0.4));
  vec3 viewDir = -rd;
  vec3 halfVec = normalize(lightDir + viewDir);

  float shadow = softShadow(p + normal * 0.01, lightDir, 0.02, 12.0);
  float diff = max(dot(normal, lightDir), 0.0) * shadow;
  float spec = pow(max(dot(normal, halfVec), 0.0), 96.0) * shadow;
  float fresnel = pow(1.0 - max(dot(normal, viewDir), 0.0), 5.0);
  float ao = ambientOcclusion(p, normal);

  vec3 baseColor = vec3(0.95, 0.36, 0.2);
  if (material > 1.5) {
    float tiles = mod(floor(p.x) + floor(p.z), 2.0);
    baseColor = mix(vec3(0.88), vec3(0.22), tiles);
  }

  vec3 color = baseColor * (0.16 * ao + 0.95 * diff);
  color += vec3(1.0, 0.95, 0.85) * spec * mix(1.0, 0.35, step(1.5, material));
  color += skyColor(reflect(rd, normal)) * mix(0.18, 0.05, step(1.5, material));
  color += baseColor * fresnel * 0.12;

  if (material > 1.5) {
    float rim = smoothstep(0.0, 0.8, 1.0 - max(dot(normal, viewDir), 0.0));
    color += vec3(0.03, 0.04, 0.05) * rim;
  }

  return color;
}

void main(void) {
  vec2 uv = vec2((gl_FragCoord.x*2.0-width)/height, (gl_FragCoord.y*2.0-height)/height);

  if (menuCase == 6) {
    float yaw = -mouseX * 0.01;
    float pitch = clamp(-mouseY * 0.006, -0.6, 0.85);
    float cameraDistance = max(2.2, 4.2 + posOffset);
    vec3 target = vec3(0.0, 0.2, 0.0);
    vec3 orbitDirection = normalize(vec3(
      sin(yaw) * cos(pitch),
      0.35 + sin(pitch),
      -cos(yaw) * cos(pitch)
    ));
    vec3 ro = target + orbitDirection * cameraDistance;
    vec3 rd = normalize(cameraBasis(ro, target) * vec3(uv, 1.7));

    fragColor = vec4(shadeSpherePlane(ro, rd), 1.0);
    return;
  }

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
