uniform float u_Time;
uniform sampler2D u_TexBase;
uniform sampler2D u_TexNoisemap;
uniform vec2 u_Res;

// Multiplication blend mode
vec3 blendMultiply(vec3 base, vec3 blend) { return base*blend; }
vec3 blendMultiply(vec3 base, vec3 blend, float opacity) { return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity)); }

// Overlay (incrustation) blend mode
float blendOverlay(float base, float blend) { return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend)); }
vec3 blendOverlay(vec3 base, vec3 blend) { return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b)); }
vec3 blendOverlay(vec3 base, vec3 blend, float opacity) { return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity)); }

void main() {
	float ratio = u_Res.x / u_Res.y;
	vec2 uv = gl_FragCoord / u_Res;
	float ar = u_Res.x / u_Res.y;
	vec2 uvRatio = vec2(uv.x * ar, uv.y);

	vec2 offset1 = vec2(sin(u_Time) + .5, cos(u_Time) + .8) * .125;
	vec4 clrPx1 = texture2D(u_TexNoisemap, uvRatio * .45 + offset1);
	vec2 offset2 = vec2(sin(u_Time * 2.5) + .5, cos(u_Time * 2.5) + .8) * .0625;
	vec4 clrPx2 = texture2D(u_TexNoisemap, uvRatio * .35 + offset2);
	gl_FragColor = texture2D(u_TexBase, gl_TexCoord[0].xy);

	float avg1 = ((clrPx1.r + clrPx1.g + clrPx1.b) * .333333) * 1.5 - .333333;
	float avg2 = ((clrPx2.r + clrPx2.g + clrPx2.b) * .333333) * 1.5 - .333333;
	gl_FragColor.rgb = blendOverlay(gl_FragColor.rgb, vec3(avg1 * .25, avg1, 0.), .5);
	gl_FragColor.rgb = blendOverlay(gl_FragColor.rgb, vec3(0., avg2 * .25, avg2), .5);
}