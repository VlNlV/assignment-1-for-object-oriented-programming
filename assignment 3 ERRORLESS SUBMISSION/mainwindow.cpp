#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstdio>           // für snprintf

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , beispiel("C:/Users/varun/OneDrive/Desktop/SoSe25CODE/ubung-3-abgabe-so-se-25/allfiles") // network-objekt bekommt "zugriff" auf die files via wertzuweisung des datenpfads
{
    ui->setupUi(this);

    ui->suchwort->setEnabled(false);    // deaktivierung des suchfelds (am anfang)
    ui->comboBox_trips->setEnabled(false);  // deaktiviering der zweiten combobox + tabelle (solange keine linie ausgewählt wurde)
    ui->tableView->setEnabled(false);


    for(const auto& route : beispiel.getRoutes())   // range based for schleife fürs durchgehen aller routes (in route wird temporär EINE route gespeichert)
    {
        std::string nameforcomboboxitem = beispiel.getRouteDisplayName(route);  // innerhalb einer route wird das anzeige-format (shortname mit/ohne longname) bestimmt und in "nameforcomboboxitem" gespeichert
        ui->comboBox->addItem(QString::fromStdString(nameforcomboboxitem));     // da die combobox nur mit qstrings arbeiten kann, wird dann shortname mit/ohne longname in einen qstring konvertiert und in die combobox hinzugefügt
    }

    connect(ui->suchwort, &QLineEdit::textChanged, this, &MainWindow::SuchWortChanged);     // verbinden der signale mit den slots: hier nur mit suchwort (linedit) mit der methode "suchwortchanged", damit das listwidget auf änderung des suchworts entsprechend ändert
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &MainWindow::RouteChanged); // routechanged wird getriggert, wenn sich der index ändert bzw. der user klickt auf eine andere linie
    connect(ui->comboBox_trips, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::TripChanged);

    //SuchWortChanged();      // für initialisierung des listwidgets (hilft dagegen, dass nicht alte werte "vorerst" genommen werden)
}

MainWindow::~MainWindow()       // destruktor
{
    delete ui;
}

std::string TimeToStringConverter(const bht::GTFSTime& time)   // hilfsfunktion fürs konvertieren von GTFStime zu einem string
{
    char buffer[9];     // erstellung eines buffers (HH:MM:SS + den null terminator sind 9 chars)
    snprintf(buffer, sizeof(buffer), "%02u:%02u:%02u", time.hour, time.minute, time.second);    // formartierte text wird jetzt in "buffer" reingeschrieben
    return std::string(buffer);     //konvertierung des buffers in einen string (zum weiterverarbeiten)
}

