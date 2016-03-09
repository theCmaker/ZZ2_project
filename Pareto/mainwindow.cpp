#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QGraphicsScene>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    s(nullptr),
    ui(new Ui::MainWindow),
    file_path_(".")
{
    ui->setupUi(this);

    setWindowTitle("ParetoFront Viewer");

    ui->Graphique->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables | QCP::iSelectItems);
    ui->Graphique->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->Graphique, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
    connect(ui->Graphique, SIGNAL(selectionChangedByUser()), this, SLOT(update_visualization_panel()));
    connect(ui->Graphique, SIGNAL(selectionChangedByUser()), this, SLOT(show_hypervolumen()));

    ui->actionClose->setDisabled(true);
    ui->actionSave_as->setDisabled(true);
    ui->actionGNUPlot->setDisabled(true);
    ui->actionJPG_Image->setDisabled(true);
    ui->actionPNG_Image->setDisabled(true);
    ui->actionPDF_File->setDisabled(true);
    ui->actionTikZ_for_LaTeX->setDisabled(true);

    ui->actionAxes->setDisabled(true);
    ui->actionLabels->setDisabled(true);
    ui->actionGrid->setDisabled(true);

    hide_stats();
}

MainWindow::~MainWindow() {
    delete ui;
    if (s != nullptr) {
        delete s;
    }
}

//------------------------------------------------------------------------
// composant de la sortie
//------------------------------------------------------------------------
QTextEdit* MainWindow::Sortie_application() {
    return ui->Console;
}

void MainWindow::on_actionOpen_triggered() {
    //afficher la dialog de openfile
    QString f_nom = QFileDialog::getOpenFileName(this,tr("Open file"),tr(file_path_.c_str()),tr("All files (*)"));

    if(!f_nom.isEmpty())
    {
        //std::cout << f_nom.toStdString() << std::endl;
        unsigned index = f_nom.toStdString().find_last_of('/');
        file_path_ = f_nom.toStdString().substr(0,index);

        ui->actionOpen->setDisabled(true);
        ui->actionClose->setDisabled(false);
        ui->actionSave_as->setDisabled(false);
        //ui->actionGNUPlot->setDisabled(false);
        ui->actionJPG_Image->setDisabled(false);
        ui->actionPNG_Image->setDisabled(false);
        ui->actionPDF_File->setDisabled(false);
        ui->actionTikZ_for_LaTeX->setDisabled(false);

        ui->actionGrid->setDisabled(false);
        ui->actionAxes->setDisabled(false);
        ui->actionLabels->setDisabled(false);

        ui->actionGrid->setChecked(true);
        ui->actionAxes->setChecked(true);
        ui->actionLabels->setChecked(true);

        load_file(f_nom);
    }
}

void MainWindow::on_actionClose_triggered() {
    if (s != nullptr) {
        //View
        ui->Graphique->reset();
        ui->Console->clear();
        delete s;
        s = nullptr;

        hide_stats();

        //Actions
        ui->actionClose->setDisabled(true);
        ui->actionSave_as->setDisabled(true);
        ui->actionGNUPlot->setDisabled(true);
        ui->actionJPG_Image->setDisabled(true);
        ui->actionPNG_Image->setDisabled(true);
        ui->actionPDF_File->setDisabled(true);
        ui->actionTikZ_for_LaTeX->setDisabled(true);
        ui->actionOpen->setDisabled(false);

        ui->actionAxes->setDisabled(true);
        ui->actionLabels->setDisabled(true);
        ui->actionGrid->setDisabled(true);
    }
}

void MainWindow::on_actionSave_as_triggered() {
    QString f_nom = QFileDialog::getSaveFileName(this,tr("Save as"),tr(file_path_.c_str()),tr("All files (*)"));
    if (!f_nom.isEmpty())
    {
        unsigned index = f_nom.toStdString().find_last_of('/');
        file_path_ = f_nom.toStdString().substr(0,index);
        s->saveToFile(f_nom.toStdString().c_str());
    }
}

