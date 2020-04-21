#include "GUISprite.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

//GUISprite::GUISprite()
//{
//	//this->dxHandler = nullptr;
//	this->SRV = nullptr;
//	this->spritebatch = nullptr;
//
//	this->color = DirectX::XMVectorSet(0, 0, 0, 0);
//	this->origin = DirectX::XMVectorSet(0, 0, 0, 0);
//	this->position = DirectX::XMVectorSet(0, 0, 0, 0);	
//	this->scale = DirectX::XMVectorSet(0, 0, 0, 0);
//
//	this->y = 0;
//	this->x = 0;
//	this->xScale = 0;
//	this->yScale = 0;
//	this->rot = 0.f;
//}

GUISprite::GUISprite(DX11Handler& dx11, std::string spriteFile, float x, float y)
{
	// Store dxhandler
	this->dxHandler = &dx11;
	this->spritebatch = nullptr;

	// Store position
	this->xPosition = x;
	this->yPosition = y;
	// Set position
	this->position = DirectX::XMVectorSet(this->xPosition, this->yPosition, 0, 0);

	// Store scale
	this->xScale = 1.0f;
	this->yScale = 1.0f;
	// Set scale
	this->scale = DirectX::XMVectorSet(this->xScale, this->yScale, 1, 1);
	
	// Needed for drawfunction //
	this->rotation = 0.0f;								// NO USE RIGHT NOW
	this->color = DirectX::XMVectorSet(1, 1, 1, 1);		// NO USE RIGHT NOW
	this->origin = DirectX::XMVectorSet(1, 1, 1, 1);	// NO USE RIGHT NOW
	// - - - - - - - - - - - - //	

	this->SRV = nullptr;

	HRESULT result;
	std::wstring wsConvert(spriteFile.begin(), spriteFile.end());
	result = DirectX::CreateWICTextureFromFile(dx11.GetDevice(), wsConvert.c_str(), nullptr, &SRV);
	assert(SUCCEEDED(result));
}

GUISprite::~GUISprite()
{
	if (SRV != nullptr) {
		SRV->Release();
		SRV = 0;
	}

	if (spritebatch != nullptr) {
		spritebatch = nullptr;
		spritebatch = 0;
	}
}

void GUISprite::Draw(DirectX::SpriteBatch* spritebatch)
{	
	spritebatch->Draw(SRV, this->position, nullptr, this->color, rotation, origin, scale,DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

void GUISprite::SetPosition(float x, float y)
{
	this->xPosition = x;
	this->yPosition = y;
	this->position = DirectX::XMVectorSet(this->xPosition, this->yPosition, 0, 0);
}

void GUISprite::SetWICSprite(DX11Handler& dx11, std::string spriteFile)
{
	if (this->SRV != nullptr)
		this->SRV->Release();			

	HRESULT result;
	std::wstring wsConvert(spriteFile.begin(), spriteFile.end());
	result = DirectX::CreateWICTextureFromFile(dx11.GetDevice(), wsConvert.c_str(), nullptr, &SRV);
	assert(SUCCEEDED(result));
}

void GUISprite::SetDDSSprite(DX11Handler& dx11, std::string spriteFile)
{
	if (this->SRV != nullptr)
		this->SRV->Release();

	HRESULT result;
	std::wstring wsConvert(spriteFile.begin(), spriteFile.end());
	result = DirectX::CreateDDSTextureFromFile(dx11.GetDevice(), wsConvert.c_str(), nullptr, &SRV);
	assert(SUCCEEDED(result));
}

void GUISprite::HealthBar(float maxHealth, float currentHealth)
{
	this->xScale = currentHealth/maxHealth;
	float xs = this->yScale;
	this->scale = DirectX::XMVectorSet(xs, 1, 1, 1);
}

