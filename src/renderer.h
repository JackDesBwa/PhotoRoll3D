#ifndef RENDERER_H
#define RENDERER_H

#include <QMatrix4x4>
#include "general_shader_frag.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLBuffer)
QT_FORWARD_DECLARE_CLASS(QOpenGLVertexArrayObject)
QT_FORWARD_DECLARE_CLASS(QWindow)
QT_FORWARD_DECLARE_CLASS(QOffscreenSurface)

class Renderer {
    public:
        Renderer(QOffscreenSurface *offscreenSurface);
        ~Renderer();

        void resize(int w, int h);
        void changeDisplayMode(DisplayMode mode) { m_displaymode = int(mode); }
        void render(QWindow *w, QOpenGLContext *share, uint texture);

    private:
        void init(QWindow *w, QOpenGLContext *share);
        void setupVertexAttribs();

        QOffscreenSurface *m_offscreenSurface;
        QOpenGLContext *m_context;
        QOpenGLShaderProgram *m_program;
        QOpenGLBuffer *m_vbo;
        QOpenGLVertexArrayObject *m_vao;
        int m_matrixLoc;
        int m_modeLoc;
        QMatrix4x4 m_proj;
        int m_displaymode;
        bool m_resized;
};

#endif
