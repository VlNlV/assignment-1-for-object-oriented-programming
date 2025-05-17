#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , beispiel("C:/Users/varun/OneDrive/Desktop/SoSe25CODE/ubung-2-abgabe-so-se-25/allfiles")   // das network objekt bekommt den pfad (zum zugriff auf die dateien zu bekommen)
{
    ui->setupUi(this);

    connect(ui->suchwort, &QLineEdit::textChanged, this, &MainWindow::SuchWortChanged); // signal wird mit slot verbunden, also methode wird aufgerufen, wenn das suchwort-line-edit part geändert wird
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SuchWortChanged()
{
    QString input = ui->suchwort->text();   // der text von suchwort wird in "input" gespeichert
    std::string suchwort_als_string = input.toStdString();  // input wird in ein string konvertiert, damit die suchfunktion damit arbeiten kann

    std::vector<bht::SStop> ergebnisse = beispiel.search(suchwort_als_string);  // an dem network objekt wird die suchfunktion aufgerufen mit dem bestimmten suchwort, es resultiert der ergebnisvector (von SStops) mit den gesuchten haltestellen

    ui->suchergebnisse->clear();    // entfernt die vorherigen haltestellen (wenn vorhanden) aus der listwidget (damit keine haltestellen bei suchwortänderung sich nicht "aufstapeln")

    for (const auto& stop : ergebnisse) // geh durch den ergebnisvector und via einer range based for schleife geh durch eine haltestelle
    {
        QString haltestelleimlistwidget = QString::fromStdString(stop.name);        // konvertierte immer eines der gesuchten haltestellen (zurzeit als normaler string) in einen qstring, damit sie später via "additem" ins listwidget hinzugefügt werden kann
        ui->suchergebnisse->addItem(haltestelleimlistwidget);                   // fügt die haltestelle (als qstring) jetzt zum listwidget
    }
}
