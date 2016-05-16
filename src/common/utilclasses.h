// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
/*
 *   This program is free software; you can redistribute it and/or
 *   modify
 *   it under the terms of the GNU Lesser General Public License as
 *   published by the Free Software Foundation; either version 3.0 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 *   02111-1307 USA
 *
 *   This file is part of the Open-HMI Tester,
 *   http://openhmitester.sourceforge.net
 *
 */
// -*- mode: c++; c-basic-offset: 4; c-basic-style: bsd; -*-
#ifndef UTILCLASSES_H
#define UTILCLASSES_H

#include <QByteArray>
#include <QString>

/// ///
///
/// Circular Byte Array
///
/// ///

class QCircularByteArray_ : public QByteArray
{
        public:
    QCircularByteArray_(){}
    ~QCircularByteArray_(){}

    ///DATA METHODS
    bool addData(QByteArray ba)
    {
        append(ba);
        return true;
    }

    ///STRING METHODS
    QString getOneString()
    {
        return getOneString('\0');
    }

    QString getOneString(char separator)
    {
        //buscamos la primera aparición de separator
        int index = deepIndexOf(separator);
        //caso de que no se encuentre
        if (index == -1)
        {
            return "";
        }

        int resultSize = index + 1;
        //devolvemos el trozo correspondiente
        //eliminandlo del buffer original
        return getOneString(resultSize);
    }

    QString getOneString(int size)
    {
        //comprobacion inicial
        if (QByteArray::size() < size) return "";
        //cortamos el trozo correspondiente
        QByteArray result = left(size);
        //eliminamos ese trozo del buffer_
        remove(0,size);
        //devolvemos convertido en cadena
        return QString(result);
    }

    QString getAllCompleteStrings()
    {
        return getAllCompleteStrings('\0');
    }

    QString getAllCompleteStrings(char separator)
    {
        //buscamos la ultima aparición de separator
        int index = deepLastIndexOf(separator);
        //caso de que no se encuentre
        if (index == -1) return "";

        int resultSize = index + 1;
        //devolvemos el trozo correspondiente
        //eliminandlo del buffer original
        return getOneString(resultSize);
    }

    QString toString()
    {
        QString res(data());
        return res;
    }

    int deepIndexOf(char sep)
    {
        for (int i=0; i < size(); i++)
        {
            //qDebug() << "buffer[" << i << "]=" << at(i);
            if (at(i) == sep) return i;
        }
        return -1;
    }

    int deepLastIndexOf(char sep)
    {
        int index = -1;
        for (int i=0; i < size(); i++)
        {
            if (at(i) == sep) index = i;
        }
        return index;
    }



        private:

};


/// ///
///
///
///
/// ///

#endif // UTILCLASSES_H
