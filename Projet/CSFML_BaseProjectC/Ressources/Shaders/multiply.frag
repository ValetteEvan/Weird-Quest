uniform sampler2D u_TexBase;
uniform sampler2D u_TexBlend;

vec3 blendMultiply(vec3 base, vec3 blend) { return base*blend; }
vec3 blendMultiply(vec3 base, vec3 blend, float opacity) { return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity)); }

void main() {
    vec4 pixel1 = texture2D(u_TexBase, gl_TexCoord[0].xy);
    vec4 pixel2 = texture2D(u_TexBlend, vec2(gl_TexCoord[0].x, 1. - gl_TexCoord[0].y));
    
	vec4 clr;
    clr = vec4(blendMultiply(pixel1.rgb, pixel2.rgb, pixel2.a), pixel1.a);
    gl_FragColor = clr;
}