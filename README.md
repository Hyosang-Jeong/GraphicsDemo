# cs250-class-project-synergy
* cs250-class-project-synergy created by GitHub Classroom
* Procedure Modeling

* Toon/Cell & Fog shading
* Value Noise
* Gradient Noise
* Hermite Curve & Catmull Curve
* Geometry demo
* Tessellation Demo
* Shadow Demo
* Point Shadow Demo

You can run the project with Class Project.exe. When you run the project you can See ImGui Frame on left top. This is for changing the demo what you see.
1. Procedural Geometric Modeling
	- Plane, Cube, Sphere, Torus, Cylinder, and Cone
	- If you click "Procedure Modeling" button you can see six model.
	- You can see with wire frame if you click the wire frame button.
	- You can see with Fill frame if you click the Fill frame button.
	- You can interact with model with imgui, x,y,z degree slider float is for changing degree of model.
	- You can also change stack and slices with imgui.
	- All Six model has same ImGui function to interact.
2. Toon / Cell Shading, Fog
	- There are two sphere
	- If you click "Toon Fog" button you can see two sphere(yellow and blue)
	- You can change angle of view with Angle Imgui Slider floats
	- You can change FogMax,Min with ImgUi
	- You can change Toon shading level with Toon first,second,third layer SliderFloats
	- If you change that values, you can change the "diff" value for toon shading
3. Value noise
	- Implemented in C++ to generate a texture
	- Value,wood fractal marble turblence pattern
	- If you click "Value Noise" button you can see five texture.
	- You can see value Noise with Value Noise button and can control Frequency with Imgui.
	- You can see Wood with Wood button and can control Frequency with Imgui.
	- You can see Fractal with Fractal button and can control Frequency,frequencyMult,amplitudeMult,Numlayers with Imgui.
	- You can see Marble with Marble button and can control Frequency,frequencyMult,amplitudeMult,Numlayers with Imgui.
	- You can see Turblence with Turblence button and can control Frequency,frequencyMult,amplitudeMult,Numlayers with Imgui.
	- Also all texture would move right to left if you click the Animated button.
4. Gradient noise / Perlin noise
	- Implented in C++ to generate a terrain mesh
	- Gradient Noise and Gradient plane own creative model(Gradient sun).
	- If you click "Gradient Noise" button you can see three texture.
	- You can see Gradient Noise with Gradient Noise button and can control Frequency with Imgui.
	- You can see Gradient plane with Gradient plane button and can control Frequency with Imgui.
	- You can see Gradient Sun with Gradient Sun button and can control Frequency with Imgui.
	- You can see Marble with Marble button and can control Frequency,frequencyMult,amplitudeMult,Numlayers with Imgui.
	- Also all texture would move if you click the Animated button.
5. Hermite Curves / Catmull-Rom Splines
	- Hermite Curves and Catmull-Rom splines
	- If you click "Hermite Curve" button you can see two types of curve.
	- In Hermite Curve you can move the position of tangent vectors and start,end points with dragging mouse.
	- You can add vertex with Add Vertex button, if you click that button the vertex would be added in random position.
	- You can control the number of vertices with ImGui, if you control that number bigger, the points on line would be added more.
	- You can clear easily all vertex except origin one with Clear button.
	- In Catmull Ron Curve you can contral t_min and max. If you control it, you can see extended line of catmull Rom curve 
	- When you add vertex with Add Vertex button, the point would be added and next tangent vector would be added with randomly.
	- Also you can control vertivve number.
	- You can Clear all points except for origin one with Clear button.

6. Geometry and Tessellation Shaders
	- you can see two demo when you click Geometry Demo.
	- Two demos are own creative demo.
	- You can control stack and slice with ImGui
	- You can see othe demo with sphere mode_1_2, Also it has Animated button, the model would be moved if you click it.
7. Tesselation Demo
	- You can see big Sphere when you click the Tessellation Demo button.
	- You can control Shrink with ImGui
	- You can control Depth with ImGui
	- If you click the Animation, every shape would be moved with own progmatically calculation in shader.
	- If you click Grass button, you can see the shape of grass.
	- You can control TessLevelInner and Outer with ImGui
	- If you click the Animation button, the grass would be moved
8. Shadow Mapping
	- From a Directional or Sun light
	- You can control Light Position X,Y,Z
	- 

### Tech Requirements

You may use third party libraries to support your application(s). Things like glm, glfw, sdl2, Qt, stb, Dear ImGUI, etc... are all fine to use.

However all **CS250 topics must be implemented by the students**.

If you are unsure about whether or not you should use some third party library then ask the instructor.


## Submission

You will be submitting your work on GitHub with the provided class assignment repository. You will also create a copy of your project as a 7z file on the course site. 

The name of the 7z file must follow this naming convention: `[class_name]_[login]_project.7z`

For example, if a student had the login name fun.frank then the appropriate filename would be `cs250_fun.frank_project.7z`. Note: The .7z file names are case-sensitive and must be in all lowercase, exactly as described above.

In addition to the project development files, add pre-built binary files so that we can quickly and easily see your work running.

Note that we will be grading from the GitHub repo and we are using the 7z copy as a way to backup your work.

## Rubric

- [x] [**core**] All topics are implemented by students
- [x] [**core**] Demo has a scene that demonstrates **Procedural Geometric Modeling** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Toon / Cell Shading** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Fog** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Value noise** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Gradient noise / Perlin noise** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Hermite Curves / Catmull-Rom Splines** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Geometry and Tessellation Shaders** with tweakable parameters
- [x] [**core**] Demo has a scene that demonstrates **Shadow Mapping** with tweakable parameters
- [x] [**core**] Demo is implemented with OpenGL or an equivalent that allows graphics hardware acceleration 
- [x] Demo has an intuitive way to select a topic to view
- [x] Created a **Helpful** `ReadMe.md` file that explains how to build the project(s), run the project(s), and how to interact with the project(s).
- [x] An equal distribution of work can easily be seen by viewing the commit history
- [ ] All commit comments and/or pull request merges include a check-in buddy from someone else on the team
- [x] All source files compile without warnings and without errors.
- [x] Correct files submitted. No unnecessary files submitted.
- [x] A 7z archive of the project is created and submitted on the course site
- [x] 7z archive includes pre-built binaries that can be run right away
- [x] All source files have a proper header comment with your name, assignment name/number, course name, term.
- [x] Followed the appropriate Code Quality Guidelines and Code Style Guidelines.


### To earn extra credit

#### Add a new topic

We add the new topic that we have not study in class. We add the 'point shadow'. We have a demo for that topic.



