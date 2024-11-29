#version 460

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float timer;
uniform vec2 mousePos;

void main()
{ 
    //Chromatic Aberration 효과 적용
    float redOffset   =  0.01;
    float greenOffset =  0.001;
    float blueOffset  = -0.01;

    vec2 center = mousePos / textureSize(screenTexture,0).xy;
    vec2 dir =  TexCoords - center;
    
    float dis = sqrt(dir.x*dir.x + dir.y*dir.y);
    dis = pow(dis, 0.2) * (1.6 + sin(timer * 0.1) * 0.8);

    dir = normalize(dir) * dis;

    vec4 col = texture(screenTexture, TexCoords);

    col.r = texture(screenTexture, TexCoords + (dir * redOffset)).r;
    col.g = texture(screenTexture, TexCoords + (dir * greenOffset)).g;
    col.b = texture(screenTexture, TexCoords + (dir * blueOffset)).b;

    FragColor = col;

    //FragColor = vec4(mousePos,0.0,1.0);
}