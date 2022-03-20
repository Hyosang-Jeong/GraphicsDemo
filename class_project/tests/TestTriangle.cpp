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

    triangle.init();
    triangle.set_position({ 900, 450, 0});
    sphere.init();
}

void TriangleTest::Update([[maybe_unused]]float deltaTime)
{
    triangle.compute_matrix(deltaTime);
    sphere.compute_matrix(deltaTime);
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


    glUniformMatrix4fv(triangle.mvpMatLoc, 1, GL_FALSE, &triangle.SRT_mat[0].x);

    glUniformMatrix4fv(sphere.mvpMatLoc, 1, GL_FALSE, &sphere.SRT_mat[0].x);

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