void MainWindow::SuchWortChanged()  // funktion definition für wenn sich das suchwort ändert --> hat jetzt einfluss auf anzeige der tabelle
{
    QString input = ui->suchwort->text();   // aktuelle text des lineedits wird in "input" gespeichert (qstring, da wir in GUI-ebene sind)
    std::string suchwort_als_string = input.toStdString();  // da wir aber mit dem text mittels anderer funktionen arbeiten wollen, konvertieren wir es in einen string und speichern es in "suchwort_als_string"

    int indexvonroute = ui->comboBox->currentIndex();  // der index der user-gewählten linie/route wird bestimmt und in "indexvonroute" gespeichert
    int indexvontrip = ui->comboBox_trips->currentIndex(); // der index der user-gewählten fahrt/trip wird bestimmt und in "indexvontrip" gespeichert

    if (indexvonroute < 0 || indexvontrip < 0)    // wenn irgendeiner der beiden eine index kleiner 0 ist (also keine linie/trip entspricht)
    {
        ui->tableView->setModel(nullptr);   // zeige keine daten in der tabelle und beende
        return;
    }

    bht::Route route = beispiel.getRoutes()[indexvonroute];    // mittels der funktion "getRoutes" und der "indexvonroute" wird die einzelne linie (in form des route-objekts) in "route" gespeichert
    std::vector<bht::Trip> trips = beispiel.getTripsForRoute(route.id); //die "id" der einzelnen linie wird nun in die funktion "getTripsForRoute" gesteckt, damit die fahrten/trips zu der linie bestimmt werden können

    if ((size_t)indexvontrip >= trips.size())  // kurzer check, ob der indexvontrip kleiner gleich ist als die anzahl an trips (e.g. tripindex = 19, bei 5 trips) --> ungültig
    {
        ui->tableView->setModel(nullptr);   // zeige keine daten in der tabelle und beende
        return;
    }

    bht::Trip chosentrip = trips[indexvontrip];  // mittels von "tripIndex" innerhalb des vectors "trips" wird der bestimmte trip ausgewählt und in "chosentrip" gespeichert

    std::vector<bht::StopTime> StopTimesMitFilter = beispiel.searchStopTimesForTrip(suchwort_als_string, chosentrip.id);   // jetzt die nutzung von "searchStopTimesForTrip", in der das suchwort und die "chosentrip" des ausgewählten trips die anzeige der tabelle beeinflussen
                                                                                                                            // bzw. die haltestellen, die mit "id" und "suchwort" übereinstimmen, werden in "filteredStopTimes" gespeichert
    QStandardItemModel* model = new QStandardItemModel();           // erstellung der tabelle
    model->setColumnCount(4);                               // spaltenanzahlbestimmung
    model->setHeaderData(0, Qt::Horizontal, "Nr.");                 // einzelne spaltentitel
    model->setHeaderData(1, Qt::Horizontal, "Name Haltestelle");
    model->setHeaderData(2, Qt::Horizontal, "Ankunftszeit");
    model->setHeaderData(3, Qt::Horizontal, "Abfahrtszeit");

    for (int tabellenzeile = 0; tabellenzeile < (int)StopTimesMitFilter.size(); ++tabellenzeile)   // normale for schleife : durchgehen der haltestellen, die mit "suchwort" und "id" übereinstimmen
    {
        const auto& stopTime = StopTimesMitFilter[tabellenzeile];              //  einzelne zeile der gefilterten stoptimes wird in "stoptime" gespeichert
        bht::Stop stop = beispiel.getStopById(stopTime.stopId);     // dazu wird das entprechende "stop"-objekt mittels der stopId ermittelt

        model->setItem(tabellenzeile, 0, new QStandardItem(QString::number(stopTime.stopSequence)));      // die tabelle wird gefüllt
        model->setItem(tabellenzeile, 1, new QStandardItem(QString::fromStdString(stop.name)));
        model->setItem(tabellenzeile, 2, new QStandardItem(QString::fromStdString(TimeToStringConverter(stopTime.arrivalTime))));     // nutzung von "timetostringconverter" für die zeiten
        model->setItem(tabellenzeile, 3, new QStandardItem(QString::fromStdString(TimeToStringConverter(stopTime.departureTime))));
    }

    ui->tableView->setModel(model); // für die anzeige
}

void MainWindow::RouteChanged()     // definition der funktion "routeChanged" für die linienwahlsänderung
{
    int chosenzeile = ui->comboBox->currentIndex();     // arbeiten mit index (also welche zeile der user geclickt hat), hier wird in "chosenzeile" der index de angeklickten option/zeile gespeichert (combobox indizes funktionieren wie array indizes!!!)

    if (chosenzeile < 0)        // fehlercheck (falls index JEMALS kleiner 0 ist)
    {
        ui->comboBox_trips->setEnabled(false);  // wenn die ausgewählte linie "nicht existiert" dann deaktiviere alles (weil keine daten dazu existieren)
        ui->suchwort->setEnabled(false);
        ui->tableView->setEnabled(false);
        ui->comboBox_trips->clear();            // cleare die combobox
        return;             // beende
    }
    ui->comboBox_trips->setEnabled(true);   //aktiviere die combobox über die fahrten, wenn sich eine route geändert hat
    ui->suchwort->setEnabled(false);           // haltestellensuche + tabelle bleiben deaktiviert, da wir noch nach einer gültigen fahrt checken müssen
    ui->tableView->setEnabled(false);

    bht::Route route = beispiel.getRoutes()[chosenzeile];       // funktionsergebnis von "getRoutes" ist ein vector, also kann via der indiz die zeile der linie (aus den routes) bestimmt werden
                                                                // die bestimmte zeile wird in "route" gespeichert
    std::vector<bht::Trip> trips = beispiel.getTripsForRoute(route.id); // die ID dieser route wird zum argument für "getTripsForRoute" und bestimmt nun alle trips zu dieser bestimmten routeID

    ui->comboBox_trips->clear();            // leert die combobox von alten einträgen

    for (const auto& trip : trips)          // range based for schleife: durchgehen der passenden trips zu der routeID
    {
        std::string tripnames = beispiel.getTripDisplayName(trip);  // jeder trip wird in die namenteile "shortname" bzw. "headsign" eingeteilt (also die namen werden "extrahiert")
        ui->comboBox_trips->addItem(QString::fromStdString(tripnames));     // mittels qstring konvertierung kann dann der jeweilige trip (in shortname/headsign) aus den "passenden trips" in die combobox hinzugefügt werden
    }
}

