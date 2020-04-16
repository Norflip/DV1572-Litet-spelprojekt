#pragma once
#include "Texture.h"
#include "Shader.h"
#include <unordered_map>

enum PIXEL_TYPE
{
	VERTEX,
	PIXEL
};

class Material
{
public:
	
	Material(Shader* shader);
	virtual ~Material();

	void SetTexture(size_t index, Texture* texture, PIXEL_TYPE type);
	Texture* GetTexture(size_t index) const;

	Shader* GetShader() const { return this->shader; }
	void Apply(ID3D11DeviceContext*);

private:

	Shader* shader;
	std::unordered_map<size_t, std::pair<Texture*, PIXEL_TYPE>> textureMap;
};