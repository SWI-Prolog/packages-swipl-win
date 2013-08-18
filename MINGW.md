# Building swipl-win using cross-compilation from Linux

This document explains the first tryout  for building SWI-Prolog and the
Qt swipl-win.exe from source using the MinGW cross-compilation toolchain
from Linux. Tested  on  Mint  14   (Ubuntu  12.10),  64-bit  edition and
targetting 64-bit Windows.

## Install SWI-Prolog from source

See README.mingw in the toplevel. The tree  starts at $install, which is
defined in README.mingw and here is a bit more of the toplevel

  $install
    bin
    include
    src
      pl-devel
    swipl

## Install Qt

Qt  binaries  for  MinGW  cross-compilation    may  be  downloaded  from
[here](http://files.quickmediasolutions.com/qt5/).     Download      the
x86_64/shared version.

The  Qt  tar   must   be   unpacked    such   that   the   contents   of
x86_64-w64-mingw32/bin ends up in $install/bin.

Now, create a file $install/bin/qt.conf with the content below.  Replace
<$install> with the $install prefix from README.mingw

    ~~~
    [Paths]
    Prefix = <$install>
    ~~~

After this, the command `qmake -query`   should display proper locations
of the Qt paths.

## Download swipl-win sources

Clone pqConsole and swipl-win in the pl-devel/packages folder using

    git clone https://github.com/CapelliC/swipl-win.git
    git clone https://github.com/CapelliC/pqConsole.git

## Compile swipl-win

This should be easy:

    qmake
    make

## Now install the distribution files into $install/swipl/bin:

    mkdir $install/swipl/bin/accessible
    mkdir $install/swipl/bin/platforms
    cp release/swipl-win.exe $install/swipl/bin
    cp $install/bin/Qt5Core.dll $install/swipl/bin
    cp $install/bin/Qt5Gui.dll $install/swipl/bin
    cp $install/bin/Qt5Widgets.dll $install/swipl/bin
    cp $install/plugins/platforms/qwindows.dll $install/swipl/bin/platforms
    cp $install/plugins/accessible/qtaccessiblewidgets.dll $install/swipl/bin/accessible
    cp /usr/lib/gcc/x86_64-w64-mingw32/4.6/libgcc_s_sjlj-1.dll $install/swipl/bin
    cp /usr/lib/gcc/x86_64-w64-mingw32/4.6/libstdc++-6.dll $install/swipl/bin

At this moment, $install/swipl/bin/swipl-win.exe should work. Tested on
Windows 7 (64-bit).
