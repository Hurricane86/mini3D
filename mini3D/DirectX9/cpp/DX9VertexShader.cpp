/*
This source file is part of mini3D. 
Copyright (c) <2010> <www.mini3d.org>

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "../DX9VertexBuffer.h"
#include <d3d9.h>


mini3d::DX9VertexShader::DX9VertexShader(DX9GraphicsService* pGraphicsService, const ShaderBytes& shaderBytes, const VertexDeclarationVector& vertexDeclaration) :
	pGraphicsService_(pGraphicsService), shaderBytes_(shaderBytes), pShaderBuffer_(0), vertexDeclaration_(vertexDeclaration)
{
	LoadResource();
	pGraphicsService->AddResource(this);
}

mini3d::DX9VertexShader::~DX9VertexShader(void)
{
	UnloadResource();
	pGraphicsService_->RemoveResource(this);
}

void mini3d::DX9VertexShader::LoadResource(void)
{
	IDirect3DDevice9* pDevice = pGraphicsService_->GetDevice();
	if (pDevice == 0)
		return;

	// If the buffer exists tear it down.
	if (pShaderBuffer_ != 0)
	{
		UnloadResource();
	}

	if( FAILED( pDevice->CreateVertexShader((DWORD*)&shaderBytes_[0], &pShaderBuffer_)))
	{
		isDirty_ = true;
		return;
	}

	isDirty_ = false;

	// load the vertex declaration into the pool
	pGraphicsService_->PoolVertexDeclaration(vertexDeclaration_);
}


void mini3d::DX9VertexShader::UnloadResource(void)
{
	if (pShaderBuffer_ != 0)
	{
		// if this is the currently loaded pixel shader, release it
		if (pGraphicsService_->GetVertexShader() == this)
			pGraphicsService_->SetVertexShader(0);

		pShaderBuffer_->Release();
		pShaderBuffer_ = 0;
	}

	isDirty_ = true;

	// remove the vertex declaration from the pool
	pGraphicsService_->ReleaseVertexDeclaration(vertexDeclaration_);

}
