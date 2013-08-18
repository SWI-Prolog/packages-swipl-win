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

#ifndef SWIPL_WIN_H
#define SWIPL_WIN_H

#include <QApplication>
#include "pqMainWindow.h"

/** subclassing required to handle QFileOpenEvent on Mac OSX
  */
class swipl_win : public QApplication
{
    Q_OBJECT
public:

    explicit swipl_win(int& argc, char **argv);
    
signals:
    
public slots:

protected:

    /** handle the QFileOpenEvent event */
    virtual bool event(QEvent *);

    pqMainWindow *w;
};

#endif // SWIPL_WIN_H
