cmake_minimum_required(VERSION 3.1.0)
project(swipl-swipl-win)

include("../cmake/PrologPackage.cmake")
include(Install)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG)

if(Qt5Widgets_FOUND)

set(PLWIN_SRC main.cpp SwiPrologEngine.cpp Swipl_IO.cpp Preferences.cpp
    pqMainWindow.cpp pqConsole.cpp FlushOutputEvents.cpp ConsoleEdit.cpp
    Completion.cpp swipl_win.cpp ParenMatching.cpp)

set(PLWIN_MOC
    qrc_swipl-win.cpp
    moc_SwiPrologEngine.cpp moc_Swipl_IO.cpp moc_Preferences.cpp
    moc_pqMainWindow.cpp moc_ConsoleEdit.cpp moc_swipl_win.cpp
    moc_ParenMatching.cpp)

set(QT_DEFINES)

# provide appropriate linking mode for
# static compilation of pqConsole source files
list(APPEND QT_DEFINES -DPQCONSOLE_STATIC)

# remove old defines in foreign predicates params
# due to conflicting Qt5 template arguments
list(APPEND QT_DEFINES -DPL_SAFE_ARG_MACROS)

# QPlainTextEdit vs QTextEdit : faster lightweight format
# TBD evaluate timing difference
list(APPEND QT_DEFINES -DPQCONSOLE_NO_HTML)

# reactive interface
# TBD as above
list(APPEND QT_DEFINES -DPQCONSOLE_BROWSER)

# disable message hoovering and feedback, leave to reactive HTML
list(APPEND QT_DEFINES -DPQCONSOLE_HANDLE_HOOVERING)

# added code from https://github.com/CapelliC/loqt/tree/master/lqUty
# to handle highlighting of selected text in console
list(APPEND QT_DEFINES -DLQUTY_STATIC)

include_directories(BEFORE
		    ${SWIPL_ROOT}/packages/cpp)

add_executable(swipl-win ${PLWIN_SRC})
target_link_libraries(swipl-win libswipl Qt5::Widgets)
target_compile_options(swipl-win PRIVATE ${QT_DEFINES})

install(TARGETS swipl-win
	RUNTIME DESTINATION ${SWIPL_INSTALL_ARCH_EXE})

if(NOT WIN32)
# Create symbolic link from public installation dir to executables
install(DIRECTORY DESTINATION bin)
ilink(${CMAKE_INSTALL_PREFIX}/${SWIPL_INSTALL_ARCH_EXE}/swipl-win
      ${CMAKE_INSTALL_PREFIX}/bin/swipl-win)
endif()

endif(Qt5Widgets_FOUND)
