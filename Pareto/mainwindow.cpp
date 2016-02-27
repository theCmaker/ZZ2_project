#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QGraphicsScene>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    s(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ParetoFront Viewer");
    ui->Graphique->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);
    ui->Graphique->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->Graphique, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    connect(ui->Graphique,SIGNAL(selectionChangedByUser()), this, SLOT(update_visualization_panel()));

    ui->actionClose->setDisabled(true);
    ui->actionSave_as->setDisabled(true);
    ui->actionGNUPlot->setDisabled(true);
    ui->actionJPG_Image->setDisabled(true);
    ui->actionPNG_Image->setDisabled(true);
    ui->actionPDF_File->setDisabled(true);
    ui->actionTikZ_for_LaTeX->setDisabled(true);

    ui->LabelNoFrontSelected->setVisible(true);
    ui->LabelFront->setVisible(false);
    ui->LabelHypervolumen->setVisible(false);
    ui->ValueFront->setVisible(false);
    ui->ValueHypervolumen->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
    if (s != nullptr) {
        delete s;
    }
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
        //ui->actionGNUPlot->setDisabled(false);
        ui->actionJPG_Image->setDisabled(false);
        ui->actionPNG_Image->setDisabled(false);
        ui->actionPDF_File->setDisabled(false);
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
        ui->actionGNUPlot->setDisabled(true);
        ui->actionJPG_Image->setDisabled(true);
        ui->actionPNG_Image->setDisabled(true);
        ui->actionPDF_File->setDisabled(true);
        ui->actionTikZ_for_LaTeX->setDisabled(true);
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

void MainWindow::on_actionJPG_Image_triggered()
{
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Export to JPG Image"),tr("."),tr("JPG Files (*.jpg,*.jpeg)"));
    if (!f_nom.isEmpty()) {
        ui->Graphique->saveJpg(f_nom,400,400,5);
    }
}

void MainWindow::on_actionPDF_File_triggered()
{
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Export to PDF File"),tr("."),tr("PDF Files (*.pdf,*.xpdf)"));
    if (!f_nom.isEmpty()) {
        ui->Graphique->savePdf(f_nom);
    }
}

void MainWindow::load_file(QString f_nom) {
    s = new Solutions(f_nom.toStdString().c_str());
    ui->Graphique->setSolution(s);

    // Pareto Frontier
    QColor f_color = Qt::darkRed;
    s->compute_frontiers();
    ParetoFrontv & pareto = s->getPFrontiers();
    int factor;

    QVector<QVector<double> >   pareto_x(pareto.size()),
                                pareto_y(pareto.size());

    for (unsigned i = 0; i < pareto.size(); ++i) {
        for (PolyLine::iterator pitr = pareto[i].begin(); pitr != pareto[i].end(); ++pitr) {
            pareto_x[i].append((*pitr)->getX());
            pareto_y[i].append((*pitr)->getY());
        }
    }

    for (unsigned i = 0; i < pareto.size(); ++i) {
        factor = (225 * i)/pareto.size() + 100;
        ui->Graphique->addGraph();
        ui->Graphique->graph(i)->setData(pareto_x[i],pareto_y[i]);
        ui->Graphique->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, f_color.lighter(factor),QColor(255,255,255,0), 10));
        ui->Graphique->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->Graphique->graph(i)->setPen(QPen(f_color.lighter(factor)));

        s->compute_hypervolumen(pareto[i]);
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
    s->getStyle().setWidth(ui->Graphique->xAxis->range().size());
    s->getStyle().setHeight(ui->Graphique->yAxis->range().size());
    s->getStyle().setAspectRatio((double)ui->Graphique->height()/(double)ui->Graphique->width());
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
    s->compute_front_style(ui->Graphique);
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  if (ui->Graphique->selectedGraphs().size() == 1) {
    QMenu *menu = new QMenu(this);
    QMenu *menu_style = new QMenu("Style",menu);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu_style->setAttribute(Qt::WA_DeleteOnClose);

    menu_style->addAction("Cross", this, SLOT(setPointStyleCross()));
    menu_style->addAction("Plus", this, SLOT(setPointStylePlus()));
    menu_style->addAction("Circle", this, SLOT(setPointStyleCircle()));
    menu_style->addAction("Disc", this, SLOT(setPointStyleDisc()));
    menu_style->addAction("Square", this, SLOT(setPointStyleSquare()));
    menu_style->addAction("Diamond", this, SLOT(setPointStyleDiamond()));
    menu_style->addAction("Triangle", this, SLOT(setPointStyleTriangle()));
    menu->addMenu(menu_style);

    menu->popup(ui->Graphique->mapToGlobal(pos));
  }
}

