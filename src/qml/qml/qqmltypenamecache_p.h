/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQMLTYPENAMECACHE_P_H
#define QQMLTYPENAMECACHE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qqmlrefcount_p.h>
#include "qqmlcleanup_p.h"
#include "qqmlmetatype_p.h"

#include <private/qhashedstring_p.h>

#include <QtCore/qvector.h>

QT_BEGIN_NAMESPACE

class QQmlType;
class QQmlEngine;
class QQmlTypeNameCache : public QQmlRefCount
{
public:
    QQmlTypeNameCache();
    virtual ~QQmlTypeNameCache();

    inline bool isEmpty() const;

    void add(const QHashedString &name, int sciptIndex = -1, const QHashedString &nameSpace = QHashedString());
    void addSingletonType(const QHashedString &name, QQmlMetaType::SingletonInstance *apiInstance, const QHashedString &nameSpace = QHashedString());

    struct Result {
        inline Result();
        inline Result(const void *importNamespace);
        inline Result(QQmlType *type);
        inline Result(int scriptIndex);
        inline Result(const Result &);

        inline bool isValid() const;

        QQmlType *type;
        const void *importNamespace;
        int scriptIndex;
    };
    Result query(const QHashedStringRef &);
    Result query(const QHashedStringRef &, const void *importNamespace);
    Result query(const QHashedV8String &);
    Result query(const QHashedV8String &, const void *importNamespace);
    QQmlMetaType::SingletonInstance *singletonType(const void *importNamespace);

private:
    friend class QQmlImports;

    struct Import {
        inline Import();
        // Imported module
        QQmlMetaType::SingletonInstance *singletonType;
        QVector<QQmlTypeModuleVersion> modules;

        // Or, imported script
        int scriptIndex;
    };

    template<typename Key>
    Result query(const QStringHash<Import> &imports, Key key)
    {
        Import *i = imports.value(key);
        if (i) {
            if (i->scriptIndex != -1) {
                return Result(i->scriptIndex);
            } else {
                return Result(static_cast<const void *>(i));
            }
        }

        return Result();
    }

    template<typename Key>
    Result typeSearch(const QVector<QQmlTypeModuleVersion> &modules, Key key)
    {
        QVector<QQmlTypeModuleVersion>::const_iterator end = modules.constEnd();
        for (QVector<QQmlTypeModuleVersion>::const_iterator it = modules.constBegin(); it != end; ++it) {
            if (QQmlType *type = it->type(key))
                return Result(type);
        }

        return Result();
    }

    QStringHash<Import> m_namedImports;
    QMap<const Import *, QStringHash<Import> > m_namespacedImports;
    QVector<QQmlTypeModuleVersion> m_anonymousImports;

    QQmlEngine *engine;
};

QQmlTypeNameCache::Result::Result()
: type(0), importNamespace(0), scriptIndex(-1)
{
}

QQmlTypeNameCache::Result::Result(const void *importNamespace)
: type(0), importNamespace(importNamespace), scriptIndex(-1)
{
}

QQmlTypeNameCache::Result::Result(QQmlType *type)
: type(type), importNamespace(0), scriptIndex(-1)
{
}

QQmlTypeNameCache::Result::Result(int scriptIndex)
: type(0), importNamespace(0), scriptIndex(scriptIndex)
{
}

QQmlTypeNameCache::Result::Result(const Result &o)
: type(o.type), importNamespace(o.importNamespace), scriptIndex(o.scriptIndex)
{
}

bool QQmlTypeNameCache::Result::isValid() const
{
    return type || importNamespace || scriptIndex != -1;
}

QQmlTypeNameCache::Import::Import()
: singletonType(0), scriptIndex(-1)
{
}

bool QQmlTypeNameCache::isEmpty() const
{
    return m_namedImports.isEmpty() && m_anonymousImports.isEmpty();
}

QT_END_NAMESPACE

#endif // QQMLTYPENAMECACHE_P_H

