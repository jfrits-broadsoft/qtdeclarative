/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qquickabstractitemview_p.h"
#include "qquickabstractitemview_p_p.h"

QT_BEGIN_NAMESPACE

QQuickAbstractItemViewPrivate::QQuickAbstractItemViewPrivate()
    : transitioner(nullptr)
{
}

QQuickAbstractItemViewPrivate::~QQuickAbstractItemViewPrivate()
{
    if (transitioner)
        transitioner->setChangeListener(nullptr);
    delete transitioner;
}

void QQuickAbstractItemViewPrivate::createTransitioner()
{
    if (!transitioner) {
        transitioner = new QQuickItemViewTransitioner;
        transitioner->setChangeListener(this);
    }
}

QQuickAbstractItemView::QQuickAbstractItemView(QQuickFlickablePrivate &dd, QQuickItem *parent)
    : QQuickFlickable(dd, parent)
{
}

QQuickAbstractItemView::~QQuickAbstractItemView()
{
}

QQuickTransition *QQuickAbstractItemView::populateTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->populateTransition : 0;
}

void QQuickAbstractItemView::setPopulateTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->populateTransition != transition) {
        d->transitioner->populateTransition = transition;
        emit populateTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::addTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->addTransition : 0;
}

void QQuickAbstractItemView::setAddTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->addTransition != transition) {
        d->transitioner->addTransition = transition;
        emit addTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::addDisplacedTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->addDisplacedTransition : 0;
}

void QQuickAbstractItemView::setAddDisplacedTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->addDisplacedTransition != transition) {
        d->transitioner->addDisplacedTransition = transition;
        emit addDisplacedTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::moveTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->moveTransition : 0;
}

void QQuickAbstractItemView::setMoveTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->moveTransition != transition) {
        d->transitioner->moveTransition = transition;
        emit moveTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::moveDisplacedTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->moveDisplacedTransition : 0;
}

void QQuickAbstractItemView::setMoveDisplacedTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->moveDisplacedTransition != transition) {
        d->transitioner->moveDisplacedTransition = transition;
        emit moveDisplacedTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::removeTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->removeTransition : 0;
}

void QQuickAbstractItemView::setRemoveTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->removeTransition != transition) {
        d->transitioner->removeTransition = transition;
        emit removeTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::removeDisplacedTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->removeDisplacedTransition : 0;
}

void QQuickAbstractItemView::setRemoveDisplacedTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->removeDisplacedTransition != transition) {
        d->transitioner->removeDisplacedTransition = transition;
        emit removeDisplacedTransitionChanged();
    }
}

QQuickTransition *QQuickAbstractItemView::displacedTransition() const
{
    Q_D(const QQuickAbstractItemView);
    return d->transitioner ? d->transitioner->displacedTransition : 0;
}

void QQuickAbstractItemView::setDisplacedTransition(QQuickTransition *transition)
{
    Q_D(QQuickAbstractItemView);
    d->createTransitioner();
    if (d->transitioner->displacedTransition != transition) {
        d->transitioner->displacedTransition = transition;
        emit displacedTransitionChanged();
    }
}

QT_END_NAMESPACE