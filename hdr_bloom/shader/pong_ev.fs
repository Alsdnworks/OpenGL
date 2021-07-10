#version 330 core
in vec3 normal;
in vec2 texCoord;
in vec3 position;
out vec4 fragColor;
 
uniform vec3 viewPos;
struct Light {
    vec3 position;
    vec3 direction;
    vec2 cutoff;
    vec3 attenuation; //감쇠를 위한 식 구성을 위해 3개 변수가 필요하기때문
    //kc kl kq
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
 
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

uniform vec3 cameraPos; 
uniform samplerCube skybox; //skybox와 마찬가지로

void main() {
    vec3 I = normalize(position - cameraPos); //아이벡터     사선벡터와 물체법선벡터를 통해 반사벡터계산
    vec3 R = reflect(I, normalize(normal)); 
    vec3 texColor = texture2D(material.diffuse, texCoord).xyz;
    vec3 ambient = texColor * light.ambient;
//attenuation model= Fatt=1.0/kc+k1*d+kq*d^2 d는 광원과 물페사이거리, 광원이 어느정도까지 영향을 줄지 파라미터조정
    float dist = length(light.position - position);
    vec3 distPoly = vec3(1.0, dist, dist*dist);  //1.0, d,d^2
    float attenuation = 1.0 / dot(distPoly, light.attenuation);//distpoly vec3 + lightattenuation(kc kl kq)
    vec3 lightDir = (light.position - position) / dist;
    vec3 result = ambient;

    float theta = dot(lightDir, normalize(-light.direction));
    //세타 공식을 찾아야함



    float intensity = clamp(
  (theta - light.cutoff[1]) / (light.cutoff[0] - light.cutoff[1]),
  0.0, 1.0);////clamp a-b로 잡아주는 함수
 
  if (intensity > 0.0) {
        vec3 pixelNorm = normalize(normal);
        float diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3 diffuse = diff * texColor * light.diffuse;

        vec3 specColor = texture2D(material.specular, texCoord).xyz;
        vec3 viewDir = normalize(viewPos - position);
        vec3 reflectDir = reflect(-lightDir, pixelNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * specColor * light.specular;

        result += (diffuse + specular) * intensity;//texture(skybox,R).xyz;
    }

    result *= attenuation;
    fragColor = vec4(texture(skybox,R).xyz, 1.0);
}