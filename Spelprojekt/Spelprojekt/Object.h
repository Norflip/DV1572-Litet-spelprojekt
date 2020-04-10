#pragma once
#include "Transform.h"
class Renderer;

class Object
{
public:
	Object();
	virtual ~Object();

	Transform& GetTransform() { return this->transform; }
	void Draw(Renderer*);

private:
	Transform transform;
};