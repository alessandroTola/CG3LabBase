#include "drawmanager.h"
#include "ui_drawmanager.h"

#include <QtGui>
#include <string>



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

void DrawManager::setButtonMeshLoaded(bool b){
    ui->selectAxis->setEnabled(b);
    ui->xAxis->setEnabled(b);
    ui->yAxis->setEnabled(b);
    ui->zAxis->setEnabled(b);
    ui->nPlaneLabel->setEnabled(b);
    ui->nPlane->setEnabled(b);
    //ui->writeCoordinate->setEnabled(b);
    ui->showAxis->setEnabled(b);
    ui->clearMesh->setEnabled(b);
    ui->loadMesh->setEnabled(false);
    ui->xLabel->setEnabled(b);
    ui->yLabel->setEnabled(b);
    ui->xCoord->setEnabled(b);
    ui->yCoord->setEnabled(b);
    ui->serchPoint->setEnabled(b);
    ui->selectLabel->setEnabled(b);
    ui->cylinder->setEnabled(b);
    ui->Cylinder->setEnabled(b);
    ui->sphere->setEnabled(b);
    ui->Sphere->setEnabled(b);
    ui->saveMesh->setEnabled(b);
    ui->check->setEnabled(b);
    ui->stepByStep->setEnabled(b);
    ui->meshToOrigin->setEnabled(b);

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
    ui->clearAxis->setEnabled(true);
    mainWindow->enableDebugObjects();
    mainWindow->clearDebugCylinders();
    vectorUser.normalize();
    mainWindow->addDebugCylinder(Pointd(0,0,0),
                                 Pointd(vectorUser.x(), vectorUser.y(), vectorUser.z()),
                                 0.01,
                                 QColor("#ff0000"));
}

void DrawManager::on_nPlane_editingFinished(){
    QLineEdit *nPlane = new QLineEdit;
    nPlane->setValidator(new QIntValidator(0, 360,nPlane));
    nPlaneUser = ui->nPlane->text().toInt();
    stepAngle = 180 / nPlaneUser;
    stepColor = 120 / nPlaneUser;
    nextColor = - stepColor;
    polyline.setCheckerDimension(meshEigen->getNumberFaces());
}

void DrawManager::on_loadMesh_clicked()
{
    QString filename = QFileDialog::getOpenFileName(nullptr,
                       "Open Eigen Mesh",
                       ".",
                       "OBJ(*.obj);;PLY(*.ply)");
    if (!filename.isEmpty()) {
        meshEigen = new DrawableEigenMesh();
        meshEigen->readFromObj(filename.toStdString());
        mainWindow->pushObj(meshEigen, filename.toStdString().substr(filename.toStdString().find_last_of("/") + 1));
        setButtonMeshLoaded(true);
        mainWindow->updateGlCanvas();
    }
    sphere = 0.01;
    cylinder = 0.01;
    increse = 0;
    QColor c;
    c.setHsv(100,0,255);
    for(unsigned int i = 0 ; i < meshEigen->getNumberFaces(); i++){
        meshEigen->setFaceColor(c.redF(), c.greenF(), c.blueF(), i);
    }
}

void DrawManager::on_eigenToCgal_clicked()
{
    convertEigenMesh(meshEigen);
}

void DrawManager::convertEigenMesh (DrawableEigenMesh *meshEigenOrigin){
    //Il tasto è stato rimosso
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

void DrawManager::on_clearAxis_clicked()
{
    mainWindow->clearDebugCylinders();
    ui->clearAxis->setEnabled(false);
}

void DrawManager::on_saveMeshCgal_clicked()
{
    //Tasto rimosso dal manager
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

void DrawManager::on_writeCoordinate_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        ui->coordinate->setEnabled(true);
        ui->labelX->setEnabled(true);
        ui->labelY->setEnabled(true);
        ui->labelZ->setEnabled(true);
        ui->x->setEnabled(true);
        ui->y->setEnabled(true);
        ui->z->setEnabled(true);
        ui->drawAxis->setEnabled(true);
        ui->selectAxis->setEnabled(false);
        ui->xAxis->setEnabled(false);
        ui->yAxis->setEnabled(false);
        ui->zAxis->setEnabled(false);
    } else {
        ui->selectAxis->setEnabled(true);
        ui->xAxis->setEnabled(true);
        ui->yAxis->setEnabled(true);
        ui->zAxis->setEnabled(true);
        ui->coordinate->setEnabled(false);
        ui->labelX->setEnabled(false);
        ui->labelY->setEnabled(false);
        ui->labelZ->setEnabled(false);
        ui->x->setEnabled(false);
        ui->y->setEnabled(false);
        ui->z->setEnabled(false);
        ui->drawAxis->setEnabled(false);
    }
}

void DrawManager::on_showAxis_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) mainWindow->drawAxis(true);
    else mainWindow->drawAxis(false);
    mainWindow->updateGlCanvas();
}

void DrawManager::on_xAxis_toggled(bool checked)
{
    if(checked){
        selection=0;
    }
}

