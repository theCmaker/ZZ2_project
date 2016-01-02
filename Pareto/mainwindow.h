#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <iostream>
#include "Solutions.h"
#include "qcustomplot.h"
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

public:
    explicit MainWindow(QWidget *parent = 0);
    QTextEdit *Sortie_application();
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
