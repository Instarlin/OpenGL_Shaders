#version 460
out vec4 fragColor;
uniform float time;

mat3 setCamera( vec3 ro, vec3 ta, float cr ) {
  vec3 cw = normalize(ta - ro);
  vec3 cp = vec3(sin(cr), cos(cr),0.0);
  vec3 cu = normalize( cross(cw,cp) );
  vec3 cv = normalize( cross(cu,cw) );
  return mat3( cu, cv, cw );
};

vec2 hash( vec2 p ) {
	p = vec2( dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)) );
	return -1.0 + 2.0*fract(sin(p) * 43758.5453123);
};

float formRG( vec2 p ) {
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

float noise( vec3 x ) {
  vec3 p = floor(x);
  vec3 f = fract(x);
  f = f*f*(3.0-2.0*f);

  vec2 uv = (p.xy+vec2(37.0,239.0)*p.z) + f.xy;
  vec2 rg = textureLod(formRG(uv), (uv+0.5)/256.0, 0.0).yx;
  return mix( rg.x, rg.y, f.z )*2.0-1.0;
};

float map( in vec3 p ) {
  vec3 q = p - vec3(0.0,0.1,1.0)*time;
  float g = 0.5+0.5*noise( q*0.3 );

  float f;
  f  = 0.50000*noise( q ); q = q*2.02;
  f += 0.25000*noise( q ); q = q*2.23;
  f += 0.12500*noise( q ); q = q*2.41;
  f += 0.06250*noise( q ); q = q*2.62;
  f += 0.03125*noise( q ); 

  f = mix( f*0.1-0.5, f, g*g );

  return 1.5*f - 0.5 - p.y;
};

const vec3 sundir = normalize( vec3(1.0, 0.0, -1.0) );

vec4 raymarch( vec3 ro, vec3 rd, vec3 bgcol ) {
// bounding planes	
  const float yb = -3.0;
  const float yt =  0.6;
  float tb = (yb-ro.y)/rd.y;
  float tt = (yt-ro.y)/rd.t;

  // find tigthest possible raymarching segment
  float tmin, tmax;
  if( ro.y>yt ) {
  // above top plane
    if( tt<0.0 ) return vec4(0.0); // early exit
    tmin = tt;
    tmax = tb;
  } else {
  // inside clouds slabs
    tmin = 0.0;
    tmax = 60.0;
    if( tt>0.0 ) tmax = min( tmax, tt );
    if( tb>0.0 ) tmax = min( tmax, tb );
  }

  // dithered near distance
  float t = tmin;

  // raymarch loop
  vec4 sum = vec4(0.0);
  for( int i = 0; i < 190; i++ ) {
    // step size
    float dt = max(0.05,0.02*t);

    // sample cloud
    vec3 pos = ro + t*rd;
    float den = map( pos );
    if( den>0.1 ) { // if inside 
      // do lighting
      float dif = clamp((den - map(pos+0.3*sundir))/0.25, 0.0, 1.0 );
      vec3  lin = vec3(0.65,0.65,0.75)*1.1 + 0.8*vec3(1.0,0.6,0.3)*dif;
      vec4  col = vec4( mix( vec3(1.0,0.93,0.84), vec3(0.25,0.3,0.4), den ), den );
      col.xyz *= lin;
      // fog
      col.xyz = mix(col.xyz,bgcol, 1.0-exp2(-0.1*t));
      // composite front to back
      col.w    = min(col.w*8.0*dt,1.0);
      col.rgb *= col.a;
      sum += col*(1.0-sum.a);
    };
    // advance ray
    t += dt;
    // until far clip or full opacity
    if( t > tmax || sum.a > 0.99 ) break;
  };

  return clamp( sum, 0.0, 1.0 );
};

vec4 render( vec3 ro, vec3 rd ) {
  float sun = clamp( dot(sundir, rd), 0.0, 1.0 );

  // background sky
  vec3 col = vec3(0.76, 0.75, 0.95);

  // clouds    
  vec4 res = raymarch( ro, rd, col );
  col = col*(1.0-res.w) + res.xyz; 

  // tonema
  return vec4( col, 1.0 );
}

void main(void) {
  vec2 p = vec2((gl_FragCoord.x*2.0-750)/550, (gl_FragCoord.y*2.0-550)/550);

  // camera
  vec3 ro = vec3(0.0, 1.1, 0.0);
  vec3 ta = vec3(0.0, -1.0, 0.0);
  mat3 ca = setCamera( ro, ta, 0.07*cos(0.25*time) );
  // ray
  vec3 rd = ca * normalize(vec3(p.xy, 1.5));

  fragColor = render( ro, rd );
};






mat3 m = mat3( 0.00,  0.80,  0.60,
              -0.80,  0.36, -0.48,
              -0.60, -0.48,  0.64 );
float hash( float n )
{
    return fract(sin(n)*43758.5453);
}

float noise( in vec3 x )
{
    vec3 p = floor(x);
    vec3 f = fract(x);

    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*57.0 + 113.0*p.z;

    float res = mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                        mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
                    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                        mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
    return res;
}

float fbm( vec3 p )
{
    float f;
    f  = 0.5000*noise( p ); p = m*p*2.02;
    f += 0.2500*noise( p ); p = m*p*2.03;
    f += 0.1250*noise( p );
    return f;
}

float scene(vec3 p)
{	
	return .1-length(p)*.05+fbm(p*.3);
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 q = fragCoord.xy / iResolution.xy;
    vec2 v = -1.0 + 2.0*q;
    v.x *= iResolution.x/ iResolution.y;

	#if 0
    vec2 mo = -1.0 + 2.0*iMouse.xy / iResolution.xy;
    #else
	vec2 mo = vec2(iTime*.1,cos(iTime*.25)*3.);
	#endif

    // camera by iq
    vec3 org = 25.0*normalize(vec3(cos(2.75-3.0*mo.x), 0.7-1.0*(mo.y-1.0), sin(2.75-3.0*mo.x)));
	vec3 ta = vec3(0.0, 1.0, 0.0);
    vec3 ww = normalize( ta - org);
    vec3 uu = normalize(cross( vec3(0.0,1.0,0.0), ww ));
    vec3 vv = normalize(cross(ww,uu));
    vec3 dir = normalize( v.x*uu + v.y*vv + 1.5*ww );
	vec4 color=vec4(.0);
	
	
	
	const int nbSample = 64;
	const int nbSampleLight = 6;
	
	float zMax         = 40.;
	float step         = zMax/float(nbSample);
	float zMaxl         = 20.;
	float stepl         = zMaxl/float(nbSampleLight);
    vec3 p             = org;
    float T            = 1.;
    float absorption   = 100.;
	vec3 sun_direction = normalize( vec3(1.,.0,.0) );
    
	for(int i=0; i<nbSample; i++)
	{
		float density = scene(p);
		if(density>0.)
		{
			float tmp = density / float(nbSample);
			T *= 1. -tmp * absorption;
			if( T <= 0.01)
				break;
				
				
			 //Light scattering
			float Tl = 1.0;
			for(int j=0; j<nbSampleLight; j++)
			{
				float densityLight = scene( p + normalize(sun_direction)*float(j)*stepl);
				if(densityLight>0.)
                	Tl *= 1. - densityLight * absorption/float(nbSample);
                if (Tl <= 0.01)
                    break;
			}
			
			//Add ambiant + light scattering color
			color += vec4(1.)*50.*tmp*T +  vec4(1.,.7,.4,1.)*80.*tmp*T*Tl;
		}
		p += dir*step;
	}    

    fragColor = color;

}