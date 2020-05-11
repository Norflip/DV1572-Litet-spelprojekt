#include "Shader.h"

Shader::Shader() : pixelShader(nullptr), vertexShader(nullptr), geometryShader(nullptr), inputLayout(nullptr)
{
	this->id = shader_id_counter++;
	Logger::Write("Created shader with id " + std::to_string(this->id));

	flags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	flags = flags | D3DCOMPILE_DEBUG;
#endif
}

Shader::~Shader()
{
	if (vertexShader != nullptr)
	{ 
		vertexShader->Release();
		vertexShader = 0;
	}

	if (inputLayout != nullptr)
	{
		inputLayout->Release();
		inputLayout = 0;
	}

	if (pixelShader != nullptr)
	{
		pixelShader->Release();
		pixelShader = 0;
	}
	if (geometryShader != nullptr)
	{
		geometryShader->Release();
		geometryShader = 0;
	}
}

void Shader::LoadPixelShader(LPCWSTR path, LPCSTR entry, ID3D11Device* device)
{
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* pixelShaderBlob = nullptr;

	HRESULT pixelShaderCompileResult = D3DCompileFromFile
	(
		path,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry, 
		"ps_5_0",
		flags, 
		0, 
		&pixelShaderBlob, 
		&errorBlob
	);
	
	if (FAILED(pixelShaderCompileResult) && errorBlob)
	{
		PrintShaderError(errorBlob);
		errorBlob->Release();
	}

	HRESULT psCompileResult = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &this->pixelShader);
	assert(SUCCEEDED(psCompileResult));
}

void Shader::LoadVertexShader(LPCWSTR path, LPCSTR entry, ID3D11Device* device)
{
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* vertexShaderBlob = nullptr;

	HRESULT	vertShaderSucc = D3DCompileFromFile
	(
		path,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry,
		"vs_5_0",
		0,
		0,
		&vertexShaderBlob,
		&errorBlob
	);
	
	if (FAILED(vertShaderSucc) && errorBlob)
	{
		PrintShaderError(errorBlob);
		errorBlob->Release();
	}

	HRESULT vsCompileResult = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &this->vertexShader);
	assert(SUCCEEDED(vsCompileResult));

	HRESULT vsInputLayoutResult = device->CreateInputLayout(INPUT_LAYOUT_V_UV_N_T, ARRAYSIZE(INPUT_LAYOUT_V_UV_N_T), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
	assert(SUCCEEDED(vsInputLayoutResult));
}
void Shader::LoadGeometryShader(LPCWSTR path, LPCSTR entry, ID3D11Device* device)
{
	ID3DBlob* errorBlob = nullptr;
	ID3DBlob* geometryShaderBlob = nullptr;

	HRESULT geometryShaderCompileResult = D3DCompileFromFile
	(
		path,
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entry,
		"gs_5_0",
		flags,
		0,
		&geometryShaderBlob,
		&errorBlob
	);
	assert(SUCCEEDED(geometryShaderCompileResult));

	if (FAILED(geometryShaderCompileResult) && errorBlob)
	{
		PrintShaderError(errorBlob);
		errorBlob->Release();
	}

	HRESULT psCompileResult = device->CreateGeometryShader(geometryShaderBlob->GetBufferPointer(), geometryShaderBlob->GetBufferSize(), nullptr, &this->geometryShader);
	assert(SUCCEEDED(psCompileResult));

}
void Shader::Bind(ID3D11DeviceContext* context)
{
	// sets the vertex shader and layout
	context->IASetInputLayout(inputLayout);
	context->GSSetShader(geometryShader, 0, 0);

	context->VSSetShader(vertexShader, 0, 0);

	//sets the pixel shader	
	context->PSSetShader(pixelShader, 0, 0);
}

void Shader::PrintShaderError(ID3DBlob* errorBlob)
{
	OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	Logger::Write(LOG_LEVEL::Error, (char*)errorBlob->GetBufferPointer());
}
