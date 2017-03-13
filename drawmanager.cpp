#include "drawmanager.h"
#include "ui_drawmanager.h"

#include <QtGui>
#include <string>

#include <eigenmesh/eigenmesh/gui/drawableeigenmesh.h>
#include <eigenmesh/eigenmesh/eigenmesh.h>
#include <eigenmesh/eigenmesh/gui/eigenmeshmanager.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>

#include <viewer/interfaces/drawable_object.h>

typedef Mesh::Vertex_index vertex_descriptor;
typedef Mesh::Face_index face_descriptor;
typedef Mesh::Vertex_range verticesIter;

DrawManager::DrawManager(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DrawManager),
    mainWindow((MainWindow*)parent)

{
    ui->setupUi(this);
}

DrawManager::~DrawManager(){
    delete ui;
}

void DrawManager::on_x_editingFinished()
{
    QLineEdit *xCordinate = new QLineEdit;
    xCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, xCordinate));
    vectorUser.setX(ui->x->text().toDouble());
    //QString prova = QString::number(vectorUser->x());
    //ui->x->setText(prova + " ciao");

}

void DrawManager::on_y_editingFinished()
{
    QLineEdit *yCordinate = new QLineEdit;
    yCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, yCordinate));
    vectorUser.setY(ui->y->text().toDouble());
}

void DrawManager::on_z_editingFinished()
{
    QLineEdit *zCordinate = new QLineEdit;
    zCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, zCordinate));
    vectorUser.setZ(ui->z->text().toDouble());
}

void DrawManager::on_drawAxis_clicked()
{
    mainWindow->enableDebugObjects();
    mainWindow->clearDebugCylinders();
    vectorUser.normalize();
    mainWindow->addDebugCylinder(Pointd(0,0,0),
                                 Pointd(vectorUser.x(), vectorUser.y(), vectorUser.z()),
                                 0.01,
                                 QColor("#ff0000"));
}


void DrawManager::on_loadMesh_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "Open Eigen Mesh",
                       ".",
                       "OBJ(*.obj);;PLY(*.ply)");
    if (!filename.isEmpty()) {
        //meshEigen = new DrawableEigenMesh();
        meshEigen.readFromObj(filename.toStdString());
        mainWindow->pushObj(&meshEigen, filename.toStdString().substr(filename.toStdString().find_last_of("/") + 1));
        //setButtonsMeshLoaded(true);
        mainWindow->updateGlCanvas();
    }
}

void DrawManager::on_eigenToCgal_clicked()
{
    convertEigenMesh(&meshEigen);
    /*typedef SM_Halfedge_index Halfedge_index;
    typedef SM_Vertex_index verIndex;
    boost::uint32_t id = 34;
    Halfedge_index sdad = Halfedge_index(id) ;
    verIndex ide = 0;
    QString prova = QString::number(mesh->face());
    ui->x->setText(prova + " ciao");*/
}

void DrawManager::convertEigenMesh (DrawableEigenMesh *meshEigenOrigin){

    int nVertex = meshEigenOrigin->getNumberVertices();
    int nFaces = meshEigenOrigin->getNumberFaces();

    std::vector<vertex_descriptor> vecIter;

    for(int i=0; i<nVertex;i++){
        Point<double> v = meshEigenOrigin->getVertex(i);
        vertex_descriptor vert = mesh.add_vertex(K::Point_3(v.x(),v.y(),v.z()));
        vecIter.push_back(vert);
    }

    for(int i=0; i<nFaces;i++){
        Point<int> f = meshEigenOrigin->getFace(i);
        vertex_descriptor u = vecIter[f.x()];
        vertex_descriptor v = vecIter[f.y()];
        vertex_descriptor z = vecIter[f.z()];
        mesh.add_face(u, v, z);
    }
}

void DrawManager::on_drawCilinder_clicked()
{
        mainWindow->clearDebugCylinders();
}

void DrawManager::on_saveMeshCgal_clicked()
{
      QFileDialog filedialog(mainWindow, tr("Export surface to file"));
      filedialog.setFileMode(QFileDialog::AnyFile);

      filedialog.setNameFilter(tr("OFF files (*.off);;"
                                  "All files (*)"));

      filedialog.setAcceptMode(QFileDialog::AcceptSave);
      filedialog.setDefaultSuffix("off");
      if(filedialog.exec())
      {
        const QString filename = filedialog.selectedFiles().front();
        std::cerr << "Saving to file \"" << filename.toLocal8Bit().data() << "\"...";
        std::ofstream out(filename.toUtf8());
        out<<mesh;

        if(!out)
        {
          QMessageBox::warning(mainWindow, mainWindow->windowTitle(),
                               tr("Export to the OFF file <tt>%1</tt> failed!").arg(filename));
          std::cerr << qPrintable(tr("Export to the OFF file %1 failed!").arg(filename)) << std::endl;
          mainWindow->statusBar()->showMessage(tr("Export to the OFF file %1 failed!").arg(filename));
          std::cerr << " failed!\n";
        }
        else
        {
          std::cerr << " done.\n";
          std::cerr << qPrintable(tr("Successfull export to the OFF file %1.").arg(filename)) << std::endl;
          mainWindow->statusBar()->showMessage(tr("Successfull export to the OFF file %1.").arg(filename));
        }
      }
}
