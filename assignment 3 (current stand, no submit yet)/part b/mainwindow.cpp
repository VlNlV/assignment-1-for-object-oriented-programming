#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , beispiel("C:/Users/varun/OneDrive/Desktop/SoSe25CODE/ubung-3-abgabe-so-se-25/allfiles") // network-objekt bekommt "zugriff" auf die files via wertzuweisung des datenpfads
{
    ui->setupUi(this);

    for(const auto& route : beispiel.getRoutes())   // range based for schleife fürs durchgehen aller routes (in route wird temporär EINE route gespeichert)
    {
        std::string nameforcomboboxitem = beispiel.getRouteDisplayName(route);  // innerhalb einer route wird das anzeige-format (shortname mit/ohne longname) bestimmt und in "nameforcomboboxitem" gespeichert
        ui->comboBox->addItem(QString::fromStdString(nameforcomboboxitem));     // da die combobox nur mit qstrings arbeiten kann, wird dann shortname mit/ohne longname in einen qstring konvertiert und in die combobox hinzugefügt
    }

    connect(ui->suchwort, &QLineEdit::textChanged, this, &MainWindow::SuchWortChanged);     // verbinden der signale mit den slots: hier nur mit suchwort (linedit) mit der methode "suchwortchanged", damit das listwidget auf änderung des suchworts entsprechend ändert

    SuchWortChanged();      // für initialisierung des listwidgets (hilft dagegen, dass nicht alte werte "vorerst" genommen werden)
}

MainWindow::~MainWindow()       // destruktor
{
    delete ui;
}

void MainWindow::SuchWortChanged()      // definition der funktion "suchwortchanged" (also was ausgeführt werden muss bei suchwortänderung)
{
    QString input = ui->suchwort->text();   // der text von suchwort wird in "input" gespeichert (als qstring da wir auf GUI-ebene sind)
    std::string suchwort_als_string = input.toStdString();  // input wird in ein string konvertiert, damit die suchfunktion damit arbeiten kann

    std::vector<bht::SStop> ergebnisse = beispiel.search(suchwort_als_string); // an dem network objekt wird die suchfunktion aufgerufen mit dem bestimmten suchwort, es resultiert der ergebnisvector (von SStops) mit den gesuchten haltestellen (also haltestellen die das suchwort beinhalten)

    ui->suchergebnisse->clear();    // entfernt die vorherigen haltestellen (wenn vorhanden) aus der listwidget (damit keine haltestellen bei suchwortänderung sich nicht "aufstapeln")

    for (const auto& stop : ergebnisse) // geh durch den ergebnisvector und via einer range based for schleife geh durch eine haltestelle
    {
           // konvertierte immer eines der gesuchten haltestellen (zurzeit als normaler string) in einen qstring, damit sie später via "additem" ins listwidget hinzugefügt werden kann
        ui->suchergebnisse->addItem(QString::fromStdString(stop.name));     // fügt die haltestelle (als qstring) jetzt zum listwidget
    }
}


