#include "TestTriangle.h"
#include"../glhelper.h"
TriangleTest::TriangleTest()
{
}

TriangleTest::~TriangleTest()
{
}

void TriangleTest::init()
{
    GLint wid = GLHelper::width;
    GLint hei = GLHelper::height;

	cube = mesh[CUBE];
	sphere = mesh[SPHERE];
	torus = mesh[TORUS];
	cylinder = mesh[CYLINDER];
	cone = mesh[CONE];//There is no cap in bottom of the cone.

    cube.init({ wid/6, hei * (3.f/4.f) , 0 }, { 0.5,0.5,1 }, { 0,0,QUARTER_PI });

    sphere.init({ wid / 2, hei * (3.f / 4.f) , 0 }, { 0.5,0.5,1 }, { 0,0,0 });

    torus.init({ wid * (5.f/6.f), hei * (3.f / 4.f) , 0 }, { 0.5,0.5,1 }, { 0,0,0 });

    cylinder.init({ wid / 6, hei * (1.f / 4.f) , 0 }, { 0.5,0.5,1 }, { 0,0,0 });

    cone.init({ wid / 2, hei * (1.f / 4.f) , 0 }, { 0.5,0.5,1 }, { 0,0,0 });
}

void TriangleTest::Update(float deltaTime)
{
    cube.rotation.x += deltaTime;
    cube.rotation.y += deltaTime;
    cube.rotation.z += deltaTime;

    cube.compute_matrix(deltaTime);
    sphere.compute_matrix(deltaTime);
    torus.compute_matrix(deltaTime);
    cylinder.compute_matrix(deltaTime);
    cone.compute_matrix(deltaTime);
}

void TriangleTest::Draw()
{
    //   glClearBufferfv(GL_DEPTH, 0, &one);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*  Send each part's data to shaders for rendering */
    glUniform4fv(cube.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(sphere.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(torus.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cylinder.colorLoc, 1, ValuePtr(useNormal));
    glUniform4fv(cone.colorLoc, 1, ValuePtr(useNormal));

    glUniformMatrix4fv(cube.mvpMatLoc, 1, GL_FALSE, &cube.SRT_mat[0].x);
    glBindVertexArray(cube.VAO);
    glDrawElements(GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_INT, nullptr);
  
    glUniformMatrix4fv(sphere.mvpMatLoc, 1, GL_FALSE, &sphere.SRT_mat[0].x);
    glBindVertexArray(sphere.VAO);
    glDrawElements(GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(torus.mvpMatLoc, 1, GL_FALSE, &torus.SRT_mat[0].x);
    glBindVertexArray(torus.VAO);
    glDrawElements(GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(cylinder.mvpMatLoc, 1, GL_FALSE, &cylinder.SRT_mat[0].x);
    glBindVertexArray(cylinder.VAO);
    glDrawElements(GL_TRIANGLES, cylinder.numIndices, GL_UNSIGNED_INT, nullptr);

    glUniformMatrix4fv(cone.mvpMatLoc, 1, GL_FALSE, &cone.SRT_mat[0].x);
    glBindVertexArray(cone.VAO);
    glDrawElements(GL_TRIANGLES, cone.numIndices, GL_UNSIGNED_INT, nullptr);

}

void TriangleTest::OnImGuiRender()
{
}
