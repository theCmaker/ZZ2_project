#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*!
 * \file mainwindow.h
 * \brief Main window of ParetoFrontViewer
 * \author Pierre-Loup Pissavy
 */

#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include "Solutions.h"
#include "qcp_hover.h"
#include <QtWidgets/QTextEdit>

namespace Ui {
class MainWindow;
}

/*!
 * \brief Application main window
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    Solutions *s;               //!< Solutions set
    void load_file(QString);    //!< Load a file into the solutions set
    void compute_style();       //!< Compute the style of the solutions set for TikZ output
    void show_stats();          //!< Show left panel content
    void hide_stats();          //!< Hide left panel content

public:
    explicit MainWindow(QWidget *parent = 0); //!< Constructor
    QTextEdit *Sortie_application();          //!< Console panel
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
    void setPointStyle(QCPScatterStyle::ScatterShape shape);
};

#endif // MAINWINDOW_H