void DrawManager::on_yAxis_toggled(bool checked)
{
    if(checked){
        selection=1;
    }
}

void DrawManager::on_zAxis_toggled(bool checked)
{
    if(checked){
        selection=2;
    }
}

void DrawManager::on_rotationAxis_clicked()
{
    //Funzione momentaneamente rimossa, cambio piano di attacco alla mesh
    /*polyline.minMaxPoints(mesh, selection);

    mainWindow->enableDebugObjects();
    mainWindow->clearDebugSpheres();

    //PlaneC plane(polyline.getMax(), polyline.getMin(), PointC(0,0,0));
    PointC punto(polyline.getMin().x(), polyline.getMin().y(), polyline.getMin().z()+1);
    mainWindow->addDebugSphere(Pointd(polyline.getMin().x(), polyline.getMin().y(), polyline.getMin().z()),0.01,QColor("#ff0000"),50);
    mainWindow->addDebugSphere(Pointd(polyline.getMax().x(), polyline.getMax().y(), polyline.getMax().z()),0.01,QColor("#ff0000"),50);
    mainWindow->addDebugSphere(Pointd(punto.x(), punto.y(), punto.z()), 0.01, QColor("#ff0000"),50);

    //Creo il piano passante per 3 punti
    PlaneC plane(polyline.getMax(), polyline.getMin(), punto);

    K::Vector_3 chePalle(plane.orthogonal_vector());
    Vec3 normalPlane(chePalle.x(), chePalle.y(), chePalle.z() );

    polyline.setNormal(normalPlane);
    polyline.setD();
    polyline.setPoly(mesh, normalPlane);

    for(unsigned int i = 0; i < polyline.poly.size(); i++){
        for(unsigned int j = 0; j < polyline.poly[i].size(); j++){
            mainWindow->addDebugSphere(polyline.poly[i][j], 0.01, QColor("#ff0000"),50);
        }
    }
    polyline.checkPolyline();
    qDebug() << polyline.poly2d[0].x();
    for(unsigned int i = 0; i <polyline.poly2d.size(); i++) {
        mainWindow->addDebugSphere(Pointd(polyline.poly2d[i].x(),polyline.poly2d[i].y(),0) , 0.01, QColor("#ff0000"),50);
    }*/
}

void DrawManager::on_clearMesh_clicked()
{
    ui->selectAxis->setEnabled(false);
    ui->xAxis->setEnabled(false);
    ui->yAxis->setEnabled(false);
    ui->zAxis->setEnabled(false);
    ui->nPlaneLabel->setEnabled(false);
    ui->nPlane->setEnabled(false);
    ui->writeCoordinate->setEnabled(false);
    ui->showAxis->setEnabled(false);
    ui->clearMesh->setEnabled(false);
    ui->loadMesh->setEnabled(true);
    mainWindow->deleteObj(meshEigen);
    mainWindow->clearDebugCylinders();
    mainWindow->clearDebugSpheres();
    delete meshEigen;
    polyline.getChecker().clear();
    meshEigen = nullptr;

}

void DrawManager::on_xCoord_editingFinished()
{
    QLineEdit *xCordinate = new QLineEdit;
    xCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, xCordinate));
    coordPlane.setXCoord(ui->xCoord->text().toDouble());
}

void DrawManager::on_yCoord_editingFinished()
{
    QLineEdit *yCordinate = new QLineEdit;
    yCordinate->setValidator(new QDoubleValidator(-999.0, 999.0, 2, yCordinate));
    coordPlane.setYCoord(ui->yCoord->text().toDouble());
}

void DrawManager::on_serchPoint_clicked()
{
    switch (selection) {
    case 0:
        vectorUser.setX(-100);
        vectorUser.setY(coordPlane.y());
        vectorUser.setZ(coordPlane.x());
        point.setX(100);
        point.setY(coordPlane.y());
        point.setZ(coordPlane.x());
        break;
    case 1:
        vectorUser.setX(coordPlane.x());
        vectorUser.setY(-100);
        vectorUser.setZ(coordPlane.y());
        point.setX(coordPlane.x());
        point.setY(100);
        point.setZ(coordPlane.y());
        break;
    case 2:
        vectorUser.setX(coordPlane.y());
        vectorUser.setY(coordPlane.x());
        vectorUser.setZ(-100);
        point.setX(coordPlane.y());
        point.setY(coordPlane.x());
        point.setZ(100);
        break;
    default:
        break;
    }
    mainWindow->enableDebugObjects();
    mainWindow->clearDebugSpheres();
    mainWindow->clearDebugCylinders();
    mainWindow->addDebugSphere(vectorUser, sphere, QColor("#ff0000"),50);
    mainWindow->addDebugCylinder(vectorUser, point,cylinder, QColor("#ff0000"));
}

void DrawManager::on_sphere_editingFinished()
{
    QLineEdit *spherea = new QLineEdit;
    spherea->setValidator(new QDoubleValidator(-999.0, 999.0, 2, spherea));
    sphere = ui->sphere->text().toDouble();
}

