#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

uniform vec3 lightPos;  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;

void main()
{   
    vec3 pointLightColor=vec3(0.85f);
    vec3 norm = normalize(Normal);

    float ambientStrength = 0.1;
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    
    //point light
    vec3 lightDir = normalize(lightPos - FragPos);  
    
    vec3 ambient = ambientStrength * pointLightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * pointLightColor; 
    
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec  * lightColor;  
    vec3 point_result = (ambient + diffuse + specular) ;

    //directional
    lightDir=normalize(vec3(-1.0f,1.0f,1.0f));
    vec3 dirLightColor=vec3(0.75f);

    ambient = ambientStrength * dirLightColor;

    diff = max(dot(norm, lightDir), 0.0);
    diffuse = diff * dirLightColor; 

    reflectDir = reflect(-lightDir, norm);  
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    
    specular = specularStrength * spec  * dirLightColor;  

    vec3 directional_result= (ambient + diffuse + specular) ;

    vec3 final_result=directional_result+point_result;
    final_result =final_result * texture(texture_diffuse1,TexCoords).xyz;
    
    
    FragColor = vec4(final_result, 1.0);

   
}  
