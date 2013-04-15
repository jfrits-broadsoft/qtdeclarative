#ifndef QV4UNWINDHELPER_PDW2_H
#define QV4UNWINDHELPER_PDW2_H

#include "qv4unwindhelper_p.h"
#include "qv4functionobject_p.h"
#include <wtf/Platform.h>

#include <QMap>
#include <QMutex>

#define __USE_GNU
#include <dlfcn.h>

namespace QQmlJS {
namespace VM {

namespace {
#if CPU(X86_64)
// Generated by fdegen
static const unsigned char cie_fde_data[] = {
    0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0x0, 0x8, 0x78, 0x10, 0xc, 0x7, 0x8,
    0x90, 0x1, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
    0x18, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x41, 0x13, 0x7e, 0x86,
    0x2, 0x43, 0xd, 0x6, 0x8c, 0x3, 0x8e, 0x4,
    0x0, 0x0, 0x0, 0x0
};
static const int fde_offset = 20;
static const int initial_location_offset = 28;
static const int address_range_offset = 36;
#elif CPU(X86) && OS(LINUX)
static const unsigned char cie_fde_data[] = {
    0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x1, 0x0, 0x4, 0x7c, 0x8, 0xc, 0x4, 0x4,
    0x88, 0x1, 0x0, 0x0, 0x20, 0x0, 0x0, 0x0,
    0x18, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x41, 0x13, 0x7e, 0x85,
    0x2, 0x43, 0xd, 0x5, 0x86, 0x3, 0x87, 0x4,
    0x0, 0x0, 0x0, 0x0,
};
static const int fde_offset = 20;
static const int initial_location_offset = 28;
static const int address_range_offset = 32;
#endif
} // anonymous namespace

static QMutex functionProtector;
static QMap<quintptr, Function*> allFunctions;

static Function *lookupFunction(void *pc)
{
    quintptr key = reinterpret_cast<quintptr>(pc);
    QMap<quintptr, Function*>::ConstIterator it = allFunctions.lowerBound(key);
    if (it != allFunctions.begin() && allFunctions.count() > 0)
        --it;
    if (it == allFunctions.end())
        return 0;

    quintptr codeStart = reinterpret_cast<quintptr>((*it)->code);
    if (key < codeStart || key >= codeStart + (*it)->codeSize)
        return 0;
    return *it;
}

namespace {
void writeIntPtrValue(unsigned char *addr, intptr_t val)
{
    addr[0] = (val >>  0) & 0xff;
    addr[1] = (val >>  8) & 0xff;
    addr[2] = (val >> 16) & 0xff;
    addr[3] = (val >> 24) & 0xff;
#if QT_POINTER_SIZE == 8
    addr[4] = (val >> 32) & 0xff;
    addr[5] = (val >> 40) & 0xff;
    addr[6] = (val >> 48) & 0xff;
    addr[7] = (val >> 56) & 0xff;
#endif
}
} // anonymous namespace

static void ensureUnwindInfo(Function *f)
{
    if (!f->unwindInfo.isEmpty())
        return;
    QByteArray info;
    info.resize(sizeof(cie_fde_data));

    unsigned char *cie_and_fde = reinterpret_cast<unsigned char *>(info.data());
    memcpy(cie_and_fde, cie_fde_data, sizeof(cie_fde_data));

    intptr_t ptr = static_cast<char *>(f->codeRef.code().executableAddress()) - static_cast<char *>(0);
    writeIntPtrValue(cie_and_fde + initial_location_offset, ptr);

    writeIntPtrValue(cie_and_fde + address_range_offset, f->codeSize);

    f->unwindInfo = info;
}

#if defined(Q_OS_DARWIN)
extern "C" void __register_frame(void *fde);
extern "C" void __deregister_frame(void *fde);
#endif

static void registerFunctionUnlocked(Function *f)
{
    allFunctions.insert(reinterpret_cast<quintptr>(f->code), f);
#if defined(Q_OS_DARWIN)
    ensureUnwindInfo(f);
    __register_frame(f->unwindInfo.data() + fde_offset);
#endif
}

static void deregisterFunctionUnlocked(Function *f)
{
    allFunctions.remove(reinterpret_cast<quintptr>(f->code));
#if defined(Q_OS_DARWIN)
    if (!f->unwindInfo.isEmpty())
        __deregister_frame(f->unwindInfo.data() + fde_offset);
#endif
}

void UnwindHelper::registerFunction(Function *function)
{
    QMutexLocker locker(&functionProtector);
    registerFunctionUnlocked(function);
}

void UnwindHelper::registerFunctions(QVector<Function *> functions)
{
    QMutexLocker locker(&functionProtector);
    foreach (Function *f, functions)
        registerFunctionUnlocked(f);
}

void UnwindHelper::deregisterFunction(Function *function)
{
    QMutexLocker locker(&functionProtector);
    deregisterFunctionUnlocked(function);
}

void UnwindHelper::deregisterFunctions(QVector<Function *> functions)
{
    QMutexLocker locker(&functionProtector);
    foreach (Function *f, functions)
        deregisterFunctionUnlocked(f);
}

} // VM namespace
} // QQmlJS namespace

#if defined(Q_OS_LINUX)
extern "C" {

struct bases
{
    void *tbase;
    void *dbase;
    void *func;
};

Q_V4_EXPORT void *_Unwind_Find_FDE(void *pc, struct bases *bases)
{
    typedef void *(*Old_Unwind_Find_FDE)(void *pc, struct bases *bases);
    static Old_Unwind_Find_FDE oldFunction = 0;
    if (!oldFunction)
        oldFunction = (Old_Unwind_Find_FDE)dlsym(RTLD_NEXT, "_Unwind_Find_FDE");

    {
        QMutexLocker locker(&QQmlJS::VM::functionProtector);
        QQmlJS::VM::Function *function = QQmlJS::VM::lookupFunction(pc);
        if (function) {
            bases->tbase = 0;
            bases->dbase = 0;
            bases->func = reinterpret_cast<void*>(function->code);
            QQmlJS::VM::ensureUnwindInfo(function);
            return function->unwindInfo.data() + QQmlJS::VM::fde_offset;
        }
    }

    return oldFunction(pc, bases);
}

}
#endif

#endif // QV4UNWINDHELPER_PDW2_H
