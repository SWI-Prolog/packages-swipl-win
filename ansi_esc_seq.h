/*  Part of SWI-Prolog interface to Qt

    Author:        Carlo Capelli
    E-mail:        cc.carlo.cap@gmail.com
    Copyright (c)  2019, Carlo Capelli
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in
       the documentation and/or other materials provided with the
       distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
    POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef ANSI_ESC_SEQ_H
#define ANSI_ESC_SEQ_H

#include <QTextCharFormat>

// parse a subset of ANSI ESCAPE sequences
// just what's required by SWI-Prolog Qt console output
//
// from https://en.wikipedia.org/wiki/ANSI_escape_code:
//
// ANSI escape sequences are a standard for in-band signaling to control
// the cursor location, color, and other options
// on video text terminals and terminal emulators.
//
// Implementation of CSI would require to directly perform on QTextCursor
//  that's not implied in this fix to swipl-win. So, we're left with SGR.
// But I will consider it for application of multilevel DCG cursor.
//
class ANSI_ESC_SEQ
{
public:

    ANSI_ESC_SEQ(const QString src, QTextCharFormat &tcf);
    ~ANSI_ESC_SEQ();

    operator bool() const { return pos >= 0; }
    QString next();

private:

    // For CSI, or "Control Sequence Introducer" commands, the ESC [
    // is followed by any number (including none) of "parameter bytes" in the range 0x30-0x3F (ASCII 0-9:;<=>?),
    // then by any number of "intermediate bytes" in the range 0x20-0x2F (ASCII space and !"#$%&'()*+,-./),
    // then finally by a single "final byte" in the range 0x40-0x7E
    struct CSI {
        enum func {
            _,        // unknown
            CUU ='A', // Cursor Up	Moves the cursor n (default 1) cells in the given direction. If the cursor is already at the edge of the screen, this has no effect.
            CUD ='B', // Cursor Down
            CUF ='C', // Cursor Forward
            CUB ='D', // Cursor Back
            CNL ='E', // Cursor Next Line	Moves cursor to beginning of the line n (default 1) lines down. (not ANSI.SYS)
            CPL ='F', // Cursor Previous Line	Moves cursor to beginning of the line n (default 1) lines up. (not ANSI.SYS)
            CHA ='G', // Cursor Horizontal Absolute	Moves the cursor to column n (default 1). (not ANSI.SYS)
            CUP ='H', // Cursor Position	Moves the cursor to row n, column m. The values are 1-based, and default to 1 (top left corner) if omitted. A sequence such as CSI ;5H is a synonym for CSI 1;5H as well as CSI 17;H is the same as CSI 17H and CSI 17;1H
            ED  ='J', // Erase in Display	Clears part of the screen. If n is 0 (or missing), clear from cursor to end of screen. If n is 1, clear from cursor to beginning of the screen. If n is 2, clear entire screen (and moves cursor to upper left on DOS ANSI.SYS). If n is 3, clear entire screen and delete all lines saved in the scrollback buffer (this feature was added for xterm and is supported by other terminal applications).
            EL  ='K', // Erase in Line	Erases part of the line. If n is 0 (or missing), clear from cursor to the end of the line. If n is 1, clear from cursor to beginning of the line. If n is 2, clear entire line. Cursor position does not change.
            SU  ='S', // Scroll Up	Scroll whole page up by n (default 1) lines. New lines are added at the bottom. (not ANSI.SYS)
            SD  ='T', // Scroll Down	Scroll whole page down by n (default 1) lines. New lines are added at the top. (not ANSI.SYS)
            //HVP     // TBD --- Horizontal Vertical Position	Same as CUP
            SGR ='m', //- Select Graphic Rendition	Sets the appearance of the following characters, see SGR parameters below.
        } f;
        int n;
    };

    // SGR (Select Graphic Rendition) sets display attributes.
    // Several attributes can be set in the same sequence, separated by semicolons.
    // Each display attribute remains in effect until a following occurrence of SGR resets it.
    // If no codes are given, CSI m is treated as CSI 0 m (reset / normal).
    struct SGR {
        enum Formatting {
            Bold, Underline, Italic, Blink,
            _Formatting, // unknown
        } f;
        enum Color {
            Black, Red, Green, Yellow, Blue, Magenta, Cyan, White,
            _Color,      // unknown
        } fg, bg, bright_fg, bright_bg;
    };

    struct Out {
        QString out;
        SGR mode;
        void setStyle(QTextCharFormat &tcf) const;
    };

    const QString src;
    QTextCharFormat &tcf;
    int off = 0, pos;
};

#endif // ANSI_ESC_SEQ_H
