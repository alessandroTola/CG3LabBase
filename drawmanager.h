#ifndef DRAWMANAGER_H
#define DRAWMANAGER_H

#include <QFrame>
#include <viewer/mainwindow.h>

#include <Qt3DExtras/QCylinderMesh>
#include <QtCore/QObject>



class QLineEdit;

namespace Ui {
    class DrawManager;
}

class DrawManager : public QFrame
{
        Q_OBJECT

    public:
        explicit DrawManager(QWidget *parent = 0);
        ~DrawManager();

    private slots:

        void on_x_editingFinished();

        void on_y_editingFinished();

        void on_z_editingFinished();

        void on_pushButton_2_clicked();

private:

        Ui::DrawManager *ui;
        MainWindow* mainWindow;
        QVector3D *vectorUser;
        Qt3DCore::QEntity *rootEntity;
        Qt3DCore::QEntity *cylinderEntity;

};

#endif // DRAWMANAGER_H
