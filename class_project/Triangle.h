#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
class Triangle 
{
public:
    void init();
    void init_shader();
    void draw();

   GLuint vertex_shd;
   GLuint fragment_shd;
    GLuint pgm;
    GLuint VBO;
    GLuint VAO;
};