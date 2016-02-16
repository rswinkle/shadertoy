// Robert Winkler
#version 330



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
	vec2 p_orig = (2.0*gl_FragCoord.xy-iResolution.xy)/min(iResolution.y,iResolution.x);

	//change this to make gear move faster/slower
	float rate = 1.4;
	float prongs = 8; //any even whole number works
	
	vec2 p = p_orig + 0.3 * vec2(sin(rate*iGlobalTime), cos(rate*iGlobalTime));
	

	float a = atan(p.x,p.y);
	float r = length(p);

	//[-0.2, 0.8]
	float offset = mod(prongs/2, 2.0);
	float s = 0.2 + 0.6*sin(a*prongs + (rate*prongs/2.0)*iGlobalTime + 3.14159*offset);

	//[ ]
	float d = 0.52 + 0.25 * pow(s, 1.0);
	float h = r/d;
	float f = segm(0.1, 0.98, 0.02, h);//smoothstep(0.92, 1.0, h);


	a = atan(p_orig.x, p_orig.y);
	r = length(p_orig);

	s = 0.3 + 0.6 * sin(a*prongs*1.5);
	d = 0.8 + 0.2 * pow(s, 1.0);
	h = r/d;

	//I'm sure there's a way to avoid the branch ...
	f = (h >= 1.02) ? smoothstep(1.02, 1.04, h) : f;

	//s = sin(4.0*a + 0.4*iGlobalTime);
	//f = 1.0 - step(s, r);


	vec3 bcol = vec3(0.0);
	//bcol = mix(bcol, vec3(0.0, 0.0, 1.0), );

	//bcol *= 0.85 + 0.15*q.x*q.y;
	//bcol *= 0.5 + 0.5*pow( 16.0*q.x*q.y*(1.0-q.x)*(1.0-q.y), 0.25 );

	fragColor = vec4(mix(bcol, vec3(0.55,0.55,0.55), f), 1.0);
}



