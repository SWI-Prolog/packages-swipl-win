# A Qt-based portable console for SWI-Prolog

swipl-win:

    a script that setup and run a fairly complete replacement of swipl-win.exe.

pqConsole:

  a basic Console, running SWI-Prolog PlEngine in background,
  presenting a QPlainTextEditor for user interface in foreground (GUI) thread.

Features

 - handling of keyboard input specialized for Prolog REPL
   and integration in TAB based multiwindow interfaces
 - output text colouring (subset of ANSI terminal sequences)
 - commands history
 - completion interface
 - swipl-win compatible API, allows menus to be added to top level widget,
   and enable creating a console for each thread
 - XPCE ready, allows reuse of current IDE components

author:  Carlo Capelli - Brescia 2013
licence: LGPL v2.1
