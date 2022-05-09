#include"Engine.h"

#include"tests/TestTriangle.h"
#include"tests/Toon_Fog.h"
#include"tests/ValueNoise.h"
#include"tests/GradientNoise.h"
#include"tests/Curve.h"
#include"tests/geometry_test.h"

int main() 
{
    Engine engine;
    TriangleTest* triangle = new TriangleTest();
    Toon_Fog* toon_fog = new Toon_Fog();
    Noise* value_noise = new Noise();
    Gradient_Noise* gradient_noise = new Gradient_Noise();
    CurveTest* curve_test = new CurveTest();
    Geometry_test* geometry_test = new Geometry_test();

   engine.Add(triangle);
   engine.Add(toon_fog);
   engine.Add(value_noise);
   engine.Add(gradient_noise);
   engine.Add(curve_test);
   engine.Add(geometry_test);

   engine.init();

    while (!engine.ShouldClose()) 
    {
        engine.Update();
        engine.Draw();
    }

}