void MainWindow::on_actionTikZ_for_LaTeX_triggered() {
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Export to TikZ file"),tr(file_path_.c_str()),tr("LaTeX Source Files (*.tex)"));
    if (!f_nom.isEmpty()) {
        unsigned index = f_nom.toStdString().find_last_of('/');
        file_path_ = f_nom.toStdString().substr(0,index);
        compute_style();
        s->exportToTikZ(f_nom.toStdString().c_str());
    }
}

void MainWindow::on_actionPNG_Image_triggered() {
    QString f_nom=QFileDialog::getSaveFileName(this,tr("Export to PNG Image"),tr(file_path_.c_str()),tr("PNG Files (*.png)"));
    if (!f_nom.isEmpty()) {
        unsigned index = f_nom.toStdString().find_last_of('/');
        file_path_ = f_nom.toStdString().substr(0,index);
        ui->Graphique->savePng(f_nom,400,400,5);
    }
}

void MainWindow::on_actionJPG_Image_triggered() {
    QString f_nom = QFileDialog::getSaveFileName(this,tr("Export to JPG Image"),tr(file_path_.c_str()),tr("JPG Files (*.jpg,*.jpeg)"));
    if (!f_nom.isEmpty()) {
        unsigned index = f_nom.toStdString().find_last_of('/');
        file_path_ = f_nom.toStdString().substr(0,index);
        ui->Graphique->saveJpg(f_nom,400,400,5);
    }
}

void MainWindow::on_actionPDF_File_triggered() {
    QString f_nom = QFileDialog::getSaveFileName(this,tr("Export to PDF File"),tr(file_path_.c_str()),tr("PDF Files (*.pdf,*.xpdf)"));
    if (!f_nom.isEmpty()) {
        unsigned index = f_nom.toStdString().find_last_of('/');
        file_path_ = f_nom.toStdString().substr(0,index);
        ui->Graphique->savePdf(f_nom);
    }
}


void MainWindow::on_actionAxes_changed()
{
    bool active = ui->actionAxes->isChecked();
    ui->Graphique->xAxis->setVisible(active);
    ui->Graphique->yAxis->setVisible(active);
    if (!active) {
        ui->actionGrid->setDisabled(true);
        ui->actionLabels->setDisabled(true);
    } else {
        ui->actionGrid->setDisabled(false);
        ui->actionLabels->setDisabled(false);
    }
    if (s != nullptr) {
        s->getStyle().setHas_axes(active);
        s->getStyle().setHas_grid(ui->actionGrid->isEnabled() && ui->actionGrid->isChecked());
    }
    ui->Graphique->replot();
    //std::cout << "Axes are now " << (active?"enabled":"disabled") << std::endl;
}

void MainWindow::on_actionGrid_changed()
{
    bool active = ui->actionGrid->isChecked();
    if (s != nullptr) {
        s->getStyle().setHas_grid(active);
    }
    ui->Graphique->xAxis->grid()->setVisible(active);
    ui->Graphique->yAxis->grid()->setVisible(active);
    ui->Graphique->replot();
    //std::cout << "Grid is now " << (active?"enabled":"disabled") << std::endl;
}

void MainWindow::on_actionLabels_changed()
{
    bool active = ui->actionLabels->isChecked();
    if (s != nullptr) {
        s->getStyle().setHas_axes_labels(active);
    }
    if (!active) {
        ui->Graphique->xAxis->setLabel("");
        ui->Graphique->yAxis->setLabel("");
    } else if (s != nullptr) {
        ui->Graphique->xAxis->setLabel(s->getAbscissa().c_str());
        ui->Graphique->yAxis->setLabel(s->getOrdinate().c_str());
    }
    ui->Graphique->replot();
    //std::cout << "Labels are now " << (active?"enabled":"disabled") << std::endl;
}

void MainWindow::load_file(QString f_nom) {
    s = new Solutions(f_nom.toStdString().c_str());
    s->compute_frontiers();
    ui->Graphique->setSolution(s);

    // Pareto Frontier
    QColor f_color = Qt::darkRed;
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
        ui->Graphique->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, f_color.lighter(factor),f_color.lighter(factor), 10));
        ui->Graphique->graph(i)->setLineStyle(QCPGraph::lsLine);
        ui->Graphique->graph(i)->setPen(QPen(f_color.lighter(factor)));

        s->compute_hypervolumen(pareto[i]);
        pareto[i].compute_distances();
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
    s->getStyle().setY_ticks(ui->Graphique->yAxis->tickVector().toStdVector());
    s->compute_front_style(ui->Graphique);
}

