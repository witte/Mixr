/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls 2 module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


import QtQuick 2.7
import QtQuick.Templates 2.0 as T


T.Slider {
    id: control
    anchors.horizontalCenter: parent.horizontalCenter
    height: 120
    wheelEnabled: true
    orientation: Qt.Vertical

    from: 0
    to: 1
    value: 1

    implicitWidth: 40
    implicitHeight: 120


    stepSize: 0.05

    padding: 6

    //! [handle]
    handle: Rectangle {
        x: control.leftPadding + (horizontal ? control.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.visualPosition * (control.availableHeight - height))
        implicitWidth: 24
        implicitHeight: 8
        radius: 2
        color: "#f8f8f8"
        border.width: 1
        border.color: "#fb47494a"

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }
    //! [handle]

    //! [background]
    background: Rectangle {
        x: control.leftPadding + (horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: horizontal ? 200 : 6
        implicitHeight: horizontal ? 6 : 120
        width: implicitWidth
        height: control.availableHeight
        radius: 3
        opacity: control.enabled ? 1 : 0.3
        color: "#e0e0e0"

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }
    //! [background]
}

