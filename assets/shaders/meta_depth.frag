uniform sampler2D texture;

void main()
{
	vec4 meta = texture2D(texture, gl_TexCoord[0].xy);

	gl_FragColor = meta;
    gl_FragDepth = 1.0 - meta.r;
}