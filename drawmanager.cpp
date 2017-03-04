#include "drawmanager.h"
#include "ui_drawmanager.h"

#include <QtGui>
#include <Qt3DExtras/QCylinderMesh>

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
        vectorUser = new QVector3D();
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
        vectorUser = new QVector3D();
    }
    QLineEdit *yCordinate = new QLineEdit;
    yCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, yCordinate));
    vectorUser->setY(ui->y->text().toDouble());

}

void DrawManager::on_z_editingFinished()
{
    QLineEdit *zCordinate = new QLineEdit;
    zCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, zCordinate));
    vectorUser->setZ(ui->z->text().toDouble());

}

void DrawManager::on_pushButton_2_clicked()
{
    vectorUser->normalize();

}
