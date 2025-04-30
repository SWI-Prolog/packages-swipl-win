cmake_minimum_required(VERSION 3.10)
project(swipl-swipl-win)

include("../cmake/PrologPackage.cmake")
include(Install)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6Widgets_FOUND)
  find_package(Qt5Widgets CONFIG)
else()
  find_package(Qt6 COMPONENTS Widgets)		# Again for the warning
endif()

if(Qt5Widgets_FOUND OR Qt6Widgets_FOUND)

set(PLWIN_RESOURCES
    swipl-win.qrc)

if(Qt5Widgets_FOUND)
  set(CMAKE_CXX_STANDARD 17) # Required by SWI-cpp2.h (packages/cpp)
  qt5_add_resources(SWIPL_RES_SOURCES ${PLWIN_RESOURCES})
  set(QT_WIDGETS Qt5::Widgets)
else()
  set(CMAKE_CXX_STANDARD 17)
  set(QT_WIDGETS Qt6::Widgets)
  list(APPEND PLWIN_SRC ${PLWIN_RESOURCES})
endif()

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
target_link_libraries(swipl-win libswipl ${QT_WIDGETS})
target_compile_options(swipl-win PRIVATE ${QT_DEFINES})

install(TARGETS swipl-win
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${SWIPL_INSTALL_ARCH_EXE})
if(BUILD_MACOS_BUNDLE)
  install(FILES swipl.icns
	  DESTINATION ${SWIPL_INSTALL_RESOURCES})
endif()

if(SWIPL_INSTALL_AS_LINK)
# Create symbolic link from public installation dir to executables
install(DIRECTORY DESTINATION bin)
ilink(/${SWIPL_INSTALL_ARCH_EXE}/swipl-win
      /bin/swipl-win)
endif()

endif() # Qt5Widgets
