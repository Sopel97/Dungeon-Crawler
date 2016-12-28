uniform sampler2D texture;

uniform vec2 worldOffset;
uniform vec2 viewOffset;
uniform vec2 sourceTextureSize;
uniform vec2 destinationTextureSize;

void main()
{
    worldOffset.y = -worldOffset.y;
    vec2 fragCoords = floor(gl_FragCoord.xy - viewOffset + worldOffset * (destinationTextureSize/sourceTextureSize)) + vec2(0.5, 0.5);
    vec2 worldCoords = floor(fragCoords * (sourceTextureSize/destinationTextureSize)) + vec2(0.5, 0.5);
    
    vec2 texCoords = worldCoords/sourceTextureSize;

    texCoords -= worldOffset/sourceTextureSize;


	vec4 texPixel = texture2D(texture, texCoords);

	gl_FragColor = texPixel;
}