#ifndef APPINTERFACE_H
#define APPINTERFACE_H

#include <QObject>
#include "general_shader_frag.h"

class AppInterface : public QObject {
    Q_OBJECT
    Q_ENUM(DisplayMode)

    Q_PROPERTY(qreal factor READ factor NOTIFY factorChanged FINAL)
    Q_PROPERTY(DisplayMode displayMode READ displayMode WRITE setDisplayMode NOTIFY displayModeChanged FINAL)

    public:
        explicit AppInterface(QObject *parent = nullptr);

        void setSize(int w, int h);
        qreal factor() const { return m_factor; }
        const DisplayMode displayMode() const { return m_displaymode; }

    public slots:
        void setDisplayMode(DisplayMode mode);

    signals:
        void factorChanged(qreal);
        void displayModeChanged(DisplayMode mode);

    private:
        qreal m_factor = 1;
        DisplayMode m_displaymode;
};

#endif // APPINTERFACE_H
