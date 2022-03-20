#pragma once
#include"../math.h"
class Test
{
public:
	Test() {};
	virtual ~Test() {};

	virtual void Update(float deltaTime) {}
	virtual void Draw() {}
	virtual void OnImGuiRender() {};

	void ComputeViewProjMats()
	{

	}

private:
    Mat4 viewMat, projMat, vpMat;
    Mat4 baseMVPMat, partMVPMat;

    int eyeAlpha = 1;
    int eyeBeta = NUM_STEPS_PI / 2;
    int eyeRadius = 40;
    float nearPlane = 1.0f;
    float farPlane = 80.0f;
    float topPlane = 0.6f * nearPlane;
    float bottomPlane = -topPlane;

    /// ////// todo 1800 = width,  900 = height
    float aspect = 1.0f * 1800 /900;


    float rightPlane = topPlane * aspect;
    float leftPlane = -rightPlane;
    bool eyeMoved = true;     /*  to trigger view matrix update */
    bool resized = true;     /*  to trigger projection matrix update */
};