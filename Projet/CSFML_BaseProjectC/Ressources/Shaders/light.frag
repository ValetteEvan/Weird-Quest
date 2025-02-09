uniform vec2 u_resolution;
uniform sampler2D u_texture;
uniform vec2 u_pos;
uniform vec2 u_vpos;
uniform vec2 u_size;
uniform vec3 u_color;
uniform float u_angle;
uniform float u_rotation;
uniform float u_strength;
uniform vec2 u_vsize;
uniform int u_lightForm;

float lerp (float a, float b, float t){
    return (b - a) * t + a; 
}

float circleShape(vec2 pos, float radius)
{
    return 0. + lerp(u_strength, 0., length(u_pos - pos) / radius);
}

float rectShape(vec2 pos, vec2 size, float rotation)
{
    vec2 halfSize = size / 3.;
    vec2 rotatedPos = vec2(cos(rotation) * (pos.x - u_pos.x) + sin(rotation) * (pos.y - u_pos.y) + u_pos.x, -sin(rotation) * (pos.x - u_pos.x) + cos(rotation) * (pos.y - u_pos.y) + u_pos.y);
    vec2 clippedDistance = max(abs(rotatedPos - u_pos) - halfSize, 0.0);
    return length(clippedDistance);
}

float coneShape(vec2 pos, float angle, float rotation, float range)
{
    angle *= .5;
    vec2 dir = normalize(pos - u_pos);
    float dotProduct = dot(dir, vec2(cos(angle + rotation), sin(angle + rotation)));
    float coneFactor = smoothstep(cos(angle), 1.0, dotProduct);
    float distance = length(u_pos - pos);
    return coneFactor * smoothstep(range, 0.0, distance) * (1.0 - distance / range);
}

void main() {
    vec2 uv = gl_TexCoord[0].xy;
    vec4 pixel = texture2D(u_texture, uv);
    gl_FragColor = pixel;
	
	u_pos.x = u_pos.x - (u_vpos.x - (u_vsize.x / 2.f));
	u_pos.y = u_pos.y - (u_vpos.y - (u_vsize.y / 2.f));

	vec2 radScale;
	radScale.x = 1920. /  u_vsize.x;
	radScale.y =  1080. / u_vsize.y;

	if (u_pos.y > u_vpos.y) {
		u_pos.y -= 2 * (u_pos.y - u_vpos.y) + 2 * (u_vpos.y - (u_vsize.y/2.0)) ;

	}
	else {
		u_pos.y += 2 * (u_vpos.y - u_pos.y) - 2 * (u_vpos.y - (u_vsize.y/2.0));
	}
	
    if(u_lightForm == 0)
    {
        float circle = circleShape(gl_FragCoord.xy, u_size.x * radScale.x);
        gl_FragColor = vec4(vec3(u_color.r, u_color.g, u_color.b), circle);
    }
    if(u_lightForm == 1)
    {
         float rect = rectShape(gl_FragCoord.xy, u_size.xy * radScale.xy, u_rotation);
         gl_FragColor = vec4(u_color, 1.0 - smoothstep(0.0, 1.0, rect / u_size.x))*u_strength ;
    }
    if(u_lightForm == 2)
    {    
         float cone = coneShape(gl_FragCoord.xy, u_angle, u_rotation, u_size.y * radScale.y);
        gl_FragColor = vec4(vec3(u_color.r, u_color.g, u_color.b), cone) * u_strength;
    }
}

