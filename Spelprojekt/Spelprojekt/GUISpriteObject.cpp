#include "GUISpriteObject.h"
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

//GUISpriteObject::GUISpriteObject()
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

GUISpriteObject::GUISpriteObject(DX11Handler& dx11, float x, float y)
{
	// Store dxhandler
	this->dxHandler = &dx11;
	this->spritebatch = nullptr;

	// Store position
	this->m_XPosition = x;
	this->m_YPosition = y;
	// Set position
	this->position = DirectX::XMVectorSet(this->m_XPosition, this->m_YPosition, 0, 0);

	// Store scale
	this->m_XScale = 1.0f;
	this->m_YScale = 1.0f;
	// Set scale
	this->scale = DirectX::XMVectorSet(this->m_XScale, this->m_YScale, 1, 1);
	
	// Needed for drawfunction //
	this->m_Rot = 0.0f;									// NO USE RIGHT NOW
	this->color = DirectX::XMVectorSet(1, 1, 1, 1);		// NO USE RIGHT NOW
	this->origin = DirectX::XMVectorSet(1, 1, 1, 1);	// NO USE RIGHT NOW
	// - - - - - - - - - - - - //	
}

GUISpriteObject::~GUISpriteObject()
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

void GUISpriteObject::Draw(DirectX::SpriteBatch* spritebatch)
{	
	spritebatch->Draw(SRV, this->position, nullptr, this->color, m_Rot, origin, scale,DirectX::SpriteEffects::SpriteEffects_None, 0.0f);
}

void GUISpriteObject::SetPosition(float x, float y)
{
	this->m_XPosition = x;
	this->m_YPosition = y;
	this->position = DirectX::XMVectorSet(this->m_XPosition, this->m_YPosition, 0, 0);
}

void GUISpriteObject::SetWICSprite(DX11Handler& dx11, std::string spriteFile)
{
	if (this->SRV != nullptr)
		this->SRV->Release();			

	HRESULT result;
	std::wstring wsConvert(spriteFile.begin(), spriteFile.end());
	result = DirectX::CreateWICTextureFromFile(dx11.GetDevice(), wsConvert.c_str(), nullptr, &SRV);
	assert(SUCCEEDED(result));
}

void GUISpriteObject::SetDDSSprite(DX11Handler& dx11, std::string spriteFile)
{
	if (this->SRV != nullptr)
		this->SRV->Release();

	HRESULT result;
	std::wstring wsConvert(spriteFile.begin(), spriteFile.end());
	result = DirectX::CreateDDSTextureFromFile(dx11.GetDevice(), wsConvert.c_str(), nullptr, &SRV);
	assert(SUCCEEDED(result));
}

void GUISpriteObject::HealthBar(float maxHealth, float currentHealth)
{
	this->m_XScale = currentHealth/maxHealth;
	float xs = this->m_XScale;
	this->scale = DirectX::XMVectorSet(xs, 1, 1, 1);
}

