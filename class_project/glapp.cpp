/*!
@file    glapp.cpp
@author  pghali@digipen.edu, hyosang jung
@date    10/11/2016

This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and to extract function pointers
to OpenGL implementations.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include "glapp.h"
#include "glhelper.h"
#include<array> // for array
#include<cstdlib> // for random number
/*                                                   objects with file scope
----------------------------------------------------------------------------- */
static GLApp    g_glapp;
GLApp::GLModel GLApp::mdl;

void GLApp::init()  // add opengl info code by hyosang jung
{

    // Part 1: clear colorbuffer with RGBA value in glClearColor ...
    glClearColor(1.f, 0.f, 0.f, 1.f);
    // Part 2: use the entire window as viewport ...
    GLint w = GLHelper::width, h = GLHelper::height;
    glViewport(0, 0, w, h);
    // Part 3: initialize VAO and create shader program
    mdl.setup_vao();
    mdl.setup_shdrpgm();


    std::cout << "GPU Vendor: " << glGetString(GL_VENDOR) << '\n';
    std::cout << "GL Renderer:  " << glGetString(GL_RENDERER) << '\n';
    std::cout << "GL Version:  " << glGetString(GL_VERSION) << '\n';
    std::cout << "GL Shader Version:  " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

    GLint major_version;
    GLint minor_version;
    GLint double_buffer;
    GLint max_vertex_count;
    GLint max_indices_count;
    GLint tex_size;
    GLint viewport_dim[2];


    glGetIntegerv(GL_MAJOR_VERSION, &major_version);
    glGetIntegerv(GL_MINOR_VERSION, &minor_version);
    glGetIntegerv(GL_DOUBLEBUFFER, &double_buffer);
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &max_vertex_count);
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &max_indices_count);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &tex_size);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, viewport_dim);

    std::cout << "GL Major Version:  " << major_version << '\n';
    std::cout << "GL Minor Version:  " << minor_version << '\n';

    if (double_buffer == 1)
    {
        std::cout << "Current OpenGL Context is double buffered" << '\n';
    }
    else
    {
        std::cout << "Current OpenGL Context is not  double buffered" << '\n';
    }
    std::cout << "Maximum Vertex Count: " << max_vertex_count << '\n';
    std::cout << "Maximum Indices Count: " << max_indices_count << '\n';
    std::cout << "Maximum texture size: " << tex_size << '\n';
    std::cout << "Maximum Viewport Dimensions: " << viewport_dim[0] << " x " << viewport_dim[1] << '\n';

}

void GLApp::update(double delta_time) 
{
    glClearColor(0, 0,0, 1);
}

void GLApp::draw()
{
    // clear back buffer as before
    glClear(GL_COLOR_BUFFER_BIT);
    // now, render rectangular model from NDC coordinates to viewport
    mdl.draw();
}

void GLApp::GLModel::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    // there are many shader programs initialized - here we're saying
     // which specific shader program should be used to render geometry
    shdr_pgm.Use();
<<<<<<< Updated upstream
=======
    

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Display FPS in another viewport
    ImGui::Begin("Triangle Position/Color");
    static float rotation = 0.0;
    ImGui::SliderFloat("rotation", &rotation, 0, 2 * 3.141592f);
    static float translation[] = { 0.0, 0.0 };
    ImGui::SliderFloat2("position", translation, -1.0, 1.0);
    static float color[4] = { 1.0f,1.0f,1.0f,1.0f }; 

    // pass the parameters to the shader
    
    shdr_pgm.SetUniform("rotation", rotation);
    shdr_pgm.SetUniform("translation", translation[0], translation[1]);
    // color picker
    ImGui::ColorEdit3("color", color);
    // multiply triangle's color with this color
    shdr_pgm.SetUniform("color", color[0], color[1], color[2]);
    ImGui::End();

    //ImGui::Begin("Two");
    //ImGui::Text("Application average %0.3f ms/frame (%.01f FPS)");
    //ImGui::End();
    // 
>>>>>>> Stashed changes
    // there are many models, each with their own initialized VAO object
    // here, we're saying which VAO's state should be used to set up pipe
    glBindVertexArray(vaoid);
    // here, we're saying what primitive is to be rendered and how many
     // such primitives exist.
     // the graphics driver knows where to get the indices because the VAO
     // containing this state information has been made current ...
    glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);
    // after completing the rendering, we tell the driver that VAO
    // vaoid and current shader program are no longer current
    glBindVertexArray(0);
    shdr_pgm.UnUse();
}

void GLApp::cleanup() {
    // empty for now
}

void GLApp::GLModel::setup_vao()
{
    std::array<glm::vec2, 3> pos_vtx
    {
    glm::vec2(-0.5f, -0.5f), glm::vec2(0.5f, -0.5f),
    glm::vec2(0.f, 0.5f)
    };
    std::array<glm::vec3, 3> clr_vtx{
    glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3(0.f, 0.f, 1.f)
    };

    GLuint vbo_hdl;
    glCreateBuffers(1, &vbo_hdl);

    glNamedBufferStorage(vbo_hdl,
        sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
        nullptr, GL_DYNAMIC_STORAGE_BIT);

    glNamedBufferSubData(vbo_hdl, 0,
        sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());

    glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(),
        sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());
    ;

    glCreateVertexArrays(1, &vaoid);
    glEnableVertexArrayAttrib(vaoid, 0);
    glVertexArrayVertexBuffer(vaoid, 0, vbo_hdl, 0, sizeof(glm::vec2));
    glVertexArrayAttribFormat(vaoid, 0, 2, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 0, 0);
    glEnableVertexArrayAttrib(vaoid, 1);
    glVertexArrayVertexBuffer(vaoid, 1, vbo_hdl,
        sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
    glVertexArrayAttribFormat(vaoid, 1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(vaoid, 1, 1);

    primitive_type = GL_TRIANGLES;

    std::array<GLushort, 3> idx_vtx
    {
    0, 1, 2
    };

    idx_elem_cnt = static_cast<GLuint>(idx_vtx.size());

    GLuint ebo_hdl;
    glCreateBuffers(1, &ebo_hdl);
    glNamedBufferStorage(ebo_hdl,
        sizeof(GLushort) * idx_elem_cnt,
        reinterpret_cast<GLvoid*>(idx_vtx.data()),
        GL_DYNAMIC_STORAGE_BIT);

    glVertexArrayElementBuffer(vaoid, ebo_hdl);

    glBindVertexArray(0);

}


void GLApp::GLModel::setup_shdrpgm()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(
        GL_VERTEX_SHADER,
        "../shaders/triangle.vert"));
    shdr_files.push_back(std::make_pair(
        GL_FRAGMENT_SHADER,
        "../shaders/triangle.frag"));
    shdr_pgm.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << shdr_pgm.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

