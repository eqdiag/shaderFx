#version 330 core


in vec2 uv;
out vec4 outColor;


uniform float dx;
uniform float dy;
uniform int blurSize;
uniform float time;
uniform float noiseStrength;


uniform sampler2D tex;

uniform bool invertEnabled;
uniform bool vignetteEnabled;
uniform bool blurEnabled;
uniform bool sobelEnabled;
uniform bool noiseEnabled;


vec3 toMono(vec3 color){
	float grey = 0.21*color.r + 0.72*color.g + 0.07*color.b;
	return vec3(grey);
}

float noiseRand(vec2 p){
	return fract(sin(p.x*100.0+ p.y*6574.)*3432.);
}

float noiseSmooth(vec2 p){
	vec2 lv = fract(p);
    vec2 idx = floor(p);
    
    lv = lv*lv*(3.-2.*lv);
    
    float bl = noiseRand(idx);
    float br = noiseRand(idx+vec2(1,0));
    float b = mix(bl, br, lv.x);
    
    float tl = noiseRand(idx+vec2(0,1));
    float tr = noiseRand(idx+vec2(1,1));
    float t = mix(tl, tr, lv.x);
    
    return mix(b, t, lv.y);
}

float noiseLayered(vec2 uv) {
    float c = noiseSmooth(uv*4.);
    c += noiseSmooth(uv*8.2)*.5;
    c += noiseSmooth(uv*16.7)*.25;
    c += noiseSmooth(uv*32.4)*.125;
    c += noiseSmooth(uv*64.5)*.0625;
    
    c /= 2.;
    
    return c;
}

void main()
{
	if(invertEnabled){
		outColor = 1.0 - texture(tex,uv);
	}
	else if(vignetteEnabled){
		vec4 texCol = texture(tex,uv);
		vec2 s = uv*2.0 - 1.0;
		float d = 1.2 - length(s);
		vec3 col = d*texCol.rgb;
		outColor = vec4(col,1.0);
	}
	else if(blurEnabled){
		//Uses an avg blur filter
		int extents = blurSize/2;
		vec3 col = vec3(0.0);
		for(int x = -extents;x <= extents;x++){
			for(int y =-extents;y<=extents;y++){
				col += texture(tex,uv + vec2(x*dx,y*dy)).rgb;
			}
		}
		col /= (blurSize*blurSize);
		outColor = vec4(col,1.0);
	}else if(sobelEnabled){
		vec3 top = texture(tex,uv +  vec2(0.0,dy)).rgb;
		vec3 bottom = texture(tex,uv + vec2(0.0,-dy)).rgb;
		vec3 left = texture(tex,uv + vec2(-dx,0.0)).rgb;
		vec3 right  = texture(tex,uv + vec2(dx,0.0)).rgb;
		vec3 topLeft = texture(tex,uv + vec2(-dx,dy)).rgb;
		vec3 topRight = texture(tex,uv + vec2(dx,dy)).rgb;
		vec3 botLeft = texture(tex,uv + vec2(-dx,-dy)).rgb;
		vec3 botRight = texture(tex,uv + vec2(dx,-dy)).rgb;
		vec3 gradx = -topLeft - 2 * left - botLeft + topRight   + 2 * right  + botRight;
		vec3 grady = -topLeft - 2 * top  - topRight   + botLeft + 2 * bottom + botRight;
		vec3 col = sqrt(gradx * gradx + grady * grady);
		outColor = vec4(col,1.0);
	}else if(noiseEnabled){
		vec2 sample = uv + noiseLayered(uv*10 + time)/(110.0-noiseStrength);
		vec3 col = texture(tex,sample).rgb;
		outColor = vec4(col,1.0);
	}
	else{
		outColor = texture(tex,uv);
	}

}