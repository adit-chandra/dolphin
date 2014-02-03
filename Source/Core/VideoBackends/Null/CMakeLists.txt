set(SRCS
	main.cpp
	Render.cpp
	VertexManager.cpp
	PixelShaderCache.cpp
	VertexShaderCache.cpp
)

set(LIBS
	videocommon
	common
	dl
)

add_dolphin_library(videonull "${SRCS}" "${LIBS}")

