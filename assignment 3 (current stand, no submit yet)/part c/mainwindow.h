#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "network.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void SuchWortChanged();     // methode für die slots: das ändern der suchergebnisse, wenn suchwort anders wird
    void RouteChanged();        // methode für wenn die linienwahl geändert wurde

private:
    Ui::MainWindow *ui;
    Network beispiel;           // objekt zur klasse "network"
};
#endif // MAINWINDOW_H
