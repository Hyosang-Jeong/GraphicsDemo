#include<vector>
#include"tests/Test.h"

class Engine
{
public:
	Engine();
	~Engine();
	void Add(Test* demo);
	void init();
	void Update();
	void Draw();
	bool ShouldClose();
	void demo_switch();
	std::vector<Test*> tests;
	int current;
};