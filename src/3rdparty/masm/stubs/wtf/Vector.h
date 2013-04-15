/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the V4VM module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <wtf/Assertions.h>
#include <wtf/NotFound.h>
#include <qalgorithms.h>

enum WTF_UnusedOverflowMode {
    UnsafeVectorOverflow
};

namespace WTF {

template <typename T, int capacity = 1, int overflowMode = UnsafeVectorOverflow>
class Vector : public std::vector<T> {
public:
    Vector() {}
    Vector(int initialSize) : std::vector<T>(initialSize) {}

    inline void append(const T& value)
    { this->push_back(value); }

    inline void append(const Vector<T>& vector)
    {
        this->insert(this->end(), vector.begin(), vector.end());
    }

    using std::vector<T>::insert;

    inline void reserveInitialCapacity(size_t size) { this->reserve(size); }

    inline void insert(size_t position, T value)
    { this->insert(this->begin() + position, value); }

    inline void grow(size_t size)
    { this->resize(size); }

    inline void shrink(size_t size)
    { this->erase(this->begin() + size, this->end()); }

    inline void shrinkToFit()
    { this->shrink_to_fit(); }

    inline void remove(size_t position)
    { this->erase(this->begin() + position); }

    inline bool isEmpty() const { return this->empty(); }

    inline T &last() { return *(this->begin() + this->size() - 1); }
};

template <typename T, int capacity>
void deleteAllValues(const Vector<T, capacity> &vector)
{
    qDeleteAll(vector);
}

}

using WTF::Vector;
using WTF::deleteAllValues;

#endif // VECTOR_H
