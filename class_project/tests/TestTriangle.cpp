#include "TestTriangle.h"

TriangleTest::TriangleTest()
{
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{
	triangle = CreateCube(1, 1);
	sphere = CreateSphere(16, 16);

	triangle.setup_shdrpgm();
	sphere.setup_shdrpgm();

	ComputeViewProjMats();

	triangle.setup_mesh();
	sphere.setup_mesh();

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

    /*  Tell shader to use obj's VAO for rendering */

    glBindVertexArray(triangle.VAO);
    glBindVertexArray(sphere.VAO);

    glDrawElements(GL_TRIANGLES, triangle.numIndices, GL_UNSIGNED_INT, nullptr);
    //glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, nullptr);
}

void TriangleTest::OnImGuiRender()
{
}
