#pragma once
#include "Camera.h"

class CameraController
{
public:
	
	CameraController(Camera* camera);
	virtual ~CameraController();
	
	void Update(const float& deltaTime);

};