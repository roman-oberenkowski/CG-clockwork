#include <glm/glm.hpp>

class Positioner{
    float scale;
    glm::vec3 translate;
    glm::vec3 rotate;
public:
    Positioner(){
        scale=1.0f;
        translate=glm::vec3(0.0f,0.0f,0.0f);
        rotate=glm::vec3(0.0f,0.0f,0.0f);
        
    }
    
};