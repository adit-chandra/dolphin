// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>

#include "VideoCommon/PixelShaderGen.h"

enum DSTALPHA_MODE;

namespace Null
{

class PixelShaderCache
{
public:
	static void Init();
	static void Clear();
	static void Shutdown();
	static bool SetShader(DSTALPHA_MODE dstAlphaMode); // TODO: Should be renamed to LoadShader

private:

	typedef std::map<PixelShaderUid, std::string> PSCache;

	static PSCache PixelShaders;
	static const std::string* last_entry;
	static PixelShaderUid last_uid;

	static UidChecker<PixelShaderUid, ShaderCode> pixel_uid_checker;
};

}  // namespace NULL
