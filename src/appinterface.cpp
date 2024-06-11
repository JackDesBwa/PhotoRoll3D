#include "appinterface.h"

AppInterface::AppInterface(QObject *parent) : QObject(parent) {
}

void AppInterface::setSize(int w, int h) {
    qreal factor = w / 800.0f;
    if (m_factor != factor) {
        m_factor = factor;
        emit factorChanged(factor);
    }
}

void AppInterface::setDisplayMode(DisplayMode mode) {
    if (m_displaymode != mode) {
        m_displaymode = mode;
        emit displayModeChanged(mode);
    }
}
