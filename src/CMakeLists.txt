add_library(dsp dsp.cpp)
add_library(AudioFile AudioFile.h)
set_target_properties(AudioFile PROPERTIES LINKER_LANGUAGE CXX)
