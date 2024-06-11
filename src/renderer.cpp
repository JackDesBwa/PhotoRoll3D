#include "renderer.h"
#include "general_shader_frag.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOffscreenSurface>
#include <QWindow>

Renderer::Renderer(QOffscreenSurface *offscreenSurface) :
    m_offscreenSurface(offscreenSurface),
    m_context(nullptr),
    m_program(nullptr),
    m_vbo(nullptr),
    m_vao(nullptr),
    m_matrixLoc(0) {
}

Renderer::~Renderer() {
    // Use a temporary offscreen surface to do the cleanup.
    // There may not be a native window surface available anymore at this stage.
    m_context->makeCurrent(m_offscreenSurface);

    delete m_program;
    delete m_vbo;
    delete m_vao;

    m_context->doneCurrent();
    delete m_context;
}

void Renderer::init(QWindow *w, QOpenGLContext *share) {
    m_context = new QOpenGLContext;
    m_context->setShareContext(share);
    m_context->setFormat(w->requestedFormat());
    m_context->create();
    if (!m_context->makeCurrent(w))
        return;

    QOpenGLFunctions *f = m_context->functions();
    f->glClearColor(0.0f, 0.1f, 0.25f, 1.0f);
    f->glViewport(0, 0, w->width() * w->devicePixelRatio(), w->height() * w->devicePixelRatio());

    m_program = new QOpenGLShaderProgram();
    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, R"(
        attribute highp vec4 vertex;
        attribute lowp vec2 coord;
        varying lowp vec2 v_coord;
        uniform highp mat4 matrix;
        void main() {
           v_coord = coord;
           gl_Position = matrix * vertex;
        }
    )");
    m_program->addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, general_shader_frag);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("coord", 1);
    m_program->link();
    m_matrixLoc = m_program->uniformLocation("matrix");
    m_modeLoc = m_program->uniformLocation("mode");

    m_vao = new QOpenGLVertexArrayObject;
    m_vao->create();
    QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);

    m_vbo = new QOpenGLBuffer;
    m_vbo->create();
    m_vbo->bind();

    std::array<GLfloat, 3*6> vertexCoords = {
        -1,+1,-1, +1,+1,-1, +1,-1,-1,
        -1,+1,-1, +1,-1,-1, -1,-1,-1,
    };
    std::array<GLfloat, 2*6> uvCoords = {
        0,1, 1,1, 1,0,
        0,1, 1,0, 0,0,
    };

    m_vbo->allocate(sizeof(GLfloat) * (vertexCoords.size() + uvCoords.size()));
    m_vbo->write(0, vertexCoords.data(), sizeof(GLfloat) * vertexCoords.size());
    m_vbo->write(sizeof(GLfloat) * vertexCoords.size(), uvCoords.data(), sizeof(GLfloat) * uvCoords.size());
    m_vbo->release();

    if (m_vao->isCreated())
        setupVertexAttribs();
}

void Renderer::resize(int w, int h) {
    m_proj.setToIdentity();
    m_proj.ortho(-1, 1, -1, 1, 0, 10);
    m_resized = true;
}

void Renderer::setupVertexAttribs() {
    m_vbo->bind();
    m_program->enableAttributeArray(0);
    m_program->enableAttributeArray(1);
    m_context->functions()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    m_context->functions()->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (const void *)(6 * 3 * sizeof(GLfloat)));
    m_vbo->release();
}

void Renderer::render(QWindow *w, QOpenGLContext *share, uint texture) {
    if (!m_context)
        init(w, share);

    if (!m_context->makeCurrent(w))
        return;

    QOpenGLFunctions *f = m_context->functions();
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_resized) {
        f->glViewport(0, 0, w->width() * w->devicePixelRatio(), w->height() * w->devicePixelRatio());
        m_resized = false;
    }

    if (texture) {
        f->glBindTexture(GL_TEXTURE_2D, texture);
        f->glFrontFace(GL_CW);
        f->glEnable(GL_CULL_FACE);
        f->glEnable(GL_DEPTH_TEST);

        m_program->bind();
        QOpenGLVertexArrayObject::Binder vaoBinder(m_vao);
        if (!m_vao->isCreated())
            setupVertexAttribs();

        m_program->setUniformValue(m_matrixLoc, m_proj);
        m_program->setUniformValue(m_modeLoc, m_displaymode);

        f->glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    m_context->swapBuffers(w);
}
