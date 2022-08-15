// https://learnopengl.com

#version 330 core

out vec4 FragColor;
  
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;
in vec4 FragPosLightSpace2;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform sampler2D shadowMap2;


float ShadowCalculation(vec4 fragPosLightSpace, sampler2D shadowMap)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    return shadow;
}

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
	
    float constant;
    float linear;
    float quadratic;
};

uniform Light light;
uniform Light light2;

void main()
{
    vec4 Frag1;
    vec3 lightDir = normalize(light.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction)); 
    
    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // ambient
        vec3 ambient = light.ambient * texture(diffuseTexture, TexCoord).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(diffuseTexture, TexCoord).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess);
        vec3 specular = light.specular * spec * texture(diffuseTexture, TexCoord).rgb;  
        
        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse   *= attenuation;
        specular *= attenuation;   
            
        float shadow = ShadowCalculation(FragPosLightSpace, shadowMap);                      

        vec3 result = ambient + (1.0-shadow) * (diffuse + specular);
        Frag1 = vec4(result, 1.0);
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        
        Frag1 = vec4(light.ambient * texture(diffuseTexture, TexCoord).rgb, 1.0);
    }

    vec4 Frag2;
    vec3 lightDir2 = normalize(light2.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta2 = dot(lightDir2, normalize(-light2.direction)); 
    
    if(theta2 > light2.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // ambient
        vec3 ambient2 = light2.ambient * texture(diffuseTexture, TexCoord).rgb;
        
        // diffuse 
        vec3 norm2 = normalize(Normal);
        float diff2 = max(dot(norm2, lightDir), 0.0);
        vec3 diffuse2 = light2.diffuse * diff2 * texture(diffuseTexture, TexCoord).rgb;  
        
        // specular
        vec3 viewDir2 = normalize(viewPos - FragPos);
        vec3 reflectDir2 = reflect(-lightDir2, norm2);  
        float spec2 = pow(max(dot(viewDir2, reflectDir2), 0.0), light2.shininess);
        vec3 specular2 = light2.specular * spec2 * texture(diffuseTexture, TexCoord).rgb;  
        
        // attenuation
        float distance2    = length(light2.position - FragPos);
        float attenuation2 = 1.0 / (light2.constant + light2.linear * distance2 + light2.quadratic * (distance2 * distance2));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse2   *= attenuation2;
        specular2 *= attenuation2;   
            
        float shadow2 = ShadowCalculation(FragPosLightSpace2, shadowMap2);                      

        vec3 result = ambient2 + (1.0-shadow2) * (diffuse2 + specular2);
        Frag2 = vec4(result, 1.0);
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        
        Frag2 = vec4(light2.ambient * texture(diffuseTexture, TexCoord).rgb, 1.0);
    }

    FragColor = Frag1 + Frag2;

}