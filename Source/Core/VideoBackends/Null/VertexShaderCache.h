// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>

#include "VideoCommon/VertexShaderGen.h"

namespace Null {

class VertexShaderCache
{
public:
	static void Init();
	static void Clear();
	static void Shutdown();
	static bool SetShader(); // TODO: Should be renamed to LoadShader

private:
	typedef std::map<VertexShaderUid, std::string> VSCache;

	static VSCache vshaders;
	static const std::string* last_entry;
	static VertexShaderUid last_uid;
};

}  // namespace Null
