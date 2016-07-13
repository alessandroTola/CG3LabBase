#ifndef IGLMESHMANAGER_H
#define IGLMESHMANAGER_H

#include <QFrame>

namespace Ui {
    class IGLMeshManager;
}

class IGLMeshManager : public QFrame
{
        Q_OBJECT

    public:
        explicit IGLMeshManager(QWidget *parent = 0);
        ~IGLMeshManager();

    private:
        Ui::IGLMeshManager *ui;
};

#endif // IGLMESHMANAGER_H