void MainWindow::update_visualization_panel() {
    std::cout << __func__ << std::endl;
    if (ui->Graphique->selectedGraphs().size() == 1) {
      //Find the selected graph
      // itr;
      ParetoFrontv frontiers = s->getPFrontiers();
      bool found = false;
      unsigned index = 0;
      unsigned i;
      while (index < frontiers.size() && !found) {
        QMap<double,QCPData>::iterator itr = ui->Graphique->selectedGraphs()[0]->data()->begin();
        while (index < frontiers.size()
               && itr->key != frontiers[index].pts()[0]->getX()
               && itr->value != frontiers[index].pts()[0]->getY()) {
            //std::cout << "Tried to compare (" << itr->key << "," << itr->value << ") with " << *frontiers[index].pts()[0]  << std::endl;
          ++index;
        }
        if (index < frontiers.size()) {
            //std::cout << "The selected front could be " << index << std::endl;
            //std::cout << "Let's compare the points" << std::endl;
            //Here, the first point corresponds to a front
            //Let's compare the two sets of points
            i = 0;
            //std::cout << "Number of points in the front: " << frontiers[index].pts().size() << std::endl;
            while (itr != ui->Graphique->selectedGraphs()[0]->data()->end()
                   && i != frontiers[index].pts().size()
                   && itr->key == frontiers[index].pts()[i]->getX()
                   && itr->value == frontiers[index].pts()[i]->getY()) {
              //std::cout << "Selected (" << itr->key << "," << itr->value << ") with " << *frontiers[index].pts()[i] << std::endl;
              ++itr;
              ++i;
            }
            //At the end, if the number of matched points corresponds to the number of points in the front,
            //then it is the same
            if (i == frontiers[index].pts().size() && itr == ui->Graphique->selectedGraphs()[0]->data()->end()) {
              found = true;
            }
        }
      }
      if (found) {
        std::stringstream front_value;
        std::stringstream hypervolumen_value;

        front_value << index;
        hypervolumen_value << frontiers[index].hypervolumen();
        ui->ValueFront->setText(QString(front_value.str().c_str()));
        ui->ValueHypervolumen->setText(QString(hypervolumen_value.str().c_str()));
        ui->LabelFront->setVisible(true);
        ui->ValueFront->setVisible(true);
        ui->ValueHypervolumen->setVisible(true);
        ui->LabelHypervolumen->setVisible(true);
        ui->LabelNoFrontSelected->setVisible(false);
      }
    } else {
        ui->LabelNoFrontSelected->setVisible(true);
        ui->ValueFront->setVisible(false);
        ui->ValueHypervolumen->setVisible(false);
        ui->LabelFront->setVisible(false);
        ui->LabelHypervolumen->setVisible(false);
    }
  }

void MainWindow::setPointStyle(QCPScatterStyle::ScatterShape shape) {
    QCPScatterStyle style = ui->Graphique->selectedGraphs().first()->scatterStyle();
    style.setShape(shape);
    ui->Graphique->selectedGraphs().first()->setScatterStyle(style);
}

void MainWindow::setPointStyleCross() {
    setPointStyle(QCPScatterStyle::ssCross);
}

void MainWindow::setPointStylePlus() {
    setPointStyle(QCPScatterStyle::ssPlus);
}

void MainWindow::setPointStyleCircle() {
    setPointStyle(QCPScatterStyle::ssCircle);
}

void MainWindow::setPointStyleDisc() {
    setPointStyle(QCPScatterStyle::ssDisc);
}

void MainWindow::setPointStyleSquare() {
    setPointStyle(QCPScatterStyle::ssSquare);
}

void MainWindow::setPointStyleDiamond() {
    setPointStyle(QCPScatterStyle::ssDiamond);
}

void MainWindow::setPointStyleTriangle() {
    setPointStyle(QCPScatterStyle::ssTriangle);
}
