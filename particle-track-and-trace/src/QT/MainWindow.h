#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../advection/UVGrid.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget* parent = 0);
  ~MainWindow();

private slots:
  void on_FirstButton_clicked(bool checked);
  void on_SecondButton_clicked(bool checked);
  void on_ComplementaryButton_clicked(bool checked);
  void on_ContrastingButton_clicked(bool checked);
  void on_MonochromaticButton_clicked(bool checked);
  void on_SaturateButton_clicked(bool checked);
  void on_DesaturateButton_clicked(bool checked);
  void on_CircleButton_clicked(bool checked);
  void on_TriangleButton_clicked(bool checked);
  void on_SquareButton_clicked(bool checked);
  void on_CrossButton_clicked(bool checked);
  void on_FullySampledButton_clicked(bool checked);
  void on_RegularlySubsampledButton_clicked(bool checked);
  void on_IregularlySubsampledButton_clicked(bool checked);

private:
  Ui::MainWindow* ui;

  void setupTechniques();
};

#endif
