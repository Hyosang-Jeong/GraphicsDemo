/******************************************************************************/
/*!
\file   mesh.cpp
\par    Purpose: Library for generating polygonal meshes.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le, YOUR_NAME
\par    Email: sang.le\@digipen.edu, YOUR_EMAIL_ID
\par    DigiPen login: sang.le, YOUR_LOGIN_ID
\date   YOUR_COMPLETION_DATE
*/
/******************************************************************************/

#include "Mesh.h"

/*  Function prototype(s) */
void BuildIndexBuffer(int stacks, int slices, Mesh& mesh);
void addVertex(Mesh& mesh, const Vertex& v);
void addIndex(Mesh& mesh, int index);


/******************************************************************************/
/*!
\fn     Mesh CreatePlane(int stacks, int slices)
\brief
        Create a plane of dimension 1*1.
        The plane x/y-coordinates range from -0.5 to 0.5.
        The plane z-coordinate is 0.
\param  stacks
        Number of stacks on the plane
\param  slices
        Number of slices on the plane
\return
        The generated plane
*/
/******************************************************************************/
Mesh CreatePlane(int stacks, int slices)
{
    Mesh mesh;

    for (int stack = 0; stack <= stacks; ++stack)
    {
        float row = (float)stack / stacks;

        for (int slice = 0; slice <= slices; ++slice)
        {
            float col = (float)slice / slices;

            Vertex v;

            v.pos = glm::vec3(col - 0.5f, 0.5f - row, 0.0f);
            v.nrm = glm::vec3(0.0f, 0.0f, 1.0f);
            v.uv = glm::vec2(col, row);

            addVertex(mesh, v);
        }
    }

    BuildIndexBuffer(stacks, slices, mesh);

    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateCube(int stacks, int slices)
\brief
        Create a cube of dimension 1*1*1.
        The x/y/z-coordinates range from -0.5 to 0.5.
\param  stacks
        Number of stacks on each side of the cube
\param  slices
        Number of slices on each side of the cube
\return
        The generated cube
*/
/******************************************************************************/
Mesh CreateCube(int stacks, int slices)
{
    Mesh planeMesh = CreatePlane(stacks, slices);
    Mesh mesh;

    glm::vec3 const translateArray[] =
    {
        glm::vec3(+0.0f, +0.0f, +0.5f), // Z+
        glm::vec3(+0.0f, +0.0f, -0.5f), // Z-
        glm::vec3(+0.5f, +0.0f, +0.0f), // X+
        glm::vec3(-0.5f, +0.0f, +0.0f), // X-
        glm::vec3(+0.0f, +0.5f, +0.0f), // Y+
        glm::vec3(+0.0f, -0.5f, +0.0f), // Y-
    };

    glm::vec2 const rotateArray[] =
    {
        glm::vec2(+0.0f, +0.0f),             // Z+
        glm::vec2(+0.0f, (float)+PI),        // Z-
        glm::vec2(+0.0f, (float)+HALF_PI),   // X+       
        glm::vec2(+0.0f, (float)-HALF_PI),   // X-
        glm::vec2((float)-HALF_PI, +0.0f),   // Y+
        glm::vec2((float)+HALF_PI, +0.0f)    // Y-
    };


    /*  Transform the plane to 6 positions to form the faces of the cube */
    for (int i = 0; i < 6; ++i)
    {
        Mat4 transformMat = Translate(translateArray[i]) *
            Rotate(rotateArray[i][YINDEX], { 0,1,0 }) *
            Rotate(rotateArray[i][XINDEX], { 1,0,0 });

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
        {
            addIndex(mesh, planeMesh.indexBuffer[j] + planeMesh.numVertices * i);
        }
    }

    return mesh;
}


/******************************************************************************/
/*!
\fn     Mesh CreateSphere(int stacks, int slices)
\brief
        Create a sphere of radius 0.5.
        The center of the sphere is (0, 0, 0).
\param  stacks
        Number of stacks on the sphere
\param  slices
        Number of slices on the sphere
\return
        The generated sphere
*/
/******************************************************************************/
Mesh CreateSphere(int stacks, int slices)
{
    Mesh mesh;

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
//
//
///******************************************************************************/
///*!
//\fn     Mesh CreateCylinder(int stacks, int slices)
//\brief
//        Create a cylinder with radius 0.5 and height 1.
//        The center of the cylinder is (0, 0, 0).
//        The cylinder mesh should also contain two caps.
//\param  stacks
//        Number of stacks on the cylinder
//\param  slices
//        Number of slices on the cylinder
//\return
//        The generated cylinder
//*/
///******************************************************************************/
//Mesh CreateCylinder(int stacks, int slices)
//{
//    Mesh mesh;
//    Mesh top;
//    Mesh bottom;
//    float col = 0;
//    float alpha = 0;
//    for (int stack = 0; stack <= stacks; ++stack)
//    {
//        float row = (float)stack / stacks; // [0.0, 1.0]
//        float z_val = 0.5;
//        float diff = 2.0 * PI / slices;
//        for (int slice = 0; slice <= slices; ++slice)
//        {
//            col = (float)slice / slices;
//            alpha = col * PI * 2.0;
//
//            Vertex v;
//
//            // side
//            v.uv.x = row;
//            v.uv.y = col;
//
//            v.pos.x = 0.5 * sin(alpha);
//            v.pos.y = row - 0.5;
//            v.pos.z = 0.5 * cos(alpha);
//
//
//            v.nrm.x = v.pos.x / 0.5;
//            v.nrm.y = 0.0;
//            v.nrm.z = v.pos.z / 0.5;
//
//            addVertex(mesh, v);
//        }
//    }
//
//    BuildIndexBuffer(stacks, slices, mesh);
//
//    ////////////////////////////////////////////////
//
//    addIndex(top, 0);
//    addIndex(top, 1);
//    addIndex(top, 2);
//
//    addIndex(top, 0);
//    addIndex(top, 2);
//    addIndex(top, 3);
//
//    addIndex(top, 0);
//    addIndex(top, 3);
//    addIndex(top, 4);
//
//    addIndex(top, 0);
//    addIndex(top, 4);
//    addIndex(top, 5);
//
//    addIndex(top, 0);
//    addIndex(top, 5);
//    addIndex(top, 6);
//
//    addIndex(top, 0);
//    addIndex(top, 6);
//    addIndex(top, 7);
//
//    addIndex(top, 0);
//    addIndex(top, 7);
//    addIndex(top, 8);
//
//    addIndex(top, 0);
//    addIndex(top, 8);
//    addIndex(top, 1);
//
//    for (int j = 0; j < top.numIndices; ++j)
//        addIndex(mesh, mesh.numVertices + top.indexBuffer[j]);
//
//
//
//    Vertex upper_center;
//    upper_center.pos = Vec3{ 0,0.5,0 };
//    addVertex(mesh, upper_center);
//
//    for (int stack = 0; stack < stacks; ++stack)
//    {
//        float row = (float)stack / stacks; // [0.0, 1.0]
//        float diff = 2.0 * PI / slices;
//
//        for (int slice = 0; slice < slices; ++slice)
//        {
//            col = (float)slice / slices;
//            alpha = col * PI * 2.0;
//
//            Vertex upper;
//
//            upper.uv.x = row;
//            upper.uv.y = col;
//
//            upper.pos.x = 0.5f * sin(alpha);
//            upper.pos.y = 0.5f;
//            upper.pos.z = 0.5f * cos(alpha);
//
//            upper.nrm.x = 0;
//            upper.nrm.y = 1.0;
//            upper.nrm.z = 0;
//
//            addVertex(mesh, upper);
//        }
//    }
//
//
//
//    //bottom
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 1);
//    addIndex(bottom, 2);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 2);
//    addIndex(bottom, 3);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 3);
//    addIndex(bottom, 4);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 4);
//    addIndex(bottom, 5);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 5);
//    addIndex(bottom, 6);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 6);
//    addIndex(bottom, 7);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 7);
//    addIndex(bottom, 8);
//
//    addIndex(bottom, 0);
//    addIndex(bottom, 8);
//    addIndex(bottom, 1);
//
//    for (int j = 0; j < bottom.numIndices; ++j)
//        addIndex(mesh, mesh.numVertices + bottom.indexBuffer[j]);
//
//
//    upper_center.pos = Vec3{ 0,-0.5,0 };
//    addVertex(mesh, upper_center);
//
//    for (int stack = 0; stack < stacks; ++stack)
//    {
//        float row = (float)stack / stacks; // [0.0, 1.0]
//        float diff = 2.0 * PI / slices;
//
//        for (int slice = 0; slice < slices; ++slice)
//        {
//            col = (float)slice / slices;
//            alpha = col * PI * 2.0;
//
//            Vertex upper;
//
//            upper.uv.x = row;
//            upper.uv.y = col;
//
//            upper.pos.x = 0.5f * sin(alpha);
//            upper.pos.y = -0.5f;
//            upper.pos.z = 0.5f * cos(alpha);
//
//            upper.nrm.x = 0;
//            upper.nrm.y = -1.0;
//            upper.nrm.z = 0;
//
//            addVertex(mesh, upper);
//        }
//    }
//
//
//
//    return mesh;
//}
//
//
///******************************************************************************/
///*!
//\fn     Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle)
//\brief
//        Create a torus that starts from startAngle and ends at endAngle.
//        The major and minor radii are 0.35 and 0.15, respectively.
//        The center of the torus is (0, 0, 0).
//\param  stacks
//        Number of stacks on the torus
//\param  slices
//        Number of slices on the torus
//\param  startAngle
//        The starting angle of the torus
//\param  endAngle
//        The ending angle of the torus
//\return
//        The generated torus
//*/
///******************************************************************************/
//Mesh CreateTorus(int stacks, int slices, float startAngle, float endAngle)
//{
//    Mesh mesh;
//    for (int stack = 0; stack <= stacks; ++stack)
//    {
//        float row = (float)stack / stacks; // [0.0, 1.0]
//
//        float alpha = PI + startAngle + (row * (endAngle - startAngle)); // [0.0, 2¥ð]
//
//        float sinAlpha = sin(alpha);
//        float  cosAlpha = cos(alpha);
//
//        for (int slice = 0; slice <= slices; ++slice)
//        {
//            float col = (float)slice / slices;
//
//            float beta = col * PI * 2.0;
//
//            Vertex center;
//            center.pos = Vec3(0, 0, 0);
//
//            Vertex v;
//            v.uv.x = col;
//            v.uv.y = row;
//
//            v.pos.x = -(0.35 + (0.15 * cos(beta))) * sinAlpha;
//            v.pos.y = -0.15 * sin(beta);
//            v.pos.z = -(0.35 + (0.15 * cos(beta))) * cosAlpha;
//
//            v.nrm = v.pos - center.pos;
//
//            v.nrm /= 0.15;
//            addVertex(mesh, v);
//        }
//
//    }
//
//    BuildIndexBuffer(stacks, slices, mesh);
//    return mesh;
//}
//
///******************************************************************************/
///*!
//\fn     Mesh CreateCone(int stacks, int slices)
//\brief
//        Create a cone with base radius 0.5 and height 1.
//        The center along the axis of the cone is (0, 0, 0).
//        The cone mesh should also contain its base.
//\param  stacks
//        Number of stacks on the cone
//\param  slices
//        Number of slices on the cone
//\return
//        The generated cone
//*/
///******************************************************************************/
//Mesh CreateCone(int stacks, int slices)
//{
//    Mesh mesh;
//    Mesh top;
//    Mesh bottom;
//    float col = 0;
//    float alpha = 0;
//    for (int stack = 0; stack <= stacks; ++stack)
//    {
//        float row = ((float)stack / stacks) - 0.5; // [0.0, 1.0]
//        float z_val = 0.5;
//        for (int slice = 0; slice <= slices; ++slice)
//        {
//            col = (float)slice / slices;
//            alpha = col * PI * 2.0;
//
//            Vertex v;
//
//            // side
//            v.uv.x = row;
//            v.uv.y = col;
//
//            v.pos.x = 0.5 * (0.5 - row) * sin(alpha);
//            v.pos.y = row;
//            v.pos.z = 0.5 * (0.5 - row) * cos(alpha);
//
//
//            v.nrm.x = v.pos.x / 0.5;
//            v.nrm.y = 0.0;
//            v.nrm.z = v.pos.z / 0.5;
//
//            addVertex(mesh, v);
//        }
//    }
//    BuildIndexBuffer(stacks, slices, mesh);
//    return  mesh;
//}


/******************************************************************************/
/*!
\fn     void BuildIndexBuffer(int stacks, int slices, Mesh &mesh)
\brief
        Generate the index buffer for the mesh.
\param  stacks
        Number of stacks on the mesh
\param  slices
        Number of slices on the mesh
\param  mesh
        The mesh whose index buffer will be generated.
*/
/******************************************************************************/
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


/******************************************************************************/
/*!
\fn     void addVertex(Mesh &mesh, const Vertex &v)
\brief
        Add a vertex to a mesh
\param  mesh
        The mesh to be updated.
\param  v
        The vertex to be added.
*/
/******************************************************************************/
void addVertex(Mesh& mesh, const Vertex& v)
{
    mesh.vertexBuffer.push_back(v);
    ++mesh.numVertices;
}


/******************************************************************************/
/*!
\fn     void addIndex(Mesh &mesh, int index)
\brief
        Add an index to a mesh
\param  mesh
        The mesh to be updated.
\param  index
        The vertex index to be added.
*/
/******************************************************************************/
void addIndex(Mesh& mesh, int index)
{
    mesh.indexBuffer.push_back(index);
    ++mesh.numIndices;

    if (mesh.numIndices % 3 == 0)
        ++mesh.numTris;
}

void Mesh::setup_shdrpgm()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(
        GL_VERTEX_SHADER,
        "../shaders/triangle.vert"));
    shdr_files.push_back(std::make_pair(
        GL_FRAGMENT_SHADER,
        "../shaders/triangle.frag"));
    renderProg.CompileLinkValidate(shdr_files);
    if (GL_FALSE == renderProg.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << renderProg.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void Mesh::setup_mesh()
{
    glUseProgram(renderProg.GetHandle());

    /*  Obtain the locations of the variables in the shaders with the given names */
     mvpMatLoc = glGetUniformLocation(renderProg.GetHandle(), "mvpMat");
     colorLoc       = glGetUniformLocation(renderProg.GetHandle(), "color");
     textureLoc =   glGetUniformLocation(renderProg.GetHandle(), "tex");

    SendVertexData();

    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*  Initially drawing using filled mode */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*  Hidden surface removal */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);     /*  For efficiency, not drawing back-face */
}

