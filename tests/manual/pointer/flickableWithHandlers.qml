/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the manual tests of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.7
import Qt.labs.handlers 1.0
import "qrc:/quick/shared/" as Examples
import "content"

Rectangle {
    id: root
    width: 400
    height: 400
    objectName: "root"
    color: "#222222"

    Flickable {
        anchors.fill: parent
        anchors.margins: 10
        anchors.topMargin: 40
        contentHeight: 600
        contentWidth: 600
        pressDelay: pressDelayCB.checked ? 1000 : 0

        Column {
            spacing: 6
            Rectangle {
                radius: 5
                width: parent.width - 12
                height: pressDelayCB.implicitHeight + 12
                x: 6
                color: "lightgray"
                Examples.CheckBox {
                    x: 6; y: 6
                    id: pressDelayCB
                    text: "press delay"
                }
            }


            Row {
                spacing: 6
                Slider {
                    label: "DragHandler"
                    value: 49; width: 100; height: 400
                }
                MouseAreaSlider {
                    label: "MouseArea"
                    value: 49; width: 100; height: 400
                }
                Column {
                    spacing: 6
                    MouseAreaButton {
                        text: "MouseArea"
                    }
                    MptaButton {
                        text: "MultiPointTouchArea"
                    }
                    MptaButton {
                        text: "MultiPointTouchArea"
                    }
                    TapHandlerButton {
                        text: "TapHandler"
                    }
                    TapHandlerButton {
                        text: "TapHandler"
                    }
                }
            }
        }
    }
}
