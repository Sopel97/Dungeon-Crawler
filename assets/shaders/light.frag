uniform sampler2D texture;

uniform vec2 lightTextureSize;
uniform vec2 metaTextureSize;
uniform sampler2D metaTexture;

void main()
{
    vec2 metaTexCoords = (gl_TexCoord[0].xy*lightTextureSize)/metaTextureSize;

    float offset = texture2D(metaTexture, metaTexCoords).r;

    vec2 lightTexCoords = vec2(
    gl_TexCoord[0].x + (offset*(256.0/4.0))/lightTextureSize.x,
    gl_TexCoord[0].y - (offset*(256.0/4.0))/lightTextureSize.y
    );

    vec4 texPixel = texture2D(texture, lightTexCoords);
    float a = 1.0 - texPixel.a;

    gl_FragColor = vec4(0.0, 0.0, 0.0, a);
}