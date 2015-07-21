/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt SceneGraph Raster Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/
#include "pixmaptexture.h"

PixmapTexture::PixmapTexture(const QImage &image)
    // Prevent pixmap format conversion to reduce memory consumption
    // and surprises in calling code. (See QTBUG-47328)
    : m_pixmap(QPixmap::fromImage(image, Qt::NoFormatConversion))
{
}

PixmapTexture::PixmapTexture(const QPixmap &pixmap)
    : m_pixmap(pixmap)
{
}


int PixmapTexture::textureId() const
{
    return 0;
}

QSize PixmapTexture::textureSize() const
{
    return m_pixmap.size();
}

bool PixmapTexture::hasAlphaChannel() const
{
    return m_pixmap.hasAlphaChannel();
}

bool PixmapTexture::hasMipmaps() const
{
    return false;
}

void PixmapTexture::bind()
{
    Q_UNREACHABLE();
}
