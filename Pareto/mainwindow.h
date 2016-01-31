#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include "Solutions.h"
#include "qcp_hover.h"
#include <QtWidgets/QTextEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Solutions *s;

protected:
    void load_file(QString);
    void compute_style();

public:
    explicit MainWindow(QWidget *parent = 0);
    QTextEdit *Sortie_application();
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSave_as_triggered();
    void on_actionTikZ_for_LaTeX_triggered();
    void on_actionPNG_Image_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
