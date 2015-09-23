#include <GL/glew.h>
#include "Source\editor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QSurfaceFormat format;
	format.setDepthBufferSize(24);
	format.setStencilBufferSize(8);
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	//format.setDoubleBuffer(true);
	//here qt will set this as the default format for the rest of the app
	//and will use it to implicitly initialize any object that requires one
	QSurfaceFormat::setDefaultFormat(format);

	QApplication a(argc, argv);
	Editor w;
	w.show();
	return a.exec();
}
