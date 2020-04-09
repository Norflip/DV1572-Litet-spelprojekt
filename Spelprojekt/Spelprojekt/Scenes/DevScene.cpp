#include "DevScene.h"

DevScene::DevScene(Application* application, Renderer* renderer) : Scene(application, renderer)
{
}

DevScene::~DevScene()
{
}

void DevScene::Update(const float& deltaTime)
{
	// itererats through the objects and passes the renderer to the object.
	// sorts the objects based shader -> material properties -> object

	//Logger::Write(LOG_LEVEL::Info, "updating from devScene");
}
