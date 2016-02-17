// Robert Winkler
#version 330


// Created by inigo quilez - iq/2013
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
// https://www.shadertoy.com/view/4dXGzn
// https://www.youtube.com/watch?v=_1FgCX0-Ny4


uniform vec3 iResolution;
uniform float iGlobalTime;

out vec4 fragColor;


float segm( float a, float b, float c, float x )
{
	return smoothstep(a-c,a,x) - smoothstep(b,b+c,x);
}


void main()
{
	vec2 q = gl_FragCoord.xy/iResolution.xy;
	vec2 p = (2.0*gl_FragCoord.xy-iResolution.xy)/min(iResolution.y,iResolution.x);

	float rate = 0.6;
	
	p += 0.3 * vec2(sin(rate*iGlobalTime), cos(rate*iGlobalTime));
	
	//p *= 0.65; // zoom in

	float a = atan(p.x,p.y);
	float r = length(p);

	//[-0.2, 0.6]
	float s = 0.2 + 0.6*sin(a*8.0 + (rate*2)*iGlobalTime);

	//[ ]
	float d = 0.5 + 0.2 * pow(s, 1.0);
	float h = r/d;
	float f = segm(0.1, 0.98, 0.02, h);//smoothstep(0.92, 1.0, h);

	s = 0.5 + 0.6 * sin(a*16);
	h = r/s;
	f *= smoothstep(1.02, 1.04, h);

	//s = sin(4.0*a + 0.4*iGlobalTime);
	//f = 1.0 - step(s, r);

/*
	float b = pow(0.5 + 0.5*sin(3.0*iGlobalTime),500.0);
	vec2  e = vec2( abs(p.x)-0.15,(p.y-0.1)*(1.0+10.0*b) );
	float g = 1.0 - (segm(0.06,0.09,0.01,length(e)))*step(0.0,e.y);

	float t = 0.5 + 0.5*sin(12.0*iGlobalTime);
	vec2  m = vec2( p.x, (p.y+0.15)*(1.0+10.0*t) );
	g *= 1.0 - (segm(0.06,0.09,0.01,length(m)));
	*/

	vec3 bcol = vec3(0.0);
	//bcol = mix(bcol, vec3(0.0, 0.0, 1.0), );

	//bcol *= 0.85 + 0.15*q.x*q.y;
	//bcol *= 0.5 + 0.5*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.25 );

	fragColor = vec4(mix(bcol, vec3(1.0,0.85,0.0), f), 1.0);
}