void MainWindow::contextMenuRequest(QPoint pos) {
  if (ui->Graphique->selectedGraphs().size() == 1) {
    QMenu *menu = new QMenu(this);
    QMenu *menu_style = new QMenu("Style",menu);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu_style->setAttribute(Qt::WA_DeleteOnClose);
    menu_style->addAction("Cross", this, SLOT(setPointStyleCross()));
    menu_style->addAction("Plus", this, SLOT(setPointStylePlus()));
    menu_style->addAction("Disc", this, SLOT(setPointStyleDisc()));
    menu_style->addAction("Square", this, SLOT(setPointStyleSquare()));
    menu_style->addAction("Diamond", this, SLOT(setPointStyleDiamond()));
    menu_style->addAction("Triangle", this, SLOT(setPointStyleTriangle()));
    menu->addMenu(menu_style);

    menu->popup(ui->Graphique->mapToGlobal(pos));
  }
}

void MainWindow::update_visualization_panel() {
    if (ui->Graphique->selectedGraphs().size() == 1) {
      //Find the selected graph
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
              s->getStyle().setSelected_front(index);
            } else {
            //otherwise, we need to try another front.
                ++index;
            }
        }
      }
      if (found) {
        std::stringstream front_value;
        std::stringstream hypervolumen_value;
        std::stringstream nbpoints_value;
        std::stringstream meandistance_value;
        std::stringstream maxdistance_value;
        std::stringstream sumdistance_value;

        front_value << index;
        hypervolumen_value << frontiers[index].hypervolumen();
        nbpoints_value << frontiers[index].pts().size();
        meandistance_value << frontiers[index].mean_distance();
        maxdistance_value << frontiers[index].max_distance();
        sumdistance_value << frontiers[index].sum_distance();

        ui->ValueFront->setText(QString(front_value.str().c_str()));
        ui->ValueHypervolumen->setText(QString(hypervolumen_value.str().c_str()));
        ui->ValueNbPoints->setText(QString(nbpoints_value.str().c_str()));
        ui->ValueMeanDistance->setText(QString(meandistance_value.str().c_str()));
        ui->ValueMaxDistance->setText(QString(maxdistance_value.str().c_str()));
        ui->ValueSumDistance->setText(QString(sumdistance_value.str().c_str()));

        show_stats();
      }
    } else {
        hide_stats();
        s->getStyle().setSelected_front(-1);
    }
}

void MainWindow::show_stats() {
    //Labels
    ui->LabelFront->setVisible(true);
    ui->LabelHypervolumen->setVisible(true);
    ui->LabelNbPoints->setVisible(true);
    ui->LabelMeanDistance->setVisible(true);
    ui->LabelMaxDistance->setVisible(true);
    ui->LabelSumDistance->setVisible(true);
    ui->LabelNoFrontSelected->setVisible(false);
    //Values
    ui->ValueFront->setVisible(true);
    ui->ValueHypervolumen->setVisible(true);
    ui->ValueNbPoints->setVisible(true);
    ui->ValueMeanDistance->setVisible(true);
    ui->ValueMaxDistance->setVisible(true);
    ui->ValueSumDistance->setVisible(true);
}

void MainWindow::hide_stats() {
    //Labels
    ui->LabelNoFrontSelected->setVisible(true);
    ui->LabelFront->setVisible(false);
    ui->LabelHypervolumen->setVisible(false);
    ui->LabelNbPoints->setVisible(false);
    ui->LabelMeanDistance->setVisible(false);
    ui->LabelMaxDistance->setVisible(false);
    ui->LabelSumDistance->setVisible(false);
    //Values
    ui->ValueFront->setVisible(false);
    ui->ValueHypervolumen->setVisible(false);
    ui->ValueNbPoints->setVisible(false);
    ui->ValueMeanDistance->setVisible(false);
    ui->ValueMaxDistance->setVisible(false);
    ui->ValueSumDistance->setVisible(false);
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

void MainWindow::show_hypervolumen()
{
    ui->Graphique->paintHypervolumen();
}
