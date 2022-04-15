#include "Curve.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include<vector>
#include"../glhelper.h"
using namespace glm;

CurveTest::CurveTest()
{
}

CurveTest::~CurveTest()
{
}

void CurveTest::init()
{
   start = { -0.5, 0 };
   end = { 0.5, 0 };
   derive_start={ -0.7,- 0.5 };
   derive_end = { 0.5, -0.5 };

    compute_vertices(10, start, end, derive_start, derive_end);
    send_data();
    setup_shader("curve");

}

void CurveTest::Update(float deltaTime)
{
    update_vertice();
}

void CurveTest::Draw()
{
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT );
    Prog.Use();
    glLineWidth(3.f);

    draw_derives();
    draw_curve();



    glLineWidth(1.f);
    glPointSize(1.f);
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void CurveTest::UnLoad()
{
}

void CurveTest::send_data()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vec2 derives[] = {
        derive_start,start,
        derive_end,end
    };
    glGenVertexArrays(1, &Derive_VAO);
    glGenBuffers(1, &Derive_VBO);
    glBindVertexArray(Derive_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) *4, &derives[0].x, GL_STATIC_DRAW);
    // position attribute
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


void CurveTest::compute_vertices(int num_vertices, glm::vec2 start, glm::vec2 end, glm::vec2 derive_start, glm::vec2 derive_end)
{
    vertices.clear();


    float diff =1.f/ static_cast<float>(num_vertices);
    float t = 0;
    for (int i = 0; i <=num_vertices; i++)
    {
        float start_val = (2 * t * t * t) - (3 * t * t) + 1;
        float start_derive_val = (t * t * t) - (2 * t * t) + t;
        float end_val = (-2 * t * t * t) + (3 * t * t);
        float end_derive_val = (t * t * t) - (t * t);
        vec2 result = (start_val * start) + (start_derive_val * (derive_start-start)) + (end_val * end) + (end_derive_val * (derive_end-end));
        vertices.push_back(result);
        t += diff;
    }

}

void CurveTest::draw_curve()
{
    glBindVertexArray(VAO);
    glVertexAttrib3f(1, 1.f, 1.0f, 1.f); // white color for line
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glVertexAttrib3f(1, 1.f, 0.0f, 0.f); // red color for points
    glPointSize(10.f);
    glDrawArrays(GL_POINTS, 0, vertices.size());
}

void CurveTest::draw_derives()
{
    glBindVertexArray(Derive_VAO);
    glVertexAttrib3f(1, 0.f, 1.0f, 0.f); //for line
    glDrawArrays(GL_LINES, 0, 4);
    glVertexAttrib3f(1, 1.f, 1.0f, 0.f); // for points
    glPointSize(10.f);
    glDrawArrays(GL_POINTS, 0, 4);
}

void CurveTest::update_vertice()
{
    double mouse_pos_x;
    double mouse_pos_y;

    //upper left is 0,0
    //bottom right is width, height
    glfwGetCursorPos(GLHelper::ptr_window, &mouse_pos_x, &mouse_pos_y);

    //conver to ndc coord
    mouse_pos_x = ((mouse_pos_x / static_cast<double>(GLHelper::width))   *   2)  -  1;
    mouse_pos_y = ((mouse_pos_y / static_cast<double>(GLHelper::height))  *  -2) + 1;

    float offset = 0.01f;

    if (GLHelper::mouse_pressed == true)
    {
        if (mouse_pos_x >= start.x - offset && mouse_pos_x <= start.x + offset && 
            mouse_pos_y >= start.y - offset && mouse_pos_y <= start.y + offset)
        {
            if((is_pressing[1]||is_pressing[2]||is_pressing[3]) == 0)
            is_pressing[0] = true;
        }
        
        else if (mouse_pos_x >= end.x - offset && mouse_pos_x <= end.x + offset && 
            mouse_pos_y >= end.y - offset && mouse_pos_y <= end.y + offset)
        {
            if ((is_pressing[0] || is_pressing[2] || is_pressing[3]) == 0)
            is_pressing[1] = true;

        }

        else if (mouse_pos_x >= (derive_start.x - offset) && mouse_pos_x <=( derive_start.x + offset) && 
            mouse_pos_y >= (derive_start.y - offset) && mouse_pos_y <= (derive_start.y + offset))
        {
            if ((is_pressing[1] || is_pressing[0] || is_pressing[3]) == 0)
            is_pressing[2] = true;

        }

        else  if (mouse_pos_x >= derive_end.x - offset && mouse_pos_x <= derive_end.x + offset 
            && mouse_pos_y >= derive_end.y - offset && mouse_pos_y <= derive_end.y + offset)
        {
            if ((is_pressing[1] || is_pressing[2] || is_pressing[0]) == 0)
            is_pressing[3] = true;
        }

    }
    else
    {
        is_pressing[0] = false;
        is_pressing[1] = false;
        is_pressing[2] = false;
        is_pressing[3] = false;
    }

    if (is_pressing[0] == true)
    {
        start.x = mouse_pos_x;
        start.y = mouse_pos_y;
    }
    if (is_pressing[1] == true)
    {
        end.x = mouse_pos_x;
        end.y = mouse_pos_y;
    }
    if (is_pressing[2] == true)
    {
        derive_start.x = mouse_pos_x;
        derive_start.y = mouse_pos_y;
    }
    if (is_pressing[3] == true)
    {
        derive_end.x = mouse_pos_x;
        derive_end.y = mouse_pos_y;
    }

    compute_vertices(10, start, end, derive_start, derive_end);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), &vertices[0].x, GL_STATIC_DRAW);

    vec2 derives[] = 
    {
    derive_start,start,
     derive_end,end
    };

    glBindBuffer(GL_ARRAY_BUFFER, Derive_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 4, &derives[0].x, GL_STATIC_DRAW);
}
