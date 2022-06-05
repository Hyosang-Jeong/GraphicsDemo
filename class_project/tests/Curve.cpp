//Name: Hyosang Jung, Jaewoo Choi
//Assignment name: Class project
//Course name: CS250
//Term & Year : 2022&Spring
/*!
@file    Curve.cpp
@author  Hyosang Jung, Jaewoo.choi
@date    05/02/2022
Note : This file is for Fifth demo that shows
       2 demo that(Hermite Curve and Catmull Rom Curve).
       All of these demo, you can control vertices by Imgui.
       Also If you clicked Add vertex, you can add vertex. so it add the end point of the line.
       Also it has Imgui implementing forclearing all vertex without default vertex.
       if you click that button you can clear all.
*//*__________________________________________________________________________*/
#include "Curve.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include<vector>
#include"../glhelper.h"
#include<random>
using namespace glm;

void CurveTest::init()
{
    is_hermite = true;
    t_min = 0;
    t_max = 1;
    if (start_point.size() != 0)
    {
        clear_vertices();
    }
    else
    {
        start_point.push_back({ { -0.5,0 },{ -0.7,-0.5} });
        end_point.push_back({ { 0.5, 0 },{ 0.5, -0.5} });
    }
    num_vertices = 10;
    hermite_curve();
    send_data();
    setup_shader("curve");
}

void CurveTest::Update(float )
{
    update_vertice();
}

void CurveTest::Draw()
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Prog.Use();
    glLineWidth(3.f);

    draw_derives();
    draw_curve();

    glLineWidth(1.f);
    glPointSize(1.f);


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnImGuiRender();

}

void CurveTest::UnLoad()
{
}

void CurveTest::OnImGuiRender()
{
    if (ImGui::Button("Hermite Curve") == true)
    {
        is_hermite = true;
        clear_vertices();
        t_min = 0;
        t_max = 1;
    }
    if (ImGui::Button("Catmull Rom Curve") == true)
    {
        is_hermite = false;
        clear_vertices();
        t_min = 0;
        t_max = 1;
    }
    if (is_hermite == false)
    {
        if (ImGui::SliderFloat("t_min", &t_min, -1.f, 0.f))
        {
            catmullRom_curve();
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);


            std::vector<Point> tmp;
            for (int i = 0; i < start_point.size(); i++)
            {
                tmp.push_back(start_point[i]);
                tmp.push_back(end_point[i]);
            }
            glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);
        }
        if (ImGui::SliderFloat("t_max", &t_max, 1.f, 2.f))
        {
            catmullRom_curve();
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);


            std::vector<Point> tmp;
            for (int i = 0; i < start_point.size(); i++)
            {
                tmp.push_back(start_point[i]);
                tmp.push_back(end_point[i]);
            }
            glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);
        }
    }
    if (ImGui::Button("Add Vertex") == true)
    {
        add_vertex();
    }
    if (ImGui::Button("Clear") == true)
    {
        clear_vertices();
        t_min = 0;
        t_max = 1;
    }
    if (ImGui::SliderInt("Vertice Number", &num_vertices, 10, 100))
    {
        if (is_hermite == true)
            hermite_curve();
        else
            catmullRom_curve();
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);


        std::vector<Point> tmp;
        for (int i = 0; i < start_point.size(); i++)
        {
            tmp.push_back(start_point[i]);
            tmp.push_back(end_point[i]);
        }
        glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);
    }
}

void CurveTest::send_data()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size() * 3, &vertices[0].x, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    send_derive_data();
}

void CurveTest::send_derive_data()
{
    std::vector<Point> tmp;
    for (int i = 0; i < start_point.size(); i++)
    {
        tmp.push_back(start_point[i]);
        tmp.push_back(end_point[i]);
    }
    glGenVertexArrays(1, &Derive_VAO);
    glGenBuffers(1, &Derive_VBO);
    glBindVertexArray(Derive_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void CurveTest::setup_shader(std::string shader)
{
    std::string vert = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + shader + ".vert";
    frag = frag + shader + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));

    Prog.CompileLinkValidate(shdr_files);
    if (GL_FALSE == Prog.IsLinked())
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << Prog.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}


void CurveTest::hermite_curve()
{
    vertices.clear();

    float diff = 1.f / static_cast<float>(num_vertices);
    float t = 0;
    for (int num = 0; num < start_point.size(); num++)
    {
        for (int i = 0; i <= num_vertices; i++)
        {
            float start_val = (2 * t * t * t) - (3 * t * t) + 1;
            float start_derive_val = (t * t * t) - (2 * t * t) + t;
            float end_val = (-2 * t * t * t) + (3 * t * t);
            float end_derive_val = (t * t * t) - (t * t);
            vec2 result = (start_val * start_point[num].pos) + (start_derive_val * (start_point[num].tangent - start_point[num].pos)) + (end_val * end_point[num].pos) + (end_derive_val * (end_point[num].tangent - end_point[num].pos));
            vertices.push_back(result);
            t += diff;
        }
        t = 0;
    }

}

void CurveTest::catmullRom_curve()
{
    vertices.clear();
    calculate(vertices);
}

