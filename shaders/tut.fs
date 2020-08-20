#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    FragColor = mix(texture(ourTexture1, TexCoord),texture(ourTexture2, vec2(-TexCoord.x,TexCoord.y)),0.2);
    //FragColor = vec4(1.0,0.0,0.0,1.0);
    //FragColor = texture(ourTexture2, TexCoord);
}