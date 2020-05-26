#pragma once

#include "DX11Handler.h"
#include "Entities.h"
#include "Resources.h"
#include "SpawnObjects.h"
#include "Terrain.h"
#include "Gamemanager.h"

struct WorldContext
{
	Entities* entities;
	Resources* resources;
	SpawnObjects* spawner;
	Terrain* terrain;
	Gamemanager* gamemanager;
	DX11Handler* dx11;
};