void CurveTest::calculate(std::vector<glm::vec2> vertice)
{
    float diff = (t_max - t_min) / static_cast<float>(num_vertices);
    float t = t_min;
    for (int num = 0; num < start_point.size(); num++)
    {
        vec2 P0 = start_point[num].tangent;
        vec2 P1 = start_point[num].pos;
        vec2 P2 = end_point[num].pos;
        vec2 P3 = end_point[num].tangent;

        for (int i = 0; i <= num_vertices; i++)
        {
            float start_val = (-(t * t * t) / 2.f) + (t * t) - (t / 2.f);
            float start_derive_val = ((t * t * t) * 1.5f) + (-t * t * 2.5f) + 1;
            float end_val = ((-t * t * t) * 1.5f) + (t * t * 2.f) + (t / 2.f);
            float end_derive_val = (t * t * t * 0.5f) - (t * t * 0.5f);
            vec2 result = (start_val * P0) + (start_derive_val * P1) + (end_val * P2) + (end_derive_val * P3);
            vertices.push_back(result);
            t += diff;
        }
        t = t_min;
    }
}
void CurveTest::draw_curve()
{
    glBindVertexArray(VAO);
    glVertexAttrib3f(1, 1.f, 0.0f, 0.f); // red color for points
    glPointSize(10.f);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(vertices.size()));

    glVertexAttrib3f(1, 1.f, 1.0f, 1.f); // white color for line
    glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(vertices.size()));

}

void CurveTest::draw_derives()
{
    glBindVertexArray(Derive_VAO);
    glVertexAttrib3f(1, 1.f, 1.0f, 0.f); // for points
    glPointSize(10.f);
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(start_point.size() * 4));
    glVertexAttrib3f(1, 0.f, 1.0f, 0.f); //for line
    if(is_hermite)
        glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(start_point.size() * 4));
    else
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(start_point.size() * 4));
}

void CurveTest::update_vertice()
{
    double mouse_pos_x;
    double mouse_pos_y;

    //upper left is 0,0
    //bottom right is width, height
    glfwGetCursorPos(GLHelper::ptr_window, &mouse_pos_x, &mouse_pos_y);

    //conver to ndc coord
    mouse_pos_x = ((mouse_pos_x / static_cast<double>(GLHelper::width)) * 2) - 1;
    mouse_pos_y = ((mouse_pos_y / static_cast<double>(GLHelper::height)) * -2) + 1;

    if (GLHelper::mouse_pressed == true)
    {

        for (int i = 0; i < start_point.size(); i++)
        {
            vec2 start_pos = start_point[i].pos;
            vec2  start_derive = start_point[i].tangent;
            vec2 end_pos = end_point[i].pos;
            vec2  end_derive = end_point[i].tangent;
            if (in_mouse(mouse_pos_x, mouse_pos_y, start_pos) == true)
            {
                start_point[i].pos.x = static_cast<float>(mouse_pos_x);
                start_point[i].pos.y = static_cast<float>(mouse_pos_y);
            }

            else if (in_mouse(mouse_pos_x, mouse_pos_y, start_derive) == true)
            {
                start_point[i].tangent.x = static_cast<float>(mouse_pos_x);
                start_point[i].tangent.y = static_cast<float>(mouse_pos_y);
            }

            else  if (in_mouse(mouse_pos_x, mouse_pos_y, end_pos) == true)
            {
                end_point[i].pos.x = static_cast<float>(mouse_pos_x);
                end_point[i].pos.y = static_cast<float>(mouse_pos_y);
            }

            else if (in_mouse(mouse_pos_x, mouse_pos_y, end_derive) == true)
            {
                end_point[i].tangent.x = static_cast<float>(mouse_pos_x);
                end_point[i].tangent.y = static_cast<float>(mouse_pos_y);
            }
        }
    }
    if (is_hermite == true)
        hermite_curve();
    else
        catmullRom_curve();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size() * 3, &vertices[0].x, GL_STATIC_DRAW);


    std::vector<Point> tmp;
    for (int i = 0; i < start_point.size(); i++)
    {
        tmp.push_back(start_point[i]);
        tmp.push_back(end_point[i]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);
}

bool CurveTest::in_mouse(double mouse_pos_x, double mouse_pos_y, glm::vec2 pos)
{
    float offset = 0.07f;
    if (mouse_pos_x >= static_cast<double>(pos.x) - static_cast<double>(offset) && mouse_pos_x <= static_cast<double>(pos.x) + static_cast<double>(offset)
        && mouse_pos_y >= static_cast<double>(pos.y) - static_cast<double>(offset) && mouse_pos_y <= static_cast<double>(pos.y) + static_cast<double>(offset))
    {
        return true;
    }
    return false;
}

void CurveTest::add_vertex()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distrFloat(-0.5, 0.5);


    if (is_hermite == true)
    {
        start_point.push_back(*(end_point.end() - 1));
        end_point.push_back({ { distrFloat(gen), distrFloat(gen) },{ distrFloat(gen), distrFloat(gen)} });
        hermite_curve();
    }
    else
    {
        Point start_tmp = *(start_point.end() - 1);
        Point end_tmp = *(end_point.end() - 1);

        start_point.push_back(Point{end_tmp.pos,start_tmp.pos});
        end_point.push_back(Point{ end_tmp.tangent,{ distrFloat(gen), distrFloat(gen)} });
        catmullRom_curve();
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size() * 3, &vertices[0].x, GL_STATIC_DRAW);


    std::vector<Point> tmp;
    for (int i = 0; i < start_point.size(); i++)
    {
        tmp.push_back(start_point[i]);
        tmp.push_back(end_point[i]);
    }

    glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);
}

void CurveTest::clear_vertices()
{
    start_point.clear();
    end_point.clear();

    start_point.push_back({ { -0.5,0 },{ -0.7,-0.5} });
    end_point.push_back({ { 0.5, 0 },{ 0.5, -0.5} });
    if (is_hermite == true)
        hermite_curve();
    else
        catmullRom_curve();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size() * 3, &vertices[0].x, GL_STATIC_DRAW);


    std::vector<Point> tmp;
    for (int i = 0; i < start_point.size(); i++)
    {
        tmp.push_back(start_point[i]);
        tmp.push_back(end_point[i]);
    }
    glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * tmp.size() * 2, &tmp[0].pos.x, GL_STATIC_DRAW);
}