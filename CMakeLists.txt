cmake_minimum_required (VERSION 2.8)
project (IAT_Patcher)

# modules:
set ( M_BEARPARSER "bearparser/parser" )
set ( M_IATP "patcher" )

# modules paths:
set (PARSER_DIR "${CMAKE_SOURCE_DIR}/${M_BEARPARSER}" CACHE PATH "BearParser main path")
set (PARSER_INC "${PARSER_DIR}/include" CACHE PATH "BearParser include path")

set (IATP_DIR "${CMAKE_SOURCE_DIR}/${M_IATP}" CACHE PATH "IATPatcher main path")

# Add sub-directories
#
# libs
add_subdirectory (${M_BEARPARSER})
set (PARSER_LIB bearparser CACHE FILE "BearParser library path")

# executables
add_subdirectory(patcher)

# dependencies
add_dependencies(IAT_Patcher bearparser)

