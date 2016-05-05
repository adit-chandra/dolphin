set(SRCS CommandLineParse.cpp
         Disassembler.cpp
         UICommon.cpp)

if(USE_X11)
  set(SRCS ${SRCS} X11Utils.cpp)
endif()

set(LIBS common)

add_dolphin_library(uicommon "${SRCS}" "${LIBS}")
