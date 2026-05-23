# embed_shader.cmake
# Usage: cmake -DINPUT_FILE=<file> -DOUTPUT_FILE=<file> -DVAR_NAME=<name> -P embed_shader.cmake

if(NOT EXISTS "${INPUT_FILE}")
    message(FATAL_ERROR "Input file ${INPUT_FILE} does not exist.")
endif()

file(READ "${INPUT_FILE}" HEX_CONTENTS HEX)
string(REGEX MATCHALL ".." HEX_CHUNKS "${HEX_CONTENTS}")

set(ARRAY_CONTENTS "")
foreach(CHUNK IN LISTS HEX_CHUNKS)
    set(ARRAY_CONTENTS "${ARRAY_CONTENTS}0x${CHUNK}, ")
endforeach()

# Write the header file
file(WRITE "${OUTPUT_FILE}" "/* Auto-generated from ${INPUT_FILE} */\n")
file(APPEND "${OUTPUT_FILE}" "#pragma once\n\n")
file(APPEND "${OUTPUT_FILE}" "const unsigned char ${VAR_NAME}[] = { ${ARRAY_CONTENTS} };\n")
file(APPEND "${OUTPUT_FILE}" "const unsigned int ${VAR_NAME}_len = sizeof(${VAR_NAME});\n")
