/******************************************************************************/
/*!
\file   mesh.cpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le, 
\par    Email: sang.le\@digipen.edu,
\par    DigiPen login: sang.le, 
\date   YOUR_COMPLETION_DATE
*/
/******************************************************************************/

#include "Mesh.h"
#include "glhelper.h"
/*  Function prototype(s) */


Mesh CreatePlane(int stacks, int slices)
{
    Mesh mesh;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;

            v.pos = glm::vec3(col - 0.5f, row - 0.5f , 0.0f);
            v.nrm = glm::vec3(0.0f, 0.0f, 1.0f);
            v.uv = glm::vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

Mesh CreateCube(int stacks, int slices)
{
    Mesh planeMesh = CreatePlane(stacks, slices);
    Mesh mesh;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;
    Vec3 const translateArray[] =
    {
        Vec3(+0.0f, +0.0f, +0.5f), // Z+
        Vec3(+0.0f, +0.0f, -0.5f), // Z-
        Vec3(+0.5f, +0.0f, +0.0f), // X+
        Vec3(-0.5f, +0.0f, +0.0f), // X-
        Vec3(+0.0f, +0.5f, +0.0f), // Y+
        Vec3(+0.0f, -0.5f, +0.0f), // Y-
    };

    Vec2 const rotateArray[] =
    {
        Vec2(+0.0f, +0.0f),             // Z+
        Vec2(+0.0f, (float)+PI),        // Z-
        Vec2(+0.0f, (float)+HALF_PI),   // X+       
        Vec2(+0.0f, (float)-HALF_PI),   // X-
        Vec2((float)-HALF_PI, +0.0f),   // Y+
        Vec2((float)+HALF_PI, +0.0f)    // Y-
    };


    /*  Transform the plane to 6 positions to form the faces of the cube */
    for (int i = 0; i < 6; ++i)
    {
        Mat4 transformMat = Translate(translateArray[i]) * Rotate(rotateArray[i][YINDEX], { 0,1,0 }) *  Rotate(rotateArray[i][XINDEX], { 1,0,0 });

        for (int j = 0; j < planeMesh.numVertices; ++j)
        {
            Vertex v;
            v.pos = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].pos, 1.0));
            v.nrm = Vec3(transformMat * Vec4(planeMesh.vertexBuffer[j].nrm, 1.0));
            v.uv = planeMesh.vertexBuffer[j].uv;

            v.pos = RoundDecimal(v.pos);
            v.nrm = RoundDecimal(v.nrm);

            addVertex(mesh, v);
        }

        for (int j = 0; j < planeMesh.numIndices; ++j)
            addIndex(mesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
    }

    return mesh;
}

Mesh CreateSphere(int stacks, int slices)
{
    Mesh mesh;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;
        float beta = PI * (row - 0.5);

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;
            float alpha = col * PI * 2.0;
            Vertex v;
            v.uv.x = col;
            v.uv.y = row * (-1.0);

            v.pos.x = 0.5f * sin(alpha) * cos(beta);
            v.pos.y = 0.5f * sin(beta);
            v.pos.z = 0.5f * cos(alpha) * cos(beta);

            v.nrm.x = v.pos.x;
            v.nrm.y = v.pos.y;
            v.nrm.z = v.pos.z;

            v.nrm /= 0.5;

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}

