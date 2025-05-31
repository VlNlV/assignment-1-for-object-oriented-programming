#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , beispiel("C:/Users/varun/OneDrive/Desktop/SoSe25CODE/ubung-3-abgabe-so-se-25/allfiles") // network-objekt bekommt "zugriff" auf die files via wertzuweisung des datenpfads
{
    ui->setupUi(this);

    ui->comboBox_trips->setEnabled(false);
    ui->tableView->setEnabled(false);
    //ui->suchwort->setEnabled(false);


    for(const auto& route : beispiel.getRoutes())   // range based for schleife fürs durchgehen aller routes (in route wird temporär EINE route gespeichert)
    {
        std::string nameforcomboboxitem = beispiel.getRouteDisplayName(route);  // innerhalb einer route wird das anzeige-format (shortname mit/ohne longname) bestimmt und in "nameforcomboboxitem" gespeichert
        ui->comboBox->addItem(QString::fromStdString(nameforcomboboxitem));     // da die combobox nur mit qstrings arbeiten kann, wird dann shortname mit/ohne longname in einen qstring konvertiert und in die combobox hinzugefügt
    }

    connect(ui->suchwort, &QLineEdit::textChanged, this, &MainWindow::SuchWortChanged);     // verbinden der signale mit den slots: hier nur mit suchwort (linedit) mit der methode "suchwortchanged", damit das listwidget auf änderung des suchworts entsprechend ändert
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::RouteChanged); // routechanged wird getriggert, wenn sich der index ändert bzw. der user klickt auf eine andere linie
    connect(ui->comboBox_trips, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::TripChanged);

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

void MainWindow::RouteChanged()     // definition der funktion "routeChanged" für die linienwahlsänderung
{
    int chosenzeile = ui->comboBox->currentIndex();     // arbeiten mit index (also welche zeile der user geclickt hat), hier wird in "chosenzeile" der index de angeklickten option/zeile gespeichert (combobox indizes funktionieren wie array indizes!!!)

    if (chosenzeile < 0)        // fehlercheck (falls index JEMALS kleiner 0 ist)
    {
        return;             // returne nichts
    }
    //ui->suchwort->setEnabled(false);
    ui->comboBox_trips->setEnabled(true);

    bht::Route route = beispiel.getRoutes()[chosenzeile];       // funktionsergebnis von "getRoutes" ist ein vector, also kann via der indiz die zeile der linie (aus den routes) bestimmt werden
                                                                // die bestimmte zeile wird in "route" gespeichert
    std::vector<bht::Trip> trips = beispiel.getTripsForRoute(route.id); // die ID dieser route wird zum argument für "getTripsForRoute" und bestimmt nun alle trips zu dieser bestimmten routeID

    ui->comboBox_trips->clear();            // leert die combobox von alten einträgen

    for (const auto& trip : trips)          // range based for schleife: durchgehen der passenden trips zu der routeID
    {
        std::string tripNames = beispiel.getTripDisplayName(trip);  // jeder trip wird in die namenteile "shortname" bzw. "headsign" eingeteilt (also die namen werden "extrahiert")
        ui->comboBox_trips->addItem(QString::fromStdString(tripNames));     // mittels qstring konvertierung kann dann der jeweilige trip (in shortname/headsign) aus den "passenden trips" in die combobox hinzugefügt werden
    }
}

std::string TimeToStringConverter(const bht::GTFSTime& time)   // hilfsfunktion fürs konvertieren von GTFStime zu einem string
{
    char buffer[9];     // erstellung eines buffers (HH:MM:SS + den null terminator sind 9 chars)
    snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", time.hour, time.minute, time.second);    // formartierte text wird jetzt in "buffer" reingeschrieben
    return std::string(buffer);     //konvertierung des buffers in einen string (zum weiterverarbeiten)
}

void MainWindow::TripChanged(int index) // funktion für die fahrtsänderung (zweite combobox)
{
    if (index < 0)  // falls keine gültige linie (index kleiner 0) gewählt wird, dann beende
    {
        return;
    }
    int routeIndex = ui->comboBox->currentIndex();  // der aktuelle index aus der routes-combobox (erste) wird in routeindex gespeichert

    if (routeIndex < 0) // erneuter check, ob bei routes ein gültiger index gewählt wurde (wenn nicht, dann beende)
    {
        return;
    }
    //ui->suchwort->setEnabled(true);
    ui->tableView->setEnabled(true);

    bht::Route route = beispiel.getRoutes()[routeIndex];    // mittels des gültigen (ausgewählten) indizes wird nun die entsprechende route/linie bestimmt und in "route" gespeichert

    std::vector<bht::Trip> trips = beispiel.getTripsForRoute(route.id); // zu der ID der bestimmten route/linie werden nun die trips bestimmten und in "trips" (vector aus trip-objekten) gespeichert

    if ((size_t)index >= trips.size())  // check ob index größer gleich ist als die zahl an trips (indizes entsprechen ja der anzahl an zeilen/trips, also index > trips --> beende)
    {
        return;
    }


    bht::Trip selectedTrip = trips[index];  // der resultierende trip mittels der index wird jetzt in "selectedTrip" gespeichert

    std::vector<bht::StopTime> stopTimes = beispiel.getStopTimesForTrip(selectedTrip.id);   // aus diesem trip mittels ihrer ID wird werden nun die dazu gehörenden stop times bestimmt und in "stoptimes" gespeichert

    QStandardItemModel* model = new QStandardItemModel();       // erstellung der tabelle
    model->setColumnCount(4);                               // wir wollen ja 4 spalten --> columncount = 4
    model->setHeaderData(0, Qt::Horizontal, "Nr.");             // layout der spalten für die tabelle
    model->setHeaderData(1, Qt::Horizontal, "Name Haltestelle");
    model->setHeaderData(2, Qt::Horizontal, "Ankunftszeit");
    model->setHeaderData(3, Qt::Horizontal, "Abfahrtszeit");

    for (int row = 0; row < (int)stopTimes.size(); ++row)      // normale for schleife: durchgehen der stoptimes
    {
        const auto& stopTime = stopTimes[row];          // einzelnes stoptime "item" wird bestimmt
        bht::Stop stop = beispiel.getStopById(stopTime.stopId); // zu jedem stoptime "item" wird die ID bestimmt

        model->setItem(row, 0, new QStandardItem(QString::number(stopTime.stopSequence)));      // entsprechend werden die infos in den spalten eingesetzt

        model->setItem(row, 1, new QStandardItem(QString::fromStdString(stop.name)));

        model->setItem(row, 2, new QStandardItem(QString::fromStdString(TimeToStringConverter(stopTime.arrivalTime)))); // dazu noch: die zeiten werden via timetostringconverter von gtfstime in string konvertiert

        model->setItem(row, 3, new QStandardItem(QString::fromStdString(TimeToStringConverter(stopTime.departureTime))));
    }

    ui->tableView->setModel(model); // für die anzeige
}