void DrawManager::on_cylinder_editingFinished()
{
    QLineEdit *cylindera = new QLineEdit;
    cylindera->setValidator(new QDoubleValidator(-999.0, 999.0, 2, cylindera));
    cylinder = ui->cylinder->text().toDouble();
}

void DrawManager::on_drawPoint_clicked()
{
    polyline.checkIntersect(meshEigen, vectorUser, point, selection);
    mainWindow->enableDebugObjects();
    mainWindow->clearDebugSpheres();
    mainWindow->clearDebugCylinders();
    mainWindow->addDebugSphere(polyline.minP, sphere, QColor("#ff0000"),50);
    mainWindow->addDebugSphere(polyline.maxP, sphere, QColor("#ff0000"),50);

}

void DrawManager::on_translate_clicked()
{
    //Pointd centro = -meshEigen->getBarycenter();
    Pointd centro(-((polyline.getMax()-polyline.getMin())/2+polyline.getMin()));
    meshEigen->translate(centro);
    Matrix3d rotation;
    Vec3 y(1,0,0);
    Vec3 z(0,0,1);
    Vec3 x(1,0,0);
    if(selection == 1){
        Common::getRotationMatrix(z, acos(0), rotation);
        meshEigen->rotate(rotation,Vector3d(0,0,0));
        Common::getRotationMatrix(x, acos(0), rotation);
        meshEigen->rotate(rotation,Vector3d(0,0,0));
    }
    if(selection == 2){
        Common::getRotationMatrix(y, -acos(0), rotation);
        meshEigen->rotate(rotation,Vector3d(0,0,0));
    }
    polyline.minP.operator +=(centro);
    polyline.maxP.operator +=(centro);
    polyline.rotatePoint(rotation,Pointd(0,0,0));
    mainWindow->clearDebugSpheres();
    mainWindow->addDebugSphere(polyline.getMax(), sphere, QColor("#ff0000"),50);
    mainWindow->addDebugSphere(polyline.getMin(), sphere, QColor("#ff0000"),50);
    mainWindow->updateGeometry();
}

void DrawManager::on_check_clicked()
{
    const double halfC = M_PI / 180;
    int angleC = 120 / nPlaneUser;
    int angleCStart = 0;
    Matrix3d rotation;
    Vec3 axis(1,0,0);
    polyline.setCheckerDimension(meshEigen->getNumberFaces());
    polyline.check(meshEigen,angleCStart);
    for(int i = 0; i < nPlaneUser; i++){
        angleCStart+=angleC;
        Common::getRotationMatrix(axis, (stepAngle * halfC), rotation);
        meshEigen->rotate(rotation,Vector3d(0,0,0));
        meshEigen->updateBoundingBox();
        polyline.check(meshEigen,angleCStart);
        mainWindow->updateGlCanvas();
    }
}

void DrawManager::on_stepByStep_clicked()
{
    if(filename == ""){
        on_saveMesh_clicked();
    }

    const double halfC = M_PI / 180;
    Vec3 axis(1,0,0);
    nextColor += stepColor;
    QColor c;
    c.setHsv(100,0,255);
    Matrix3d rotation = getRotationMatrix(axis, (stepAngle * halfC * increse));
    meshEigen->rotate(rotation,Vector3d(0,0,0));
    meshEigen->updateBoundingBox();
    polyline.resetChecker();
    polyline.setCheckerDimension(meshEigen->getNumberFaces());
    polyline.check(meshEigen,nextColor);
    mainWindow->updateGlCanvas();

    rotation = getRotationMatrix(axis, -(stepAngle * halfC * increse));
    meshEigen->rotate(rotation,Vector3d(0,0,0));
    filename.truncate(filename.size()-5);
    QString format = ".obj";
    meshEigen->saveOnObj((filename += QString::number(increse)+=format).toUtf8().constData());
    for(unsigned int i = 0 ; i < meshEigen->getNumberFaces(); i++){
        meshEigen->setFaceColor(c.redF(), c.greenF(), c.blueF(), i);
    }
    if(increse == nPlaneUser)
        ui->stepByStep->setEnabled(false);
    increse++;
}


void DrawManager::on_meshToOrigin_clicked()
{
    meshBerycenter = meshEigen->getBarycenter();
    meshEigen->translate(-meshBerycenter);
    mainWindow->updateGlCanvas();
}

void DrawManager::on_saveMesh_clicked()
{
    QFileDialog filedialog(mainWindow, tr("Export surface to file"));
    filedialog.setFileMode(QFileDialog::AnyFile);

    filedialog.setNameFilter(tr("OBJ files (*.obj);;"
                                  "All files (*)"));

    filedialog.setAcceptMode(QFileDialog::AcceptSave);
    filedialog.setDefaultSuffix("obj");
    if(filedialog.exec()){
        filename = filedialog.selectedFiles().front();
        std::cerr << "Saving to file \"" << filename.toLocal8Bit().data() << "\"...";
        meshEigen->saveOnObj(filename.toUtf8().constData());
    }

}
