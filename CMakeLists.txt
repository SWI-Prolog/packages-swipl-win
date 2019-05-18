cmake_minimum_required(VERSION 3.1.0)
project(swipl-swipl-win)

include("../cmake/PrologPackage.cmake")
include(Install)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG)

if(Qt5Widgets_FOUND)

set(PLWIN_RESOURCES
    swipl-win.qrc)

qt5_add_resources(SWIPL_RES_SOURCES ${PLWIN_RESOURCES})

set(PLWIN_SRC main.cpp SwiPrologEngine.cpp Swipl_IO.cpp Preferences.cpp
    pqMainWindow.cpp pqConsole.cpp FlushOutputEvents.cpp ConsoleEdit.cpp
    Completion.cpp swipl_win.cpp ParenMatching.cpp ansi_esc_seq.cpp ${SWIPL_RES_SOURCES})

set(QT_DEFINES)

if(MULTI_THREADED)
list(APPEND QT_DEFINES -D_REENTRANT)
endif()

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

if(BUILD_MACOS_BUNDLE)
  set(SWIPL_APP_NAME SWI-Prolog)
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.swi-prolog.app")
  set(MACOSX_BUNDLE_INFO_STRING    "Created by the SWI-Prolog project")
  set(MACOSX_BUNDLE_COPYRIGHT      "BSD-2")
  set(MACOSX_BUNDLE_HELP_FILE      "swipl-macosx.html")
else()
  set(SWIPL_APP_NAME swipl-win)
endif()

add_executable(swipl-win MACOSX_BUNDLE ${PLWIN_SRC})
set_target_properties(swipl-win PROPERTIES
		      OUTPUT_NAME "${SWIPL_APP_NAME}"
		      MACOSX_BUNDLE_ICON_FILE swipl.icns)
target_link_libraries(swipl-win libswipl Qt5::Widgets)
target_compile_options(swipl-win PRIVATE ${QT_DEFINES})

install(TARGETS swipl-win
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${SWIPL_INSTALL_ARCH_EXE})
if(BUILD_MACOS_BUNDLE)
  install(FILES swipl.icns
	  DESTINATION ${SWIPL_INSTALL_RESOURCES})
endif()

if(NOT WIN32 AND NOT BUILD_MACOS_BUNDLE)
# Create symbolic link from public installation dir to executables
install(DIRECTORY DESTINATION bin)
ilink(${CMAKE_INSTALL_PREFIX}/${SWIPL_INSTALL_ARCH_EXE}/swipl-win
      ${CMAKE_INSTALL_PREFIX}/bin/swipl-win)
endif()

endif(Qt5Widgets_FOUND)
