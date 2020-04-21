#pragma once
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include "GUIObject.h"

class GUISpriteObject: public GUIObject
{
public:
	GUISpriteObject(DX11Handler&, float x, float y);
	~GUISpriteObject();

	void Draw(DirectX::SpriteBatch*) override;
	void SetPosition(float x, float y) override;
	
	void SetWICSprite(DX11Handler& dx11, std::string spriteFile);
	void SetDDSSprite(DX11Handler& dx11, std::string spriteFile);
	
	void HealthBar(float maxHealth, float currentHealth);	// Update from player/enemies class current health

private:
	DX11Handler* dxHandler;
	DirectX::SpriteBatch* spritebatch;	
	ID3D11ShaderResourceView* SRV;

	float m_XPosition, m_YPosition;
	float m_XScale, m_YScale;
	float m_Rot; 

	DirectX::XMVECTOR position, color, origin, scale; 
};