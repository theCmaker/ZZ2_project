#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QBrush>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Visualisation de Front de Pareto");
    ui->Graphique->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
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
    //Console d'affichage DEBUG
    QTextEdit *Console = ui->Console;

    if(!f_nom.isEmpty())
    {
        Console->append("Ouverture fichier : ");
        load_file(f_nom);
    }
}

void MainWindow::load_file(QString f_nom) {
    /*QGraphicsScene *scene = new QGraphicsScene;*/
    s = new Solutions(f_nom.toStdString().c_str());
    ui->Graphique->setSolution(s);
    /*const QBrush dots(Qt::darkRed);*/

    //View values
    /*qreal point_size = 10.;
    qreal unit_width = (ui->Vue->geometry().width() - 2*point_size) / (s->getMaxX() - s->getMinX());
    qreal unit_height = (ui->Vue->geometry().height() - 2*point_size) / (s->getMaxY() - s->getMinY());*/


    //Points
    std::vector<FPoint> v = s->getPts();
    QVector<double> x(v.size()), y(v.size());
    int j = 0;
    for (FPointv::iterator i = v.begin(); i != v.end(); ++i) {
        /*scene->addEllipse(i->getX()*unit_width - point_size/2.,
                          -(i->getY()*unit_height + point_size/2.),
                          point_size/1,
                          point_size/1,
                          QPen(),
                          dots);*/
        x[j] = i->getX();
        y[j] = i->getY();
        ++j;
    }
    /*
    ui->Graphique->addGraph();
    ui->Graphique->graph(0)->setData(x,y);
    ui->Graphique->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle, Qt::blue, Qt::NoBrush, 10));
    ui->Graphique->graph(0)->setLineStyle(QCPGraph::lsNone);*/


    // Pareto Frontier
    QColor f_color = Qt::darkRed;
    s->compute_frontiers();
    std::cout << "Pareto frontier" << std::endl;
    ParetoFrontv pareto = s->getPFrontiers();
    int factor;
    /*for (FPoints::iterator i = pareto.begin(); i != pareto.end(); ++i) {
        std::cout << *i << std::endl;
    }*/

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

    //Bounding box
    /*
    scene->addRect(s->getMinX()*unit_width,
                   -s->getMaxY()*unit_height,
                   (s->getMaxX() - s->getMinX())*unit_width,
                   (s->getMaxY() - s->getMinY())*unit_height);
    ui->Vue->setScene(scene);
    */
}
