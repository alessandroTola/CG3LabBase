#include "iglmeshmanager.h"
#include "ui_iglmeshmanager.h"

IGLMeshManager::IGLMeshManager(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::IGLMeshManager)
{
    ui->setupUi(this);
}

IGLMeshManager::~IGLMeshManager()
{
    delete ui;
}