Mesh CreateCylinder(int stacks, int slices)
{
    Mesh mesh;
    Vertex vertex;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;
    /// For the body

    for (int i = 0; i <= stacks; i++)
    {
        float row = static_cast<float>(i) / stacks;

        for (int j = 0; j <= slices; j++)
        {
            float col = static_cast<float>(j) / slices;
            float alpha = col * 2.0 * PI;
            float sinAlpha = sin(alpha);
            float cosAlpha = cos(alpha);

            vertex.uv.x = row;
            vertex.uv.y = col;

            vertex.pos.x = 0.5 * sinAlpha;
            vertex.pos.y = row - 0.5;
            vertex.pos.z = 0.5 * cosAlpha;

            vertex.nrm.x = vertex.pos.x / 0.5;
            vertex.nrm.y = 0;
            vertex.nrm.z = vertex.pos.z / 0.5;

            addVertex(mesh, vertex);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);

    int vertex_size = mesh.vertexBuffer.size();

    /// For the caps

    for (int i = 0; i <= stacks; i++)
    {
        if (i == 0)
        {
            vertex.pos = Vec3(0.0, 0.5, 0.0);
            vertex.nrm.x = vertex.pos.x / 0.5;
            vertex.nrm.y = vertex.pos.y / 0.5;
            vertex.nrm.z = vertex.pos.z / 0.5;
        }
        else if (i == 1)
        {
            vertex.pos = Vec3(0.0, -0.5, 0.0);
            vertex.nrm.x = vertex.pos.x / 0.5;
            vertex.nrm.y = vertex.pos.y / 0.5;
            vertex.nrm.z = vertex.pos.z / 0.5;
        }

        addVertex(mesh, vertex);

        float row = static_cast<float>(i) / stacks;

        for (int j = 0; j <= slices; j++)
        {
            float col = static_cast<float>(j) / slices;
            float alpha = col * 2.0 * PI;
            float sinAlpha = sin(alpha);
            float cosAlpha = cos(alpha);

            vertex.uv.x = row;
            vertex.uv.y = col;

            if (i == 0)
            {
                vertex.pos = Vec3(0.5 * sinAlpha, 0.5, 0.5 * cosAlpha);

                vertex.nrm.x = vertex.pos.x / 0.5;
                vertex.nrm.y = vertex.pos.y / 0.5;
                vertex.nrm.z = vertex.pos.z / 0.5;
            }
            else if (i == 1)
            {
                vertex.pos = Vec3(0.5 * sinAlpha, -0.5, 0.5 * cosAlpha);

                vertex.nrm.x = vertex.pos.x / 0.5;
                vertex.nrm.y = vertex.pos.y / 0.5;
                vertex.nrm.z = vertex.pos.z / 0.5;
            }

            addVertex(mesh, vertex);
        }
    }

    for (int i = 0; i <= stacks; i++)
    {
        vertex_size += (i * (slices + 2));

        for (int j = 1; j < slices; j++)
        {
            addIndex(mesh, vertex_size);
            addIndex(mesh, vertex_size + j);
            addIndex(mesh, vertex_size + j + 1);
        }
        addIndex(mesh, vertex_size);
        addIndex(mesh, vertex_size + slices);
        addIndex(mesh, vertex_size + 1);
    }

    return mesh;
}

Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle)
{
    Mesh mesh;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks; // [0.0, 1.0]

        float alpha = PI + startAngle + (row * (endAngle - startAngle)); // [0.0, 2¥ð]

        float sinAlpha = sin(alpha);
        float  cosAlpha = cos(alpha);

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            float beta = col * PI * 2.0;

            Vertex center;
            center.pos = Vec3(0, 0, 0);

            Vertex v;
            v.uv.x = col;
            v.uv.y = row;

            v.pos.x = -(0.35 + (0.15 * cos(beta))) * sinAlpha;
            v.pos.y = -0.15 * sin(beta);
            v.pos.z = -(0.35 + (0.15 * cos(beta))) * cosAlpha;

            v.nrm = v.pos - center.pos;

            v.nrm /= 0.15;
            addVertex(mesh, v);
        }

    }

    BuildIndexBuffer(stacks, slices, mesh);
    return mesh;
}

Mesh CreateCone(int stacks, int slices)
{
    Mesh mesh;
    Vertex v;
    Mesh top;
    Mesh bottom;
    mesh.stack_slice[0] = stacks;
    mesh.stack_slice[1] = slices;
    float col = 0;
    float alpha = 0;
    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = ((float)stack / stacks) - 0.5; // [0.0, 1.0]
        float z_val = 0.5;
        for (int slice = 0; slice <= slices; ++slice)
        {
            col = (float)slice / slices;
            alpha = col * PI * 2.0;

            // side
            v.uv.x = row;
            v.uv.y = col;

            v.pos.x = 0.5 * (0.5 - row) * sin(alpha);
            v.pos.y = row;
            v.pos.z = 0.5 * (0.5 - row) * cos(alpha);


            v.nrm.x = v.pos.x / 0.5;
            v.nrm.y = 0.0;
            v.nrm.z = v.pos.z / 0.5;

            addVertex(mesh, v);
        }
    }
    BuildIndexBuffer(stacks, slices, mesh);

    int vertex_size = mesh.vertexBuffer.size();

    /// For the caps

    for (int i = 0; i <1; i++)
    {
        if (i == 0)
        {
            v.pos = Vec3(0.0, -0.5, 0.0);
            v.nrm.x = v.pos.x / 0.5;
            v.nrm.y = v.pos.y / 0.5;
            v.nrm.z = v.pos.z / 0.5;
        }

        addVertex(mesh, v);

        float row = static_cast<float>(i) / stacks;

        for (int j = 0; j <= slices; j++)
        {
            float col = static_cast<float>(j) / slices;
            float alpha = col * 2.0 * PI;
            float sinAlpha = sin(alpha);
            float cosAlpha = cos(alpha);

            v.uv.x = row;
            v.uv.y = col;

            v.pos = Vec3(0.5 * sinAlpha, -0.5, 0.5 * cosAlpha);

            v.nrm.x = v.pos.x / 0.5;
            v.nrm.y = v.pos.y / 0.5;
            v.nrm.z = v.pos.z / 0.5;
            

            addVertex(mesh, v);
        }
    }

    for (int i = 0; i <= stacks; i++)
    {
        vertex_size += (i * (slices + 2));

        for (int j = 1; j < slices; j++)
        {
            addIndex(mesh, vertex_size);
            addIndex(mesh, vertex_size + j + 1);
            addIndex(mesh, vertex_size + j);
        }
        addIndex(mesh, vertex_size);
        addIndex(mesh, vertex_size + 1);
        addIndex(mesh, vertex_size + slices);
    }


    return  mesh;
}

