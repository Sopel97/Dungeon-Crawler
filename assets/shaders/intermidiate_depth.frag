uniform sampler2D texture;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    vec4 meta = texture2D(texture, gl_TexCoord[0].xy + vec2(0.5, 0.0));

	gl_FragColor = color;
    gl_FragDepth = 1.0 - meta.r;
}