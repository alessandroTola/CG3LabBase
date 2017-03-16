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
    ui->eigenToCgal->setEnabled(b);
    ui->label_4->setEnabled(b);
    ui->nPlane->setEnabled(b);
    ui->writeCoordinate->setEnabled(b);
    ui->showAxis->setEnabled(b);
    ui->clearMesh->setEnabled(b);
    ui->loadMesh->setEnabled(false);
    ui->rotationAxis->setEnabled(b);
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
}

void DrawManager::on_eigenToCgal_clicked()
{
    convertEigenMesh(meshEigen);
    ui->drawPolyline->setEnabled(true);
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

void DrawManager::on_clearAxis_clicked()
{
    mainWindow->clearDebugCylinders();
    ui->clearAxis->setEnabled(false);
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

void DrawManager::on_writeCoordinate_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        ui->coordinate->setEnabled(true);
        ui->label_2->setEnabled(true);
        ui->label_3->setEnabled(true);
        ui->label->setEnabled(true);
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
        ui->label_2->setEnabled(false);
        ui->label_3->setEnabled(false);
        ui->label->setEnabled(false);
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

void DrawManager::on_drawPolyline_clicked()
{

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
    polyline.minMaxPoints(mesh, selection);

    mainWindow->enableDebugObjects();
    mainWindow->clearDebugSpheres();
    mainWindow->addDebugSphere(Pointd(polyline.getMin().x(), polyline.getMin().y(), polyline.getMin().z()),
                               0.01,
                               QColor("#ff0000"),50);
    mainWindow->addDebugSphere(Pointd(polyline.getMax().x(), polyline.getMax().y(), polyline.getMax().z()),
                               0.01,
                               QColor("#ff0000"),50);
    PlaneC plane(polyline.getMax(), polyline.getMin(), PointC(0,0,0));
    K::Vector_3 chePalle(plane.orthogonal_vector());
    Vec3 normalPlane(chePalle.x(), chePalle.y(), chePalle.z() );

    polyline.setPoly(mesh, normalPlane, 0);

    for(int i = 0; i < polyline.poly.size(); i++){
        for(int j = 0; j < polyline.poly[i].size(); j++){
            mainWindow->addDebugSphere(polyline.poly[i][j], 0.01, QColor("#ff0000"),50);
        }
    }
}

void DrawManager::on_clearMesh_clicked()
{
    ui->selectAxis->setEnabled(false);
    ui->xAxis->setEnabled(false);
    ui->yAxis->setEnabled(false);
    ui->zAxis->setEnabled(false);
    ui->eigenToCgal->setEnabled(false);
    ui->label_4->setEnabled(false);
    ui->nPlane->setEnabled(false);
    ui->writeCoordinate->setEnabled(false);
    ui->showAxis->setEnabled(false);
    ui->clearMesh->setEnabled(false);
    ui->rotationAxis->setEnabled(false);
    ui->loadMesh->setEnabled(true);
    mainWindow->deleteObj(meshEigen);
    delete meshEigen;
    meshEigen = nullptr;

}
