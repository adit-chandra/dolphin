// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/IndexGenerator.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexLoaderManager.h"
#include "VideoCommon/VideoConfig.h"

#include "VideoBackends/Null/PixelShaderCache.h"
#include "VideoBackends/Null/VertexManager.h"
#include "VideoBackends/Null/VertexShaderCache.h"

namespace Null
{

class NullNativeVertexFormat : public NativeVertexFormat
{
public:
	NullNativeVertexFormat() {}
	void SetupVertexPointers() override {}
};

NativeVertexFormat* VertexManager::CreateNativeVertexFormat(const PortableVertexDeclaration& vtx_decl)
{
	return new NullNativeVertexFormat;
}

VertexManager::VertexManager()
{
	LocalVBuffer.resize(MAXVBUFFERSIZE);
	LocalIBuffer.resize(MAXIBUFFERSIZE);
}

VertexManager::~VertexManager()
{
}

void VertexManager::ResetBuffer(u32 stride)
{
	s_pCurBufferPointer = s_pBaseBufferPointer = LocalVBuffer.data();
	s_pEndBufferPointer = s_pCurBufferPointer + LocalVBuffer.size();
	IndexGenerator::Start(&LocalIBuffer[0]);
}

void VertexManager::vFlush(bool useDstAlpha)
{
	PixelShaderCache::SetShader(useDstAlpha ? DSTALPHA_DUAL_SOURCE_BLEND : DSTALPHA_NONE);
	VertexShaderCache::SetShader();
}


}  // namespace
