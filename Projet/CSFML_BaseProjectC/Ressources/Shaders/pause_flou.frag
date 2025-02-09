uniform sampler2D u_TexBase;
uniform vec2 u_Res;

// Normal blend mode
vec3 blendNormal(vec3 base, vec3 blend) { return blend; }
vec3 blendNormal(vec3 base, vec3 blend, float opacity) { return (blendNormal(base, blend) * opacity + base * (1.0 - opacity)); }

float max(float _a, float _b) {
	if (_a > _b) return _a;
	else return _b;
}

float max(float _a, float _b, float _c) {
	if (_a > _b && _a > _c) return _a;
	if (_b > _a && _b > _c) return _b;
	if (_c > _a && _c > _b) return _c;
	return _a;
}

float clamp(float _a, float _l, float _u) {
	if (_a <= _l) return _l;
	if (_a >= _u) return _u;
	return _a;
}

void main() {
	float ratio = u_Res.x / u_Res.y;
	vec4 px1 = vec4(0.);

	float xOff = .0005;
	float yOff = .0005 * ratio;
	int itr = 21;
	int itr_half = (itr - 1) / 2;

	for (int i = 0; i < itr; i++) {
		for (int j = 0; j < itr; j++) {
			vec2 uv = gl_TexCoord[0].xy + vec2(xOff * (i - itr_half), yOff * (j - itr_half));
			uv.y = 1. - uv.y;
			px1 += texture2D(u_TexBase, uv) * (1. / (itr * itr));
		}
	}
	px1.a = 1.;

	gl_FragColor = px1;
}