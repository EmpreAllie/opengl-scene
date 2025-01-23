#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include "Globals.h"
#include "CUtils.h"
#include "CScene.h"

using namespace std;

int main() {
	GLFWwindow* window = CUtils::InitAll(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, false, DEFAULT_WINDOW_TITLE);

	//CScene scene;
	
	CScene::GetInstance().Init(window);
	CScene::GetInstance().Loop();
	
	CUtils::ClearAll();

	return 0;
}
