cmake_minimum_required(VERSION 2.8)
project (IAT_Patcher)

message (STATUS "parser_dir='${PARSER_DIR}'")
message (STATUS "parser_lib='${PARSER_LIB}'")

find_package (Qt4 REQUIRED)

include_directories (${PARSER_DIR} ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})

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
    mainwindow.cpp
    main.cpp
)

set (forms 
    mainwindow.ui
    replacements.ui
)

set (rcc application.qrc )
qt4_add_resources(rcc_src ${rcc})

QT4_WRAP_UI (forms_hdrs ${forms})
QT4_WRAP_CPP (hdrs_moc ${hdrs} ${dllparse_hdrs})

SOURCE_GROUP("Source Files\\Auto Generated" FILES ${hdrs_moc} ${rcc_src} )
SOURCE_GROUP("Source Files\\stub" FILES ${stub_srcs} )
SOURCE_GROUP("Header Files\\stub" FILES ${stub_hdrs} )
SOURCE_GROUP("Source Files\\dllparse" FILES ${dllparse_srcs} )
SOURCE_GROUP("Header Files\\dllparse" FILES ${dllparse_hdrs} )

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable (IAT_Patcher IAT_Patcher.rc ${hdrs_moc} ${forms_hdrs} ${stub_srcs} ${stub_hdrs} ${dllparse_hdrs} ${dllparse_srcs} ${hdrs} ${srcs}  ${rcc_src})
target_link_libraries (IAT_Patcher ${PARSER_LIB} ${QT_QTGUI_LIBRARIES} ${QT_QTCORE_LIBRARIES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# dependencies
add_dependencies(IAT_Patcher bearparser)


