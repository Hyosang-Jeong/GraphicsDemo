# cs250-class-project-synergy

You can run the project with Class Project.exe in the pre-built folder - exe folder.
Or, you can open Class_project.sln in class_project folder and build(Ctrl+shift+B) and run the project(F5). 
When you run the project you can See ImGui Frame for switching demo. 

These are explanation of all demo and how to interact with them.
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
	- TessLevel is depending on the depth
	- If you click the Animation, every shape would be moved with own progmatically calculation in shader.
	- If you click Grass button, you can see the shape of grass.
	- You can control TessLevelInner and Outer with ImGui
	- If you click the Animation button, the grass would be moved
8. Shadow Mapping
	- From a Directional or Sun light
	- You can move the view with w,s,a,d,up,down key, also you can drag window for changing views
	- You can control Light Position X,Y,Z
	- You can control Field of View with Light FOV ImGui
	- You can control Near and Far plane Distant with Imgui
	- you can see with Nearest depth or Far depth with Imgui Button.
	- You can control glPolygonOffset factor and units.
	- You can see behind the shadow with Do shadow behind button
	- If you click the Animated button the light position would be moved like a sun.
	- You can change Dimension with Imgui
	- You can change the view with Camera view and Light View button.
9. Point Shadow Mapping
	- From a Directional or Sun light
	- You can move the view with w,s,a,d,up,down key, also you can drag window for changing views
	- You can control Light Position X,Y,Z
	- C,S,2,5,0 is model for testing point shadow, if you click them, you can see the model.
	- You can control the Dimension.