void Mesh::compute_matrix([[maybe_unused]]float delta_time)
{
    glm::vec4 pos_tmp = World_to_NDC * glm::vec4(position, 1.0) ;
    position.x = pos_tmp.x;
    position.y = pos_tmp.y;

    glm::mat4 trans_mat =
    {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        position.x,position.y,position.z,1
    };
    glm::mat4 scale_mat =
    {
        scale.x,0,0,0,
        0,scale.y,0,0,
        0,0,scale.z,0,
        0,0,0,1
    };

    glm::mat4  rotate_x_mat =
    {
        1,0,0,0,
        0,cos(rotation.x),-sin(rotation.x),0,
       0,sin(rotation.x),cos(rotation.x),0,
        0,0,0,1
    };

    glm::mat4 rotate_y_mat =
    {
        cos(rotation.y),0,sin(rotation.y),0,
        0,1,0,0,
      -sin(rotation.y),0,cos(rotation.y),0,
        0,0,0,1
    };

    glm::mat4 rotate_z_mat =
    {
      cos(rotation.z), sin(rotation.z), 0 ,0,
    -sin(rotation.z), cos(rotation.z), 0, 0,
    0, 0 ,1, 0,
    0 ,0 ,0 ,1
    };

    glm::mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

    SRT_mat = trans_mat  * rotate_mat * scale_mat;


}

void Mesh::draw()
{



}

void Mesh::init(glm::vec3 Pos, glm::vec3 Scale, glm::vec3 Rotate)
{

    scale = Scale;
    rotation = Rotate;
    setup_shdrpgm();
    setup_mesh();
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    World_to_NDC = {
        2.f / viewport[2],0,0,0,
        0, 2.f / viewport[3],0,0,
        0,0,1,0,
       -1,-1,0,1
    };


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
        GL_STATIC_DRAW);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    /*  Copy vertex indices to GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        numIndices * indexSize, &indexBuffer[0],
        GL_STATIC_DRAW);

    /*  Send vertex attributes to shaders */
    for (int i = 0; i < numAttribs; ++i)
    {
        glEnableVertexAttribArray(vLayout[i].location);
        glVertexAttribPointer(vLayout[i].location, vLayout[i].size, vLayout[i].type, vLayout[i].normalized, vertexSize, (void*)vLayout[i].offset);
    }
}