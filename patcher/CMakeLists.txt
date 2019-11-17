cmake_minimum_required(VERSION 2.8)
project (IAT_Patcher)

message (STATUS "parser_dir='${PARSER_DIR}'")
message (STATUS "parser_lib='${PARSER_LIB}'")

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)

include_directories ( ${PARSER_INC} )

set (stub_hdrs
    stub/Stub.h
    stub/Stub32.h
    stub/Stub64.h
    stub/Stub32Data.h
    stub/Stub64Data.h
)

set (dllparse_hdrs
    dllparse/LibraryParser.h
    dllparse/LibraryInfo.h
    dllparse/LibsModel.h
    dllparse/FunctionsModel.h
)

set (dllparse_srcs
    dllparse/LibraryParser.cpp
    dllparse/LibraryInfo.cpp
    dllparse/LibsModel.cpp
    dllparse/FunctionsModel.cpp
)

set (hdrs
    StubMaker.h
    FuncReplacements.h
    ImportsLookup.h
    ExeHandler.h
    Executables.h
    ExeController.h
    InfoTableModel.h
    ImportsTableModel.h
    FileLoader.h
    ExeHandlerLoader.h
    ReplacementsDialog.h
    mainwindow.h
    resource.h
)

set (stub_srcs
    stub/Stub.cpp
    stub/Stub32.cpp
    stub/Stub64.cpp
)

set (srcs
    StubMaker.cpp
    FuncReplacements.cpp
    ImportsLookup.cpp
    ExeHandler.cpp
    Executables.cpp
    ExeController.cpp
    InfoTableModel.cpp
    ImportsTableModel.cpp
    FileLoader.cpp
    ExeHandlerLoader.cpp
    ReplacementsDialog.cpp
    mainwindow.cpp
    main.cpp
)

set (forms 
    mainwindow.ui
    replacements.ui
)

set (rcc application.qrc )
qt5_add_resources(rcc_src ${rcc})

QT5_WRAP_UI (forms_hdrs ${forms})
QT5_WRAP_CPP (hdrs_moc ${hdrs} ${dllparse_hdrs})

SOURCE_GROUP("Source Files\\Auto Generated" FILES ${hdrs_moc} ${rcc_src} )
SOURCE_GROUP("Source Files\\stub" FILES ${stub_srcs} )
SOURCE_GROUP("Header Files\\stub" FILES ${stub_hdrs} )
SOURCE_GROUP("Source Files\\dllparse" FILES ${dllparse_srcs} )
SOURCE_GROUP("Header Files\\dllparse" FILES ${dllparse_hdrs} )

IF (WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
ENDIF()

add_executable (IAT_Patcher IAT_Patcher.rc ${hdrs_moc} ${forms_hdrs} ${stub_srcs} ${stub_hdrs} ${dllparse_hdrs} ${dllparse_srcs} ${hdrs} ${srcs}  ${rcc_src})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(IAT_Patcher bearparser Qt5::Core Qt5::Widgets)

# dependencies
add_dependencies(IAT_Patcher bearparser)
