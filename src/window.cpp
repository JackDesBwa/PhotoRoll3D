#include "window.h"
#include "general_shader_frag.h"
#include "renderer.h"

#include <QQuickRenderControl>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQuickRenderTarget>
#include <QQuickGraphicsDevice>
#include <QOpenGLFramebufferObject>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOffscreenSurface>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>

class RenderControl : public QQuickRenderControl {
    public:
        RenderControl(QWindow *w) : m_window(w) { }
        QWindow *renderWindow(QPoint *offset) override;

    private:
        QWindow *m_window;
};

QWindow *RenderControl::renderWindow(QPoint *offset) {
    if (offset)
        *offset = QPoint(0, 0);
    return m_window;
}

Window::Window() :
    m_rootItem(nullptr),
    m_textureId(0),
    m_quickInitialized(false),
    m_quickReady(false),
    m_dpr(0) {

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    setSurfaceType(QSurface::OpenGLSurface);

    QSurfaceFormat format;
    format.setDepthBufferSize(16);
    format.setStencilBufferSize(8);
    setFormat(format);

    m_context = new QOpenGLContext;
    m_context->setFormat(format);
    m_context->create();

    m_offscreenSurface = new QOffscreenSurface;
    m_offscreenSurface->setFormat(m_context->format());
    m_offscreenSurface->create();

    m_renderer = new Renderer(m_offscreenSurface);

    m_renderControl = new RenderControl(this);
    m_quickWindow = new QQuickWindow(m_renderControl);

    m_appinterface = new AppInterface(this);

    auto displaymode = DisplayMode::A_RC_HC;
    m_appinterface->setDisplayMode(displaymode);
    m_renderer->changeDisplayMode(displaymode);

    m_qmlEngine = new QQmlEngine;
    if (!m_qmlEngine->incubationController())
        m_qmlEngine->setIncubationController(m_quickWindow->incubationController());
    m_qmlEngine->rootContext()->setContextProperty("app", m_appinterface);

    m_updateTimer.setSingleShot(true);
    m_updateTimer.setInterval(5);
    connect(&m_updateTimer, &QTimer::timeout, this, &Window::render);

    connect(m_quickWindow, &QQuickWindow::sceneGraphInitialized, this, &Window::createTexture);
    connect(m_quickWindow, &QQuickWindow::sceneGraphInvalidated, this, &Window::destroyTexture);
    connect(m_renderControl, &QQuickRenderControl::renderRequested, this, &Window::requestUpdate);
    connect(m_renderControl, &QQuickRenderControl::sceneChanged, this, &Window::requestUpdate);

    connect(m_appinterface, &AppInterface::displayModeChanged, this, [this](DisplayMode mode) {
        m_renderer->changeDisplayMode(mode);
        render();
    });

    connect(this, &QWindow::screenChanged, this, &Window::handleScreenChange);
}

Window::~Window() {
    m_context->makeCurrent(m_offscreenSurface);

    delete m_qmlComponent;
    delete m_qmlEngine;
    delete m_quickWindow;
    delete m_renderControl;

    if (m_textureId)
        m_context->functions()->glDeleteTextures(1, &m_textureId);

    m_context->doneCurrent();

    delete m_renderer;
    delete m_appinterface;

    delete m_offscreenSurface;
    delete m_context;
}

void Window::createTexture() {
    m_dpr = devicePixelRatio();
    QSize sz = size();
    sz.setWidth(sz.width() * 2);
    m_textureSize = sz * m_dpr;
    QOpenGLFunctions *f = m_context->functions();
    f->glGenTextures(1, &m_textureId);
    f->glBindTexture(GL_TEXTURE_2D, m_textureId);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_textureSize.width(), m_textureSize.height(), 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    m_quickWindow->setRenderTarget(QQuickRenderTarget::fromOpenGLTexture(m_textureId, m_textureSize));
}

void Window::destroyTexture() {
    m_context->functions()->glDeleteTextures(1, &m_textureId);
    m_textureId = 0;
}

void Window::render() {
    if (!m_context->makeCurrent(m_offscreenSurface))
        return;

    m_renderControl->beginFrame();
    m_renderControl->polishItems();
    m_renderControl->sync();
    m_renderControl->render();
    m_renderControl->endFrame();

    QOpenGLFramebufferObject::bindDefault();
    m_context->functions()->glFlush();

    m_quickReady = true;

    m_renderer->render(this, m_context, m_quickReady ? m_textureId : 0);
}

void Window::requestUpdate() {
    if (!m_updateTimer.isActive())
        m_updateTimer.start();
}

