cmake_minimum_required(VERSION 2.8)
project (IAT_Patcher)

message (STATUS "parser_dir='${PARSER_DIR}'")
message (STATUS "parser_lib='${PARSER_LIB}'")

find_package (Qt4 REQUIRED)

include_directories (${PARSER_DIR} ${QT_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR})

set (hdrs
    Executables.h
    FuncReplacements.h
    StubMaker.h
    Stub.h
    Stub32.h
    Stub64.h
    Stub32Data.h
    Stub64Data.h
    ExeController.h
    InfoTableModel.h
    ImportsTableModel.h
    FileLoader.h
    mainwindow.h
    resource.h
)

set (srcs
    Executables.cpp
    FuncReplacements.cpp
    StubMaker.cpp
    Stub.cpp
    Stub32.cpp
    Stub64.cpp
    ExeController.cpp
    InfoTableModel.cpp
    ImportsTableModel.cpp
    FileLoader.cpp
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
QT4_WRAP_CPP (hdrs_moc ${hdrs})

SOURCE_GROUP("Source Files\\Auto Generated" FILES ${hdrs_moc} ${rcc_src} )

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

add_executable (IAT_Patcher IAT_Patcher.rc ${hdrs_moc} ${forms_hdrs} ${hdrs} ${srcs}  ${rcc_src})
target_link_libraries (IAT_Patcher ${PARSER_LIB} ${QT_QTGUI_LIBRARIES} ${QT_QTCORE_LIBRARIES})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

# dependencies
add_dependencies(IAT_Patcher bearparser)


