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
#ifndef MASM_EXECUTABLEALLOCATOR_H
#define MASM_EXECUTABLEALLOCATOR_H

#include <RefPtr.h>
#include <RefCounted.h>
#include <wtf/PageBlock.h>

#include <private/qv4executableallocator_p.h>

#if OS(WINDOWS)
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

namespace JSC {

class JSGlobalData;

struct ExecutableMemoryHandle : public RefCounted<ExecutableMemoryHandle> {
    ExecutableMemoryHandle(QQmlJS::VM::ExecutableAllocator *allocator, int size)
        : m_allocator(allocator)
        , m_size(size)
    {
        m_allocation = allocator->allocate(size);
    }
    ~ExecutableMemoryHandle()
    {
        m_allocator->free(m_allocation);
    }

    inline void shrink(size_t) {
        // ### TODO.
    }

    inline bool isManaged() const { return true; }

    void* start() { return m_allocation->start(); }
    int sizeInBytes() { return m_size; }

    QQmlJS::VM::ExecutableAllocator *m_allocator;
    QQmlJS::VM::ExecutableAllocator::Allocation *m_allocation;
    int m_size;
};

struct ExecutableAllocator {
    ExecutableAllocator(QQmlJS::VM::ExecutableAllocator *alloc)
        : realAllocator(alloc)
    {}

    PassRefPtr<ExecutableMemoryHandle> allocate(JSGlobalData&, int size, void*, int)
    {
        return adoptRef(new ExecutableMemoryHandle(realAllocator, size));
    }

    static void makeWritable(void*, int)
    {
    }

    static void makeExecutable(void* addr, int size)
    {
        size_t pageSize = WTF::pageSize();
        size_t iaddr = reinterpret_cast<size_t>(addr);
        size_t roundAddr = iaddr & ~(pageSize - static_cast<size_t>(1));
#if OS(WINDOWS)
        DWORD oldProtect;
        VirtualProtect(reinterpret_cast<void*>(roundAddr), size + (iaddr - roundAddr), PAGE_EXECUTE_READWRITE, &oldProtect);
#else
        int mode = PROT_READ | PROT_WRITE | PROT_EXEC;
        mprotect(reinterpret_cast<void*>(roundAddr), size + (iaddr - roundAddr), mode);
#endif
    }

    QQmlJS::VM::ExecutableAllocator *realAllocator;
};

}

#endif // MASM_EXECUTABLEALLOCATOR_H
