#ifndef WINDOW_H
#define WINDOW_H

#include <QWindow>
#include <QMatrix4x4>
#include <QTimer>
#include "appinterface.h"

QT_BEGIN_NAMESPACE
class QOpenGLContext;
class QOpenGLTexture;
class QOffscreenSurface;
class QQuickRenderControl;
class QQuickWindow;
class QQmlEngine;
class QQmlComponent;
class QQuickItem;
QT_END_NAMESPACE

class Renderer;

class Window : public QWindow {
    Q_OBJECT

    public:
        Window();
        ~Window();

    protected:
        void exposeEvent(QExposeEvent *e) override;
        void resizeEvent(QResizeEvent *e) override;
        void mouseDoubleClickEvent(QMouseEvent *ev) override;
        void mouseMoveEvent(QMouseEvent *ev) override;
        void mousePressEvent(QMouseEvent *ev) override;
        void mouseReleaseEvent(QMouseEvent *ev) override;
        void keyPressEvent(QKeyEvent *e) override;
        void keyReleaseEvent(QKeyEvent *e) override;

    private slots:
        void run();

        void createTexture();
        void destroyTexture();
        void render();
        void requestUpdate();
        void handleScreenChange();

    private:
        void startQuick(const QString &filename);
        void updateSizes();
        void resizeTexture();
        QMouseEvent remapMouse(const QMouseEvent *e);

        QOpenGLContext *m_context;
        QOffscreenSurface *m_offscreenSurface;
        QQuickRenderControl *m_renderControl;
        QQuickWindow *m_quickWindow;
        AppInterface *m_appinterface;
        QQmlEngine *m_qmlEngine;
        QQmlComponent *m_qmlComponent;
        QQuickItem *m_rootItem;
        uint m_textureId;
        QSize m_textureSize;
        bool m_quickInitialized;
        bool m_quickReady;
        QTimer m_updateTimer;
        Renderer *m_renderer;
        qreal m_dpr;
};

#endif
