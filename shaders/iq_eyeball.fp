// fragment shader for shadertoy
// Taken from iq's video tutorial
// https://www.youtube.com/watch?v=emjuqqyq_qc


// Robert Winkler
#version 130



// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.

// Simplex Noise (http://en.wikipedia.org/wiki/Simplex_noise), a type of gradient noise
// that uses N+1 vertices for random gradient interpolation instead of 2^N as in regular
// latice based Gradient Noise.

/*
vec2 hash( vec2 p )
{
	p = vec2( dot(p,vec2(127.1,311.7)),
			  dot(p,vec2(269.5,183.3)) );

	return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p )
{
	const float K1 = 0.366025404; // (sqrt(3)-1)/2;
	const float K2 = 0.211324865; // (3-sqrt(3))/6;

	vec2 i = floor( p + (p.x+p.y)*K1 );
	
	vec2 a = p - i + (i.x+i.y)*K2;
	vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
	vec2 b = a - o + K2;
	vec2 c = a - 1.0 + 2.0*K2;

	vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );

	vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));

	return dot( n, vec3(70.0) );
	
}
*/

//alternative noise implementation
float hash( float n ) {
	return fract(sin(n)*43758.5453123);
}

float noise( in vec2 x ) {
	vec2 p = floor(x);
	vec2 f = fract(x);

	f = f*f*(3.0-2.0*f);

	float n = p.x + p.y*57.0;

	return mix(mix( hash(n+  0.0), hash(n+  1.0),f.x), mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
}



mat2 m = mat2(0.8, 0.6, -0.6, 0.8);

float fbm(in vec2 p)
{
	float f = 0.0;
	f += 0.5000*noise(p); p *= m*2.02;
	f += 0.2500*noise(p); p *= m*2.03;
	f += 0.1250*noise(p); p *= m*2.01;
	f += 0.0625*noise(p); p *= m*2.04;
	f /= 0.9375;
	return f;
}


uniform vec3 iResolution;
uniform float iGlobalTime;

out vec4 fragColor;

void main()
{
	vec2 uv = gl_FragCoord.xy / iResolution.xy;
	vec2 p = -1.0 + 2.0*uv;
	p.x *= iResolution.x/iResolution.y;
	
	//p.x -= 0.75;
	
	float r = sqrt(dot(p, p));
	float a = atan(p.y, p.x);
	
	vec3 bg_col = vec3(0.05);
	vec3 col = bg_col;
	
	float ss = 0.5 + 0.5*sin(1.5*iGlobalTime);
	float anim = 1.0 + 0.1*ss*clamp(1.0-r, 0.0, 1.0);
	r *= anim;

	if (r < 0.8) {
		col = vec3(0.0, 0.3, 0.4);

		float f = fbm(5.0*p);
		col = mix(col, vec3(0.2, 0.5, 0.4), f);

		f = 1.0 - smoothstep(0.2, 0.5, r);
		col = mix(col, vec3(0.9, 0.6, 0.2), f);

		a += 0.05*fbm(20.0*p);

		f = smoothstep(0.3, 1.0, fbm(vec2(6.0*r, 20.0*a)));
		col = mix(col, vec3(1.0), f);

		f = smoothstep(0.4, 0.9, fbm(vec2(10.0*r, 15.0*a)));
		col *= 1.0 - 0.5*f;

		f = smoothstep(0.6, 0.8, r);
		col *= 1.0 - 0.5*f;

		f = smoothstep(0.2, 0.25, r);
		col *= f;


		f = 1.0 - smoothstep(0.0, 0.3, length(p - vec2(0.24, 0.2)));
		col += vec3(1.0, 0.9, 0.8)*f*0.8;


		f = smoothstep(0.75, 0.8, r);
		col = mix(col, bg_col, f);
	}
	

		
	
	fragColor = vec4(col, 1.0);
}






