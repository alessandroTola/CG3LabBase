#include "drawmanager.h"
#include "ui_drawmanager.h"

#include <QtGui>
#include <string>

#include <eigenmesh/eigenmesh/gui/drawableeigenmesh.h>
#include <eigenmesh/eigenmesh/eigenmesh.h>
#include <eigenmesh/eigenmesh/gui/eigenmeshmanager.h>
#include <QFileDialog>

#include <CGAL/Surface_mesh.h>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Surface_mesh<K::Point_3> Mesh;
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
    if(vectorUser == nullptr){
        vectorUser = new Vec3();
    }
    QLineEdit *xCordinate = new QLineEdit;
    xCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, xCordinate));
    vectorUser->setX(ui->x->text().toDouble());
    //QString prova = QString::number(vectorUser->x());
    //ui->x->setText(prova + " ciao");

}

void DrawManager::on_y_editingFinished()
{
    if(vectorUser == nullptr){
        vectorUser = new Vec3();
    }
    QLineEdit *yCordinate = new QLineEdit;
    yCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, yCordinate));
    vectorUser->setY(ui->y->text().toDouble());
}

void DrawManager::on_z_editingFinished()
{
    if(vectorUser == nullptr){
        vectorUser = new Vec3();
    }
    QLineEdit *zCordinate = new QLineEdit;
    zCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, zCordinate));
    vectorUser->setZ(ui->z->text().toDouble());
    //mainWindow->pushObj(vectorUser, "Draw axis");

}

void DrawManager::on_pushButton_2_clicked()
{
    if(vectorUser == nullptr){
        vectorUser = new Vec3(0,0,0);
    }
    //VectorXd vec = (vectorUser->x(),vectorUser->y(),vectorUser->z());
    QString filename = "/Users/alessandrotola/Desktop/modelli/cilindro2.obj";
    cylinder = new DrawableEigenMesh();
    cylinder->readFromObj(filename.toStdString());
    //cylinder->translate(vec);
    mainWindow->pushObj(cylinder, filename.toStdString().substr(filename.toStdString().find_last_of("/") + 1));
    mainWindow->updateGlCanvas();
    vectorUser->normalize();
}


void DrawManager::on_pushButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "Open Eigen Mesh",
                       ".",
                       "OBJ(*.obj);;PLY(*.ply)");
    if (!filename.isEmpty()) {
        meshEigen = new DrawableEigenMesh();
        meshEigen->readFromObj(filename.toStdString());
        mainWindow->pushObj(meshEigen, filename.toStdString().substr(filename.toStdString().find_last_of("/") + 1));
        //setButtonsMeshLoaded(true);
        mainWindow->updateGlCanvas();
    }
}

void DrawManager::on_pushButton_3_clicked()
{
    Mesh mesh = Convert::convertEigenMesh(meshEigen);

}

Surface_mesh<K::Point_3> Convert::convertEigenMesh (DrawableEigenMesh *meshEigenOrigin){

    Mesh mesh;

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

    return mesh;
}


