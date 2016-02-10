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
    void setPointStyle(QCPScatterStyle::ScatterShape shape);

public:
    explicit MainWindow(QWidget *parent = 0);
    QTextEdit *Sortie_application();
    ~MainWindow();

private slots:
    // File menu
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSave_as_triggered();
    void on_actionTikZ_for_LaTeX_triggered();
    void on_actionPNG_Image_triggered();
    void on_actionJPG_Image_triggered();
    void on_actionPDF_File_triggered();

    // Context menu
    void contextMenuRequest(QPoint pos);
    void setPointStyleCross();
    void setPointStylePlus();
    void setPointStyleCircle();
    void setPointStyleDisc();
    void setPointStyleSquare();
    void setPointStyleDiamond();
    void setPointStyleTriangle();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
