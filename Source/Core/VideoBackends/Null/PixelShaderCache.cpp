// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/Debugger.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/Statistics.h"

#include "VideoBackends/Null/PixelShaderCache.h"

namespace Null
{

PixelShaderCache::PSCache PixelShaderCache::PixelShaders;
const std::string* PixelShaderCache::last_entry;
PixelShaderUid PixelShaderCache::last_uid;
UidChecker<PixelShaderUid, ShaderCode> PixelShaderCache::pixel_uid_checker;

void PixelShaderCache::Init()
{

	Clear();

	SETSTAT(stats.numPixelShadersCreated, 0);
	SETSTAT(stats.numPixelShadersAlive, 0);

	last_entry = nullptr;
}

// ONLY to be used during shutdown.
void PixelShaderCache::Clear()
{
	PixelShaders.clear();

	last_entry = nullptr;
}

void PixelShaderCache::Shutdown()
{
	Clear();
}

bool PixelShaderCache::SetShader(DSTALPHA_MODE dstAlphaMode)
{
	PixelShaderUid uid = GetPixelShaderUid(dstAlphaMode, API_OPENGL);

	// Check if the shader is already set
	if (last_entry)
	{
		if (uid == last_uid)
		{
			return true;
		}
	}

	last_uid = uid;

	// Check if the shader is already in the cache
	PSCache::iterator iter;
	iter = PixelShaders.find(uid);
	if (iter != PixelShaders.end())
	{
		const std::string &entry = iter->second;
		last_entry = &entry;

		GFX_DEBUGGER_PAUSE_AT(NEXT_PIXEL_SHADER_CHANGE,true);
		return true;
	}

	// Need to compile a new shader
	ShaderCode code = GeneratePixelShaderCode(dstAlphaMode, API_OPENGL);
	PixelShaders.insert(std::make_pair(uid, code.GetBuffer()));

	GFX_DEBUGGER_PAUSE_AT(NEXT_PIXEL_SHADER_CHANGE, true);
	return true;
}

}
