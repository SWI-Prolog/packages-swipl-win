/*
    pqConsole    : interfacing SWI-Prolog and Qt

    Author       : Carlo Capelli
    E-mail       : cc.carlo.cap@gmail.com
    Copyright (C): 2013, Carlo Capelli

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef FLUSHOUTPUTEVENTS_H
#define FLUSHOUTPUTEVENTS_H

#include "pqConsole_global.h"
#include <QElapsedTimer>
#include <QThread>
class ConsoleEdit;

/** factorize output flushing interface
 */
struct PQCONSOLESHARED_EXPORT FlushOutputEvents {

    FlushOutputEvents(ConsoleEdit *target = 0, int msec_delta_refresh = 10);
    void flush();

    ConsoleEdit *target;
    QElapsedTimer measure_calls;
    int msec_delta_refresh;
};

#endif // FLUSHOUTPUTEVENTS_H
