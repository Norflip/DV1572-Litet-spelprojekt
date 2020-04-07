class Application;

class Scene
{
public:
	Scene(Application* application);
	~Scene();

	void Update(const float& deltaTime);

private:
	Application* application;
};