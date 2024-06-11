#include <QGuiApplication>
#include "window.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    Window window;
    window.resize(1024, 768);
    window.setTitle("PhotoRoll3D");
    window.show();

    return app.exec();
}
