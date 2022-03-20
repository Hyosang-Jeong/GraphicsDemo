#include "TestTriangle.h"

TriangleTest::TriangleTest()
{
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{

	triangle = mesh[CUBE];
	sphere = mesh[SPHERE];
	torus = mesh[TORUS];
	cylinder = mesh[CYLINDER];
	cone = mesh[CONE];//There is no cap in bottom of the cone.

	triangle.setup_shdrpgm();
	sphere.setup_shdrpgm();
	torus.setup_shdrpgm();
	cylinder.setup_shdrpgm();
	cone.setup_shdrpgm();

	ComputeViewProjMats();

	triangle.setup_mesh();
	sphere.setup_mesh();
	torus.setup_mesh();
	cylinder.setup_mesh();
	cone.setup_mesh();

}

void TriangleTest::Update([[maybe_unused]]float deltaTime)
{
	//triangle.update(deltaTime);
}

void TriangleTest::Draw()
{
 //   glClearBufferfv(GL_DEPTH, 0, &one);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ComputeViewProjMats();

    /*  Send each part's data to shaders for rendering */
    glUniform4fv(triangle.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(sphere.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(torus.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cylinder.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cone.colorLoc, 1, ValuePtr(useNormal));

    glm::mat4  mat =
    {
        cos(PI/4.f),0,-sin(PI / 4.f),0,
        0,1,0,0,
       sin(PI / 4.f),0,cos(PI / 4.f),0,
        0,0,0,1
    };

    glm::mat4  y_mat =
    {
        cos(PI / 4.f),0,sin(PI / 4.f),0,
        0,1,0,0,
      - sin(PI / 4.f),0,cos(PI / 4.f),0,
        0,0,0,1
    };

    glm::mat4 z_mat =
    {
      cos(PI / 6.f), -sin(PI / 6.f), 0 ,0,
    sin(PI / 6.f), cos(PI / 6.f), 0, 0,
    0, 0 ,1, 0,
    0 ,0 ,0 ,1
    };
    mat *= z_mat;

    glUniformMatrix4fv(triangle.mvpMatLoc, 1, GL_FALSE, &mat[0].x);
    glUniformMatrix4fv(sphere.mvpMatLoc, 1, GL_FALSE, &mat[0].x);
    glUniformMatrix4fv(torus.mvpMatLoc, 1, GL_FALSE, &mat[0].x);
    glUniformMatrix4fv(cylinder.mvpMatLoc, 1, GL_FALSE, &mat[0].x);
    glUniformMatrix4fv(cone.mvpMatLoc, 1, GL_FALSE, &mat[0].x);

    /*  Tell shader to use obj's VAO for rendering */
    
    glBindVertexArray(triangle.VAO);
    //glDrawElements(GL_TRIANGLES, triangle.numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(sphere.VAO);
    //glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(torus.VAO);
    //glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(cylinder.VAO);
    //glDrawElements(GL_TRIANGLES, cylinder.numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(cone.VAO);
    glDrawElements(GL_TRIANGLES, cone.numIndices, GL_UNSIGNED_INT, nullptr);
}

void TriangleTest::OnImGuiRender()
{
}
