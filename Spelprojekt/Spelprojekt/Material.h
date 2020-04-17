#pragma once
#include "Texture.h"
#include "Shader.h"
#include <unordered_map>

enum class PIXEL_TYPE
{
	VERTEX,
	PIXEL
};

constexpr size_t ALBEDO_MATERIAL_TYPE = 0;
constexpr size_t NORMAL_MATERIAL_TYPE = 1;

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