void BuildIndexBuffer(int stacks, int slices, Mesh& mesh)
{
    //@todo: IMPLEMENT ME
    int p0 = 0, p1 = 0, p2 = 0;
    int p3 = 0, p4 = 0, p5 = 0;

    int stride = slices + 1;
    for (int i = 0; i < stacks; ++i)
    {
        for (int j = 0; j < slices; ++j)
        {
            int current_row = i * stride;
            /*  You need to compute the indices for the first triangle here */
            /*  ... */
            p0 = current_row + j;
            p1 = p0 + 1;
            p2 = p1 + stride;
            // triangle <p3, p4, p5>
            /*  Ignore degenerate triangle */
            if (!DegenerateTri(mesh.vertexBuffer[p0].pos,
                mesh.vertexBuffer[p1].pos,
                mesh.vertexBuffer[p2].pos))
            {
                /*  Add the indices for the first triangle */
                addIndex(mesh, p0);
                addIndex(mesh, p1);
                addIndex(mesh, p2);
            }


            /*  You need to compute the indices for the second triangle here */
            /*  ... */
            p3 = p2;
            p4 = p3 - 1;
            p5 = p0;
            /*  Ignore degenerate triangle */
            if (!DegenerateTri(mesh.vertexBuffer[p3].pos,
                mesh.vertexBuffer[p4].pos,
                mesh.vertexBuffer[p5].pos))
            {
                /*  Add the indices for the second triangle */
                addIndex(mesh, p3);
                addIndex(mesh, p4);
                addIndex(mesh, p5);
            }
        }
    }
}

void addVertex(Mesh& mesh, const Vertex& v)
{
    mesh.vertexBuffer.push_back(v);
    ++mesh.numVertices;
}

void addIndex(Mesh& mesh, int index)
{
    mesh.indexBuffer.push_back(index);
    ++mesh.numIndices;

    if (mesh.numIndices % 3 == 0)
        ++mesh.numTris;
}

void Mesh::setup_shdrpgm(std::string shader)
{
    std::string vert = "../shaders/";
    std::string frag = "../shaders/";
    vert = vert + shader + ".vert";
    frag = frag + shader + ".frag";

    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, vert));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, frag));
    renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == renderProg.IsLinked()) 
    {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Mesh::setup_mesh()
{
      glUseProgram(renderProg.GetHandle());

    /*  Obtain the locations of the variables in the shaders with the given names */
    modelLoc = glGetUniformLocation(renderProg.GetHandle(), "model");
    viewLoc = glGetUniformLocation(renderProg.GetHandle(), "view");
    colorLoc       = glGetUniformLocation(renderProg.GetHandle(), "color");
    projectionLoc = glGetUniformLocation(renderProg.GetHandle(), "projection");
    LightLoc = glGetUniformLocation(renderProg.GetHandle(), "lightPos");
    ViewPosLoc = glGetUniformLocation(renderProg.GetHandle(), "viewPos");



    SendVertexData();

    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*  Initially drawing using filled mode */

    /*  Hidden surface removal */
    glEnable(GL_DEPTH_TEST);
   // glDepthFunc(GL_LESS);

  // glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */
}

void Mesh::compute_matrix([[maybe_unused]]float delta_time)
{

}

void Mesh::draw(glm::vec3 color, glm::mat4 view, glm::mat4 projection, glm::vec3 light_pos, glm::vec3 view_pos)
{
    glm::mat4 model = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    model = glm::translate(model, position);
    model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, { scale.x,scale.y,scale.z });

    
    glUniform4fv(colorLoc, 1, ValuePtr(color));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(LightLoc, 1,  ValuePtr(light_pos));
    glUniform3fv(ViewPosLoc, 1, ValuePtr(view_pos));

    //todo renderProg.use??
    //renderProg.Use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

}

void Mesh::init(std::string shader, glm::vec3 Pos, glm::vec3 Scale, glm::vec3 Rotate)
{
    position = Pos;
    scale = Scale;
    rotation = Rotate;
    setup_shdrpgm(shader);
    setup_mesh();
}

void Mesh::SendVertexData()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /*  Copy vertex attributes to GPU */
    glBufferData(GL_ARRAY_BUFFER,
        numVertices * vertexSize, &vertexBuffer[0],
        GL_DYNAMIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        numIndices * indexSize, &indexBuffer[0],
        GL_DYNAMIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, vLayout[i].normalized, vertexSize, (void*)vLayout[i].offset);
    }
}