void MainWindow::TripChanged(int index) // funktion für die fahrtsänderung (zweite combobox)
{
    if (index < 0)  // falls keine gültige fahrt (index kleiner 0) gewählt wird, dann beende
    {
        ui->suchwort->setEnabled(false);    // wenn ausgewählte fahrt nicht gültig ist, dann deaktiviere haltestellensuche + tabelle
        ui->tableView->setEnabled(false);
        return;                             // beende
    }
    int routeindex = ui->comboBox->currentIndex();  // der aktuelle index aus der routes-combobox (erste) wird in routeindex gespeichert

    if (routeindex < 0) // erneuter check, ob bei routes ein gültiger index gewählt wurde (wenn nicht, dann beende)
    {
        ui->suchwort->setEnabled(false);    // bei ungültiger routeIndex, deaktiviere haltestellensuche + tabelle
        ui->tableView->setEnabled(false);
        return;
    }
    ui->suchwort->setEnabled(true);     // wenn sie doch gültig ist, dann aktiviere beide
    ui->tableView->setEnabled(true);

    bht::Route route = beispiel.getRoutes()[routeindex];    // mittels des gültigen (ausgewählten) indizes wird nun die entsprechende route/linie bestimmt und in "route" gespeichert

    std::vector<bht::Trip> trips = beispiel.getTripsForRoute(route.id); // zu der ID der bestimmten route/linie werden nun die trips bestimmten und in "trips" (vector aus trip-objekten) gespeichert

    if ((size_t)index >= trips.size())  // check ob index größer gleich ist als die zahl an trips (indizes entsprechen ja der anzahl an zeilen/trips, also index > trips --> beende)
    {
        return;
    }


    bht::Trip chosentrip = trips[index];  // der resultierende trip mittels der index wird jetzt in "chosentrip" gespeichert

    std::vector<bht::StopTime> stopTimes = beispiel.getStopTimesForTrip(chosentrip.id);   // aus diesem trip mittels ihrer ID wird werden nun die dazu gehörenden stop times bestimmt und in "stoptimes" gespeichert

    QStandardItemModel* model = new QStandardItemModel();       // erstellung der tabelle
    model->setColumnCount(4);                               // wir wollen ja 4 spalten --> columncount = 4
    model->setHeaderData(0, Qt::Horizontal, "Nr.");             // layout der spalten für die tabelle
    model->setHeaderData(1, Qt::Horizontal, "Name Haltestelle");
    model->setHeaderData(2, Qt::Horizontal, "Ankunftszeit");
    model->setHeaderData(3, Qt::Horizontal, "Abfahrtszeit");

    for (int tabellenzeile = 0; tabellenzeile < (int)stopTimes.size(); ++tabellenzeile)      // normale for schleife: durchgehen der stoptimes
    {
        const auto& stopTime = stopTimes[tabellenzeile];          // einzelnes stoptime "vector item" wird bestimmt
        bht::Stop stop = beispiel.getStopById(stopTime.stopId); // zu jedem stoptime "vector item" wird das entsprechende stop-objekt mittels ihrer ID in "stop" gespeichert

        model->setItem(tabellenzeile, 0, new QStandardItem(QString::number(stopTime.stopSequence)));      // entsprechend werden die infos in den spalten eingesetzt

        model->setItem(tabellenzeile, 1, new QStandardItem(QString::fromStdString(stop.name)));

        model->setItem(tabellenzeile, 2, new QStandardItem(QString::fromStdString(TimeToStringConverter(stopTime.arrivalTime)))); // dazu noch: die zeiten werden via timetostringconverter von gtfstime in string konvertiert

        model->setItem(tabellenzeile, 3, new QStandardItem(QString::fromStdString(TimeToStringConverter(stopTime.departureTime))));
    }

    ui->tableView->setModel(model); // für die anzeige
}


