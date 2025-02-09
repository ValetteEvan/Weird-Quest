uniform sampler2D currentTexture;
uniform vec3 currentColor;
uniform float a;

void main() {
	
	vec4 pixel = texture2D(currentTexture, gl_TexCoord[0].xy);
	vec3 colorVec3;
	if(pixel.r + pixel.g + pixel.b > 0.1){
		colorVec3 = mix(pixel.rgb, currentColor, pixel.a * 1.);
	}
	else {
		colorVec3 = pixel.rgb;
	}
	gl_FragColor = vec4(colorVec3, pixel.a); 
}