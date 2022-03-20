/******************************************************************************/
/*!
\file   mesh.hpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    Email: sang.le\@digipen.edu
\par    DigiPen login: sang.le
\date   8/1/2015
*/
/******************************************************************************/

#include <vector>
#include <GL/glew.h> 
#include"math.h"
#include"glslshader.h"
struct Vertex
{
    Vertex(glm::vec3 pos, glm::vec3 nrm, glm::vec3 uv) : pos(pos), nrm(nrm), uv(uv) {}

    Vertex() { }

    glm::vec3 pos, nrm;
    glm::vec2 uv;
};

const int vertexSize = sizeof(Vertex);
const int indexSize = sizeof(int);


/*  The layouts for specifying the offsets of a vertex
    when it is copied into the graphics pipeline. */
struct VertexLayout
{
    int location;
    int size;
    int type;
    bool normalized;
    int offset;
};

const VertexLayout vLayout[] =
{
    { 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, pos) },
    { 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, nrm) },
    { 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) }
};

const int layoutSize = sizeof(VertexLayout);
const int numAttribs = sizeof(vLayout) / layoutSize;    // for now numAttribs = 1: only pos


typedef std::vector<Vertex> VertexBufferType;
typedef std::vector<int> IndexBufferType;

/*  Mesh format */
struct Mesh
{
    Mesh() : numVertices(0), numTris(0), numIndices(0), position{ 0 }, scale{ 1 }, rotation{ 0 } { }

    /*  Storing the actual vertex/index data */
    VertexBufferType vertexBuffer;
    IndexBufferType indexBuffer;

    int numVertices;
    int numTris;
    int numIndices;

    GLuint VAO;
    GLuint VBO;
    GLuint IBO;
    GLSLShader renderProg;

    GLint mvpMatLoc;
    GLint colorLoc;
    GLint   textureLoc;

    glm::mat4 World_to_NDC;
    glm::mat4 SRT_mat;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;

    void init(glm::vec3 Pos = {0,0,0}, glm::vec3 Scale = { 1,1,1 }, glm::vec3 Rotate = { 0,0,0 });
    void SendVertexData();
    void setup_shdrpgm();
    void setup_mesh();
    void compute_matrix(float delta_time);
    void draw();
    void set_position(glm::vec3 pos) {
        position = pos;
    }
};


/*  Mesh function(s) */
Mesh CreatePlane(int stacks, int slices);
Mesh CreateCube(int stacks, int slices);
Mesh CreateSphere(int stacks, int slices);
Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle);
Mesh CreateCylinder(int stacks, int slices);
Mesh CreateCone(int stacks, int slices);



/******************************************************************************/
/*  Pre-defined shapes                                                        */
/******************************************************************************/
enum MeshID { PLANE, CUBE,SPHERE,TORUS,CYLINDER,CONE, NUM_MESHES };

static Mesh mesh[NUM_MESHES] = { CreatePlane(1, 1),
                                 CreateCube(1, 1),
                                 CreateSphere(16, 16),
                                 CreateTorus(16, 32, 0,TWO_PI),
                                 CreateCylinder(1,8),
                                 CreateCone(16,8)
};
