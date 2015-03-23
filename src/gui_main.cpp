#include "hegui.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  HeGui hegui;
  hegui.show();
  //hegui.resize(800, hegui.height());

  return a.exec();
}
