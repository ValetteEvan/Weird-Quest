uniform sampler2D u_TexBase; /// Base texture
uniform sampler2D u_TexNoise; /// Noise map
uniform sampler2D u_TexLight; /// Light (Screen, 100%)
uniform sampler2D u_TexShade; /// Shade (Multiply, 100%)
uniform sampler2D u_TexGodrays; /// Godrays (Soft light, 100%)
/// Adjust blend opacities manually depending on what looks best

uniform float u_Time;
uniform float u_Level;
uniform int u_DistLevel;

// superposition 
float blendScreen(float base, float blend) { return 1.0-((1.0-base)*(1.0-blend)); }
vec3 blendScreen(vec3 base, vec3 blend) { return vec3(blendScreen(base.r,blend.r),blendScreen(base.g,blend.g),blendScreen(base.b,blend.b)); }
vec3 blendScreen(vec3 base, vec3 blend, float opacity) { return (blendScreen(base, blend) * opacity + base * (1.0 - opacity)); }

// Soft light blend mode
float blendSoftLight(float base, float blend) { return (blend<0.5)?(2.0*base*blend+base*base*(1.0-2.0*blend)):(sqrt(base)*(2.0*blend-1.0)+2.0*base*(1.0-blend)); }
vec3 blendSoftLight(vec3 base, vec3 blend) { return vec3(blendSoftLight(base.r,blend.r),blendSoftLight(base.g,blend.g),blendSoftLight(base.b,blend.b)); }
vec3 blendSoftLight(vec3 base, vec3 blend, float opacity) { return (blendSoftLight(base, blend) * opacity + base * (1.0 - opacity)); }

// Multiply blend mode
vec3 blendMultiply(vec3 base, vec3 blend) { return base*blend; }
vec3 blendMultiply(vec3 base, vec3 blend, float opacity) { return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity)); }

vec4 px(vec2 _offset) {
    vec2 uv = vec2(gl_TexCoord[0].x + _offset.x, gl_TexCoord[0].y + _offset.y);
    vec2 uv_flip = vec2(gl_TexCoord[0].x + _offset.x, 1. - gl_TexCoord[0].y + _offset.y);
    vec4 px1 = texture2D(u_TexBase, uv); /// Sampling the base texture
    vec4 px2 = texture2D(u_TexLight, uv_flip); /// Sampling the light overlay texture
    vec4 px3 = texture2D(u_TexShade, uv_flip); /// Sampling the shade overlay texture
    vec4 px4 = texture2D(u_TexGodrays, uv_flip); /// Sampling the godrays overlay texture

    vec4 pxShd = px1; /// Base texture (without shaders)
    pxShd = vec4(blendScreen(pxShd.rgb, px2.rgb, px2.a), pxShd.a); /// With the light shader
    pxShd = vec4(blendMultiply(pxShd.rgb, px3.rgb, px3.a), pxShd.a); /// With the shade shader
    pxShd = vec4(blendSoftLight(pxShd.rgb, px4.rgb, px4.a), pxShd.a); /// With the godrays shader

    return pxShd;
}

void main() {
    vec4 noiseTexCol = texture2D(u_TexNoise, vec2(gl_TexCoord[0].x + 0.1 * sin(u_Time * .2), gl_TexCoord[0].y + 0.1 * cos(u_Time * .17)));
    float reducedOffset = noiseTexCol.r / u_DistLevel;

    if ((1 - gl_TexCoord[0].y) + reducedOffset < u_Level) gl_FragColor = px(vec2(0.));
    else {
        vec4 col = px(vec2(reducedOffset));
        gl_FragColor = vec4(col.r / 3.44594, col.g, col.b / 1.024, col.a);
    }
}