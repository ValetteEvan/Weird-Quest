uniform sampler2D u_TexBase;
uniform sampler2D u_TexBlend;

// Overlay (incrustation) blend mode
float blendOverlay(float base, float blend) { return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend)); }
vec3 blendOverlay(vec3 base, vec3 blend) { return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b)); }
vec3 blendOverlay(vec3 base, vec3 blend, float opacity) { return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity)); }

void main() {
    vec4 pixel1 = texture2D(u_TexBase, gl_TexCoord[0].xy);
    vec4 pixel2 = texture2D(u_TexBlend, vec2(gl_TexCoord[0].x, 1. - gl_TexCoord[0].y));
    
	vec4 clr;
    clr = vec4(blendOverlay(pixel1.rgb, pixel2.rgb, pixel2.a), pixel1.a);
    gl_FragColor = clr;
}