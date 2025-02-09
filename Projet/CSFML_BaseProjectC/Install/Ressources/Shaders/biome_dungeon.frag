uniform sampler2D u_TexBase;
uniform sampler2D u_TexVignette;

// Normal blend mode
vec3 blendNormal(vec3 base, vec3 blend) { return blend; }
vec3 blendNormal(vec3 base, vec3 blend, float opacity) { return (blendNormal(base, blend) * opacity + base * (1.0 - opacity)); }

// Multiply blend mode
vec3 blendMultiply(vec3 base, vec3 blend) { return base*blend; }
vec3 blendMultiply(vec3 base, vec3 blend, float opacity) { return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity)); }

// Overlay (incrustation) blend mode
float blendOverlay(float base, float blend) { return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend)); }
vec3 blendOverlay(vec3 base, vec3 blend) { return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b)); }
vec3 blendOverlay(vec3 base, vec3 blend, float opacity) { return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity)); }

// Soft light blend mode
float blendSoftLight(float base, float blend) { return (blend<0.5)?(2.0*base*blend+base*base*(1.0-2.0*blend)):(sqrt(base)*(2.0*blend-1.0)+2.0*base*(1.0-blend)); }
vec3 blendSoftLight(vec3 base, vec3 blend) { return vec3(blendSoftLight(base.r,blend.r),blendSoftLight(base.g,blend.g),blendSoftLight(base.b,blend.b)); }
vec3 blendSoftLight(vec3 base, vec3 blend, float opacity) { return (blendSoftLight(base, blend) * opacity + base * (1.0 - opacity)); }

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec2 uvInv = vec2(uv.x, 1. - uv.y);

    vec4 pixel1 = texture2D(u_TexBase, uvInv);
    vec4 pixel2 = texture2D(u_TexVignette, uv);
    
	vec4 clr = pixel1;
    clr = vec4(blendMultiply(clr.rgb, pixel2.rgb, pixel2.a), clr.a);
    gl_FragColor = clr;
}