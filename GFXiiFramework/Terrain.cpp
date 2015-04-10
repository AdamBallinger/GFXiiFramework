#include "Terrain.h"

Terrain::Terrain()
{
	diffuseTexture = new OGLTexture();
	specularTexture = new OGLTexture();
	normalTexture = new OGLTexture();
}

Terrain::~Terrain()
{
	delete mesh;
	delete diffuseTexture;
	delete specularTexture;
	delete normalTexture;
}

void Terrain::SetMesh(LPCWSTR _meshFile)
{
	mesh = new OGLMesh(_meshFile);
}

void Terrain::SetDiffuseTex(std::string _diffuseFile)
{
	diffuseTexture->CreateTextureFromFile(_diffuseFile.c_str());
	mesh->SetTexture(diffuseTexture);
}

void Terrain::SetSpecularTex(std::string _specularFile)
{
	specularTexture->CreateTextureFromFile(_specularFile.c_str());
	mesh->SetSpecTexture(specularTexture);
}

void Terrain::SetNormalTex(std::string _normalFile)
{
	normalTexture->CreateTextureFromFile(_normalFile.c_str());
	mesh->SetNormalTexture(normalTexture);
}

void Terrain::Render()
{
	mesh->Render();
}