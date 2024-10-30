#include <QApplication>
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    widget w;

    // 设置应用程序图标
    QIcon appIcon(":/icons/Fa-Team-Fontawesome-FontAwesome-File-Waveform.ico");
    a.setWindowIcon(appIcon);

    w.setFixedSize(w.sizeHint());//维持窗口默认大小，不可拉伸
    w.setWindowTitle("PingWave");
    w.setWindowFlags(w.windowFlags() & ~Qt::WindowMaximizeButtonHint);//使用最大化按钮变灰色，不可点击
    w.show();
    return a.exec();
}
