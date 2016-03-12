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
    void show_stats();
    void hide_stats();

public:
    explicit MainWindow(QWidget *parent = 0);
    QTextEdit *Sortie_application();
    ~MainWindow();

private slots:
    // File menu
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionSave_as_triggered();
    void on_actionReload_file_triggered();
    void on_actionTikZ_for_LaTeX_triggered();
    void on_actionPNG_Image_triggered();
    void on_actionJPG_Image_triggered();
    void on_actionPDF_File_triggered();

    // View menu
    void on_actionGrid_changed();
    void on_actionAxes_changed();
    void on_actionLabels_changed();

    //About menu
    void on_actionLicense_triggered();
    void on_actionHowToUse_triggered();
    void on_actionCredits_triggered();

    // Context menu
    void contextMenuRequest(QPoint pos);
    void setPointStyleCross();
    void setPointStylePlus();
    void setPointStyleDisc();
    void setPointStyleSquare();
    void setPointStyleDiamond();
    void setPointStyleTriangle();

    //Selection changes
    void update_visualization_panel();
    void show_hypervolumen();

private:
    Ui::MainWindow *ui;
    std::string     file_path_;
};

#endif // MAINWINDOW_H
