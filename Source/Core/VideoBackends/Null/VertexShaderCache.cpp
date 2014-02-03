// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "VideoCommon/Debugger.h"
#include "VideoCommon/Statistics.h"
#include "VideoCommon/VertexShaderGen.h"

#include "VideoBackends/Null/VertexShaderCache.h"

namespace Null {

VertexShaderCache::VSCache VertexShaderCache::vshaders;
const std::string *VertexShaderCache::last_entry;
VertexShaderUid VertexShaderCache::last_uid;


void VertexShaderCache::Init()
{

	Clear();

	SETSTAT(stats.numVertexShadersCreated, 0);
	SETSTAT(stats.numVertexShadersAlive, 0);

	last_entry = NULL;
}

void VertexShaderCache::Clear()
{
	vshaders.clear();

	last_entry = NULL;
}

void VertexShaderCache::Shutdown()
{
	Clear();
}

bool VertexShaderCache::SetShader()
{
	VertexShaderUid uid = GetVertexShaderUid(API_D3D);

	if (last_entry)
	{
		if (uid == last_uid)
		{
			return true;
		}
	}

	last_uid = uid;

	VSCache::iterator iter = vshaders.find(uid);
	if (iter != vshaders.end())
	{
		const std::string &entry = iter->second;
		last_entry = &entry;

		GFX_DEBUGGER_PAUSE_AT(NEXT_VERTEX_SHADER_CHANGE, true);
		return true;
	}

	ShaderCode code = GenerateVertexShaderCode(API_D3D);
	vshaders.insert(std::make_pair(uid, code.GetBuffer()));

	GFX_DEBUGGER_PAUSE_AT(NEXT_VERTEX_SHADER_CHANGE, true);
	return true;
}

}  // namespace Null
