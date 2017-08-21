uniform sampler2D texture;

uniform vec2 offset;

void main()
{
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

             
    /*
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -12.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -11.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -10.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -9.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -8.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -7.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -6.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -5.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -4.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -3.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -2.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  -1.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  0.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  1.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  2.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  3.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  4.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  5.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  6.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  7.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  8.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  9.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  10.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  11.0);
    color += texture2D(texture, gl_TexCoord[0].xy + offset *  12.0);

    // multiply it by the color
    gl_FragColor = gl_Color * color / 25.0;
    */

    color += 0.024626 * texture2D(texture, gl_TexCoord[0].xy + offset *  -12.0);
    color += 0.027624 * texture2D(texture, gl_TexCoord[0].xy + offset *  -11.0);
    color += 0.03068 * texture2D(texture, gl_TexCoord[0].xy + offset *  -10.0);
    color += 0.033735 * texture2D(texture, gl_TexCoord[0].xy + offset *  -9.0);
    color += 0.036725 * texture2D(texture, gl_TexCoord[0].xy + offset *  -8.0);
    color += 0.039583 * texture2D(texture, gl_TexCoord[0].xy + offset *  -7.0);
    color += 0.042239 * texture2D(texture, gl_TexCoord[0].xy + offset *  -6.0);
    color += 0.044625 * texture2D(texture, gl_TexCoord[0].xy + offset *  -5.0);
    color += 0.046677 * texture2D(texture, gl_TexCoord[0].xy + offset *  -4.0);
    color += 0.048338 * texture2D(texture, gl_TexCoord[0].xy + offset *  -3.0);
    color += 0.049561 * texture2D(texture, gl_TexCoord[0].xy + offset *  -2.0);
    color += 0.050309 * texture2D(texture, gl_TexCoord[0].xy + offset *  -1.0);
    color += 0.050561 * texture2D(texture, gl_TexCoord[0].xy + offset *  0.0);
    color += 0.050309 * texture2D(texture, gl_TexCoord[0].xy + offset *  1.0);
    color += 0.049561 * texture2D(texture, gl_TexCoord[0].xy + offset *  2.0);
    color += 0.048338 * texture2D(texture, gl_TexCoord[0].xy + offset *  3.0);
    color += 0.046677 * texture2D(texture, gl_TexCoord[0].xy + offset *  4.0);
    color += 0.044625 * texture2D(texture, gl_TexCoord[0].xy + offset *  5.0);
    color += 0.042239 * texture2D(texture, gl_TexCoord[0].xy + offset *  6.0);
    color += 0.039583 * texture2D(texture, gl_TexCoord[0].xy + offset *  7.0);
    color += 0.036725 * texture2D(texture, gl_TexCoord[0].xy + offset *  8.0);
    color += 0.033735 * texture2D(texture, gl_TexCoord[0].xy + offset *  9.0);
    color += 0.03068 * texture2D(texture, gl_TexCoord[0].xy + offset *  10.0);
    color += 0.027624 * texture2D(texture, gl_TexCoord[0].xy + offset *  11.0);
    color += 0.024626 * texture2D(texture, gl_TexCoord[0].xy + offset *  12.0);
    gl_FragColor = gl_Color * color;
    
}