void Window::run() {
    disconnect(m_qmlComponent, &QQmlComponent::statusChanged, this, &Window::run);

    if (m_qmlComponent->isError()) {
        const QList<QQmlError> errorList = m_qmlComponent->errors();
        for (const QQmlError &error : errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    QObject *rootObject = m_qmlComponent->create();
    if (m_qmlComponent->isError()) {
        const QList<QQmlError> errorList = m_qmlComponent->errors();
        for (const QQmlError &error : errorList)
            qWarning() << error.url() << error.line() << error;
        return;
    }

    m_rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (!m_rootItem) {
        qWarning("run: Not a QQuickItem");
        delete rootObject;
        return;
    }

    m_rootItem->setParentItem(m_quickWindow->contentItem());

    updateSizes();

    m_rootItem->forceActiveFocus();

    m_context->makeCurrent(m_offscreenSurface);
    m_quickWindow->setGraphicsDevice(QQuickGraphicsDevice::fromOpenGLContext(m_context));
    m_renderControl->initialize();
    m_quickInitialized = true;
}

void Window::updateSizes() {
    m_appinterface->setSize(width(), height());
    m_quickWindow->setGeometry(0, 0, width(), height());
    m_rootItem->setWidth(width() * 2);
    m_rootItem->setHeight(height());
    m_renderer->resize(width(), height());
}

void Window::startQuick(const QString &filename) {
    m_qmlComponent = new QQmlComponent(m_qmlEngine, QUrl(filename));
    if (m_qmlComponent->isLoading())
        connect(m_qmlComponent, &QQmlComponent::statusChanged, this, &Window::run);
    else
        run();
}

void Window::exposeEvent(QExposeEvent *) {
    if (isExposed()) {
        if (!m_quickInitialized) {
            m_renderer->render(this, m_context, m_quickReady ? m_textureId : 0);
            startQuick(QStringLiteral("qrc:/qt/qml/pr3d/pages/home.qml"));
        }
    }
}

void Window::resizeTexture() {
    if (m_rootItem && m_context->makeCurrent(m_offscreenSurface)) {
        m_context->functions()->glDeleteTextures(1, &m_textureId);
        m_textureId = 0;
        createTexture();
        m_context->doneCurrent();
        updateSizes();
        render();
    }
}

void Window::resizeEvent(QResizeEvent *) {
    if (m_textureId && m_textureSize != size() * devicePixelRatio())
        resizeTexture();
}

void Window::handleScreenChange() {
    if (m_dpr != devicePixelRatio())
        resizeTexture();
}

QMouseEvent Window::remapMouse(const QMouseEvent *e) {
    QPointF lpos = e->position();
    QPointF gpos = e->globalPosition();
    auto displaymode = m_appinterface->displayMode();
    switch (displaymode) {
        case DisplayMode::SL:
        case DisplayMode::SR:
        case DisplayMode::IR1:
        case DisplayMode::IR2:
        case DisplayMode::IC1:
        case DisplayMode::IC2:
        case DisplayMode::CK1:
        case DisplayMode::CK2:
        case DisplayMode::A_RC_G:
        case DisplayMode::A_RC_HC:
        case DisplayMode::A_RC_FC:
        case DisplayMode::A_RC_D:
        case DisplayMode::A_YB_G:
        case DisplayMode::A_YB_HC:
        case DisplayMode::A_YB_FC:
        case DisplayMode::A_YB_D:
        case DisplayMode::A_GM_G:
        case DisplayMode::A_GM_HC:
        case DisplayMode::A_GM_FC:
        case DisplayMode::A_GM_D:
            break; // No change
    
        case DisplayMode::P:
        case DisplayMode::X:
        case DisplayMode::ML:
        case DisplayMode::MR:
        case DisplayMode::MB: {
            qreal w = width()/2;
            qreal h = height()/2;
            if ((displaymode == DisplayMode::ML || displaymode == DisplayMode::MB) && gpos.x() < w) {
                lpos.setX(w - lpos.x());
                gpos.setX(w - gpos.x());
            }
            if ((displaymode == DisplayMode::MR || displaymode == DisplayMode::MB) && gpos.x() >= w) {
                lpos.setX(3 * w - lpos.x());
                gpos.setX(3 * w - gpos.x());
            }
            lpos.setX(fmod(lpos.x(), w)*2);
            lpos.setY((lpos.y() - h/2) * 2);
            gpos.setX(fmod(gpos.x(), w)*2);
            gpos.setY((gpos.y() - h/2) * 2);
        } break;

        case DisplayMode::PA:
        case DisplayMode::XA: {
            qreal w = width()/2;
            lpos.setX(fmod(lpos.x(), w)*2);
            gpos.setX(fmod(gpos.x(), w)*2);
        } break;

        case DisplayMode::TB:
        case DisplayMode::BT: {
            qreal w = width()/2;
            qreal h = height()/2;
            lpos.setX((lpos.x() - w/2) * 2);
            lpos.setY(fmod(lpos.y(), h)*2);
            gpos.setX((lpos.x() - w/2) * 2);
            gpos.setY(fmod(lpos.y(), h)*2);
        } break;

        case DisplayMode::TBA:
        case DisplayMode::BTA: {
            qreal h = height()/2;
            lpos.setY(fmod(lpos.y(), h)*2);
            gpos.setY(fmod(gpos.y(), h)*2);
        } break;
    }
    return QMouseEvent(e->type(), lpos, gpos, e->button(), e->buttons(), e->modifiers());
}

void Window::mouseDoubleClickEvent(QMouseEvent *ev) {
    QMouseEvent mappedEvent = remapMouse(ev);
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}

void Window::mouseMoveEvent(QMouseEvent *ev) {
    QMouseEvent mappedEvent = remapMouse(ev);
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}

void Window::mousePressEvent(QMouseEvent *ev) {
    QMouseEvent mappedEvent = remapMouse(ev);
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}

void Window::mouseReleaseEvent(QMouseEvent *ev) {
    QMouseEvent mappedEvent = remapMouse(ev);
    QCoreApplication::sendEvent(m_quickWindow, &mappedEvent);
}

void Window::keyPressEvent(QKeyEvent *e) {
    QCoreApplication::sendEvent(m_quickWindow, e);
}

void Window::keyReleaseEvent(QKeyEvent *e) {
    QCoreApplication::sendEvent(m_quickWindow, e);
}
