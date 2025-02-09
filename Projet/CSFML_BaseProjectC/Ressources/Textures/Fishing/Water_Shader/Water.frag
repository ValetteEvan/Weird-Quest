#version 130

uniform sampler2D u_current_texture;
uniform sampler2D u_distortion_map_texture;
uniform float u_time;
uniform float u_level;
uniform int u_distortion_level;

void main()
{
    // Get the color of the noise texture at a position the current fragment position offset by the time
    vec4 noiseTexCol = texture2D(u_distortion_map_texture, vec2(gl_TexCoord[0].x + 0.025 * u_time, gl_TexCoord[0].y + 0.025 * u_time));
    
    // Reduce the offset
    float reducedOffset = noiseTexCol.r / u_distortion_level;

    // Upper part is normal
    if ((1 - gl_TexCoord[0].y) + reducedOffset < u_level)
    {
        // multiply it by the color
        gl_FragColor = texture2D(u_current_texture, gl_TexCoord[0].xy);
    }
    else
    {
        // Get the color of the screen at the offset location
        vec4 col = texture2D(u_current_texture, gl_TexCoord[0].xy + vec2(reducedOffset, reducedOffset));

        // Set the fragment color
        gl_FragColor = vec4(col.r / 3.44594, col.g, col.b / 1.024, col.a);
    }
}