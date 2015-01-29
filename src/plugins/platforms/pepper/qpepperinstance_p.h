/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com <http://qt.digia.com/>
**
** This file is part of the Qt Native Client platform plugin.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com <http://qt.digia.com/>
**
****************************************************************************/

#ifndef QT_PEPPER_INSTANCE_IMPL_H
#define QT_PEPPER_INSTANCE_IMPL_H

#include "qpeppereventtranslator.h"
#include "qpepperhelpers.h"

#include <qpa/qplatformtheme.h>
#include <QtCore/qhash.h>

#include <ppapi/cpp/instance.h>
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/size.h"
#include "ppapi/cpp/graphics_2d.h"
#include "ppapi/cpp/image_data.h"
#include <ppapi/cpp/completion_callback.h>
#include "ppapi/cpp/view.h"
#include <ppapi/utility/completion_callback_factory.h>

Q_DECLARE_LOGGING_CATEGORY(QT_PLATFORM_PEPPER_INSTANCE)

class QPepperInstance;
class QPepperIntegration;
class QPepperInstancePrivate
{
public:
    QPepperInstancePrivate(QPepperInstance *instance);
    virtual ~QPepperInstancePrivate();
    static QPepperInstancePrivate *get();
    static QPepperInstance *getInstance();

    bool init(uint32_t argc, const char* argn[], const char* argv[]);
    void didChangeView(const pp::View &view);
    void didChangeFocus(bool hasFucus);
    bool handleInputEvent(const pp::InputEvent& event);
    bool handleDocumentLoad(const pp::URLLoader& url_loader);
    void handleMessage(const pp::Var& var_message);

    // Instance attribute getters
    QRect geometry();
    QRect deviceGeometry();
    qreal devicePixelRatio();
    qreal cssScale();

    // publics:
    void scheduleWindowSystemEventsFlush();
    void postMessage(const QByteArray &message);
    void runJavascript(const QByteArray &script);
    void registerMessageHandler(const QByteArray &messageTag, QObject *obj, const char *slot);
    QPlatformTheme::KeyboardSchemes keyboardScheme();

    // privates:
    void startQt();
    void windowSystemEventsFlushCallback(int32_t);
    void handleGetAppVersionMessage(const QByteArray &message);

    void flushCompletedCallback(int32_t);

    QPepperInstance *q;
    QPepperIntegration *m_pepperIntegraton;

    bool m_qtStarted;

    pp::Var m_console;
    pp::Rect m_currentGeometry;
    qreal m_currentCssScale;
    qreal m_currentDeviceScale;
    qreal m_currentDevicePixelRatio;

    pp::Graphics2D *m_context2D;
    pp::ImageData *m_imageData2D;
    QImage *m_frameBuffer;
    bool m_inFlush;
    QHash<QByteArray, QPair<QPointer<QObject>, const char *> >m_messageHandlers;

    pp::CompletionCallbackFactory<QPepperInstancePrivate> m_callbackFactory;
};

#endif