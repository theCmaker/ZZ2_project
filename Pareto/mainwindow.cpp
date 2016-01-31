#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    s(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Visualisation de Front de Pareto");
    ui->Graphique->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);
    ui->actionClose->setDisabled(true);
    ui->actionSave_as->setDisabled(true);
    ui->actionTikZ_for_LaTeX->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//------------------------------------------------------------------------
// composant de la sortie
//------------------------------------------------------------------------
QTextEdit* MainWindow::Sortie_application()
{
    return ui->Console;
}

void MainWindow::on_actionOpen_triggered()
{
    //afficher la dialog de openfile
    QString f_nom=QFileDialog::getOpenFileName(this,tr("Open file"),tr("."),tr("All files (*)"));

    if(!f_nom.isEmpty())
    {
        load_file(f_nom);
        ui->actionOpen->setDisabled(true);
        ui->actionClose->setDisabled(false);
        ui->actionSave_as->setDisabled(false);
        ui->actionTikZ_for_LaTeX->setDisabled(false);
    }
}

void MainWindow::on_actionClose_triggered()
{
    if (s != nullptr) {
        ui->Graphique->xAxis->setLabel(nullptr);
        ui->Graphique->yAxis->setLabel(nullptr);
        ui->Graphique->clearGraphs();
        ui->Graphique->clearItems();
        ui->Graphique->clearPlottables();
        ui->Graphique->clearFocus();
        ui->Graphique->setSolution(nullptr);
        ui->Graphique->replot();

        ui->Console->clear();
        delete s;
        s = nullptr;
        ui->actionClose->setDisabled(true);
        ui->actionSave_as->setDisabled(true);
        ui->actionOpen->setDisabled(false);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Save as"),tr("."),tr("All files (*)"));
    if (!f_nom.isEmpty())
    {
        s->saveToFile(f_nom.toStdString().c_str());
    }
}

void MainWindow::on_actionTikZ_for_LaTeX_triggered()
{
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Export to TikZ file"),tr("."),tr("LaTeX Source Files (*.tex)"));
    if (!f_nom.isEmpty()) {
        compute_style();
        s->exportToTikZ(f_nom.toStdString().c_str());
    }
}

void MainWindow::on_actionPNG_Image_triggered()
{
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Export to PNG Image"),tr("."),tr("PNG Files (*.png)"));
    if (!f_nom.isEmpty()) {
        ui->Graphique->savePng(f_nom,400,400,5);
    }
}

void MainWindow::load_file(QString f_nom) {
    s = new Solutions(f_nom.toStdString().c_str());
    ui->Graphique->setSolution(s);

    // Pareto Frontier
    QColor f_color = Qt::darkRed;
    s->compute_frontiers();
    ParetoFrontv pareto = s->getPFrontiers();
    int factor;

    QVector<QVector<double> > pareto_x(pareto.size()), pareto_y(pareto.size());
    for (unsigned i = 0; i < pareto.size(); ++i) {
        for (ParetoFront::iterator pitr = pareto[i].begin(); pitr != pareto[i].end(); ++pitr) {
            pareto_x[i].append((*pitr)->getX());
            pareto_y[i].append((*pitr)->getY());
        }
    }
    for (unsigned i = 0; i < pareto.size(); ++i) {
        factor = (225 * i)/pareto.size() + 100;
        ui->Graphique->addGraph();
        ui->Graphique->graph(i)->setData(pareto_x[i],pareto_y[i]);
        ui->Graphique->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, f_color.lighter(factor), f_color.lighter(factor), 10));
        ui->Graphique->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->Graphique->graph(i)->setPen(QPen(f_color.lighter(factor)));
    }

    // set axes
    ui->Graphique->xAxis->setLabel(s->getAbscissa().c_str());
    ui->Graphique->yAxis->setLabel(s->getOrdinate().c_str());
    ui->Graphique->xAxis->setRange(s->getMinX()-.05*(s->getMaxX()-s->getMinX()), s->getMaxX()+.05*(s->getMaxX()-s->getMinX()));
    ui->Graphique->yAxis->setRange(s->getMinY()-.05*(s->getMaxY()-s->getMinY()), s->getMaxY()+.05*(s->getMaxY()-s->getMinY()));
    ui->Graphique->replot();
}

void MainWindow::compute_style() {
    s->getStyle().setX_step(ui->Graphique->xAxis->tickStep());
    s->getStyle().setY_step(ui->Graphique->yAxis->tickStep());
    s->getStyle().setX_min(ui->Graphique->xAxis->pixelToCoord(ui->Graphique->axisRect()->rect().bottomLeft().x()));
    s->getStyle().setX_max(ui->Graphique->xAxis->pixelToCoord(ui->Graphique->axisRect()->rect().bottomRight().x()));
    s->getStyle().setY_max(ui->Graphique->yAxis->pixelToCoord(ui->Graphique->axisRect()->rect().topLeft().y()));
    s->getStyle().setY_min(ui->Graphique->yAxis->pixelToCoord(ui->Graphique->axisRect()->rect().bottomLeft().y()));
    s->getStyle().setX_ticks(ui->Graphique->xAxis->tickVector().toStdVector());
    std::vector<QString> labels = ui->Graphique->xAxis->tickVectorLabels().toStdVector();
    std::vector<std::string> stdlabels = std::vector<std::string>();
    for (auto & i : labels) {
        stdlabels.push_back(i.toStdString());
    }
    s->getStyle().setX_ticks_labels(stdlabels);
    s->getStyle().setY_ticks(ui->Graphique->yAxis->tickVector().toStdVector());
    labels = ui->Graphique->yAxis->tickVectorLabels().toStdVector();
    stdlabels.clear();
    for (auto & i : labels) {
        stdlabels.push_back(i.toStdString());
    }
    s->getStyle().setY_ticks_labels(stdlabels);
}
