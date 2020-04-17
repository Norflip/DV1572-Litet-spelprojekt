#include "Player.h"
Player::Player(Mesh* mesh, Material* material, Input* input, TerrainGenerator* terrain)
	:input(input), terrain(terrain), Object(mesh, material)
{
	this->scaleY = terrain->getVerticalScaling();
	this->scaleXZ = terrain->getXzScale();
	this->movementspeed = 3;
}

Player::~Player()
{

}

void Player::Update(const float& deltaTime)
{

	UpdateMovement(deltaTime);
	UpdateHeight(deltaTime);

}



void Player::UpdateMovement(float FixedDeltaTime)
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, GetTransform().GetPosition());

	if (input->GetKey('w'))
		position.z += FixedDeltaTime * movementspeed;
	if (input->GetKey('a'))
		position.x -= FixedDeltaTime * movementspeed;
	if (input->GetKey('s'))
		position.z -= FixedDeltaTime * movementspeed;
	if (input->GetKey('d'))
		position.x += FixedDeltaTime * movementspeed;

	GetTransform().SetPosition({ position.x, position.y, position.z });
}

void Player::UpdateHeight(float FixedDeltaTime)
{
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, GetTransform().GetPosition());

	int xpos = position.x / scaleXZ;
	int zpos = position.z / scaleXZ;
	int whichVertex = terrain->getWidth() * zpos + xpos;
	MeshVertex te = terrain->getMesh()->vertexes.at(whichVertex);
	float howFarX = position.x - xpos;
	float howFarZ = position.z - zpos;
	float yTemp = 0;

	bool bottomTriangle;

	if ((howFarX + howFarZ) / 2 < 0.5f)
		bottomTriangle = true;
	else
		bottomTriangle = false;


	int bottomLeft=0;
	int topLeft=0;
	int bottomRight=0;
	int topRight = 0;
	float heightX = 0;
	float heightZ = 0;
	float heightTot = 0;

	if (bottomTriangle)
	{
		 bottomLeft = whichVertex;
		 topLeft = whichVertex+(zpos+1*terrain->getWidth());
		 bottomRight = whichVertex+1;
		 topRight = whichVertex +1+ (zpos + 1 * terrain->getWidth());
		 // lets solve one direction first
		 heightX = lerp(terrain->getMesh()->vertexes.at(bottomLeft).position.y, terrain->getMesh()->vertexes.at(bottomRight).position.y, howFarX);
			
		 heightZ = lerp(terrain->getMesh()->vertexes.at(bottomLeft).position.y, terrain->getMesh()->vertexes.at(topLeft).position.y, howFarZ);

		 heightTot = lerp (heightZ , heightX, (1.f-howFarX-howFarZ));

	}	
	else
	{
		bottomLeft = whichVertex;
		topLeft = whichVertex + (zpos + 1 * terrain->getWidth());
		bottomRight = whichVertex + 1;
		topRight = whichVertex + 1 + (zpos + 1 * terrain->getWidth());

		heightX = lerp(terrain->getMesh()->vertexes.at(topLeft).position.y, terrain->getMesh()->vertexes.at(topRight).position.y, howFarX);

		heightZ = lerp(terrain->getMesh()->vertexes.at(bottomRight).position.y, terrain->getMesh()->vertexes.at(topRight).position.y, howFarZ);

		heightTot = lerp(heightZ, heightX, (1.f - howFarX - howFarZ));
	}

	GetTransform().SetPosition({ position.x, heightTot + 1, position.z });
	//playerObject->GetTransform().SetPosition({ position.x, (terrain->getMesh()->vertexes.at(whichVertex).position.y)+1, position.z });


	//std::cout << "bottomTriangle " << bottomTriangle << std::endl << std::endl;
	//std::cout << "bottomLeft " << bottomLeft << std::endl;
	//std::cout << "topLeft " << topLeft << std::endl;
	//std::cout << "bottomRight " << bottomRight << std::endl<<std::endl;


	////std::cout <<"Xpos " << xpos << std::endl;
	////std::cout << "Zpos " << zpos << std::endl;
	//std::cout << "howFarX " << howFarX << std::endl;
	//std::cout << "howFarZ " << howFarZ << std::endl;
	//std::cout << "terrainWidth " << terrain->getWidth()<< std::endl;
	//std::cout << "scaleXZ " << scaleXZ << std::endl;
	//std::cout << "scaleY " << scaleY << std::endl;
	//std::cout << "bottomleft Vert is "<<  whichVertex << std::endl;

}
