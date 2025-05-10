#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform vec2 u_windowSize;
uniform vec2 u_mouse;
uniform float u_time;

float dirac(float x)
{
	return max(-ceil(abs(x))+1., 0.);    
}


float rand (in float x) {
    return fract(sin(x * 43758.5453123));
}

float noise (in float x, in float amplitude) {
    
    float i = floor(x);  // integer
	float f = fract(x);  // fraction
	return amplitude*mix(rand(i), rand(i + 1.0), smoothstep(0.,1.,f));
}


void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x -= u_mouse.x / u_resolution.x;
	st.y += u_mouse.y / u_resolution.y;
	st.y -= (u_windowSize.y / u_resolution.y - 1.);
 	st.x *= u_resolution.x/u_resolution.y;		
    float grid_N = 10.;

    st *= grid_N;      // Scale up the space by 3
    
    st.x += u_time/(grid_N * 3.) + noise(u_time/(grid_N * 3.), grid_N * 3.);
    st.y += noise(u_time/(grid_N * 3.), grid_N * 3.);

    float row = floor(st.x);
    float col = floor(st.y);
    
    float rd_row = floor(noise((u_time+10000.)/6., grid_N * 1.7));
    float rd_col = floor(noise(u_time/6., grid_N * 1.7));
    
    st = fract(st); // Wrap around 1.0

    vec2 bl = step(vec2(0.1),st);
    float pct = bl.x * bl.y;
    vec2 tr = step(vec2(0.1),1.0-st);
    pct *= tr.x * tr.y;

    // Now we have 9 spaces that go from 0-1

    vec3 color = ((1. - pct) * vec3(160./255.,73./255.,64./255.) + pct * vec3(228./255.,94/255.,73./255.)) * (1. - mod(row + col, 2.));
    color += ((1. - pct) * vec3(35./255.,109./255.,213./255.) + pct *vec3(48./255.,143./255.,245./255.)) * mod(row + col, 2.);
    color *= (1. - dirac(mod(row, grid_N * 1.7)- rd_row) * dirac(mod(col, grid_N * 1.7) - rd_col));
    color += ((1. - pct) * vec3(179./255.,143./255.,36./255.) + pct * vec3(255./255.,189./255.,39./255.))*dirac(mod(row, grid_N * 1.7)- rd_row) * dirac(mod(col, grid_N * 1.7) - rd_col);

	gl_FragColor = vec4(color,0.6);
}
