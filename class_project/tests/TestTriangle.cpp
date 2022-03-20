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


    glUniformMatrix4fv(triangle.mvpMatLoc, 1, GL_FALSE, &triangle.SRT_mat[0].x);

    glUniformMatrix4fv(sphere.mvpMatLoc, 1, GL_FALSE, &sphere.SRT_mat[0].x);

    /*  Tell shader to use obj's VAO for rendering */

    glBindVertexArray(triangle.VAO);
    glDrawElements(GL_TRIANGLES, triangle.numIndices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(sphere.VAO);
    glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, nullptr);
}

void TriangleTest::OnImGuiRender()
{
}
