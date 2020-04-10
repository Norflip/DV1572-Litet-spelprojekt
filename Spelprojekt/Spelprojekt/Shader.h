#pragma once
#include <d3d11.h>    // D3D 11
#include <dxgi.h>     // anv�nds f�r swapchain och device / deviceContext
#include <d3dcompiler.h> // for shaders
#include "Logger.h"

constexpr D3D11_INPUT_ELEMENT_DESC INPUT_LAYOUT_V_UV_N_T[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	 0, D3D11_INPUT_PER_VERTEX_DATA, 0},		// 0 + 12 b = 12 b		// D3D11_APPEND_ALIGNED_ELEMENT
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0},		// 12 + 8 b = 20 b		
	{"NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,	20, D3D11_INPUT_PER_VERTEX_DATA, 0},		// 20 + 12 b = 32b
	{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}			// 32 + 12 b = 44b
};

class Shader
{
public:

	Shader();
	virtual ~Shader();

	void LoadPixelShader(LPCWSTR path, LPCSTR entry, ID3D11Device* device);
	ID3D11PixelShader* GetPixelShader() const { return this->pixelShader; }
	
	void LoadVertexShader(LPCWSTR path, LPCSTR entry, ID3D11Device* device);
	ID3D11VertexShader* GetVertexShader() const { return this->vertexShader; }
	ID3D11InputLayout* GetInputLayout() const { return this->inputLayout; }

	void Apply(ID3D11DeviceContext*);

private:
	void HandleShaderError(ID3DBlob* errorBlob);

private:

	DWORD flags;
	ID3D11PixelShader* pixelShader;
	ID3D11VertexShader* vertexShader;
	ID3D11InputLayout* inputLayout;
};