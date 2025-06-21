#include <iostream>
#include "network.h" 

int main() {
    std::string verzeichnismitdendateien = "C:\\Users\\varun\\OneDrive\\Desktop\\SoSe25CODE\\ubung-4-abgabe-so-se-25\\GTFSTest";    // damit das objekt dann zugriff auf die dateien hat
    Network beispiel(verzeichnismitdendateien);     // objekt bekommt zugriff auf dateien

    /*std::string beispielfuerbahnhof = "de:11000:900003103"; // wir definieren eine beispiel id (der tiergarten bahnhof)

    std::cout << "ein test für ein bahnhofbeispiel mit input: " << beispielfuerbahnhof << std::endl;
    
    auto passenderesultszutest1 = beispiel.getStopsForTransfer(beispielfuerbahnhof);    // passenderesultszutest1 enthält also tiergarten bahnhof ID und ihre zugehörigen gleise IDs auch
    
    for (const auto& stop : passenderesultszutest1)     // wir loopen durch unsere ergebnisse und printen sie
    {
        std::cout << stop.id << " , " << stop.name << std::endl;
    }
    
    std::string beispielfuergleis = "de:11000:900003103:1:51";      // noch ein beispiel aber mit einem einzelnen tiergarten gleis

    std::cout << std::endl;
    
    std::cout << "ein test für gleisbeispiel mit input: " << beispielfuergleis << std::endl;
    
    auto passenderesultszutest2 = beispiel.getStopsForTransfer(beispielfuergleis);
    
    for (const auto& stop : passenderesultszutest2) {       // tiergarten gleis, weitere zugehörige gleise und ihr zugehöriger bahnhof wird geprintet
        std::cout << stop.id << " , " << stop.name << std::endl;
    }
    */
    /*std::string beispielfuerinputgleis = "de:12073:900341111:1:50";     // ein beispielgleis wird gegeben

    std::unordered_set<std::string> allenachbarn = beispiel.getNeighbors(beispielfuerinputgleis);   // wir holen uns die nächstkommenden nachbarn des beispielgleises

    std::cout << "hier sind die nachbarn von: " << beispielfuerinputgleis << std::endl; // printen der nachbarn
    for (const auto& einezelnenachbarID : allenachbarn)     // durchgehen der nachbarn
    {
        bht::Stop stop = beispiel.getStopById(einezelnenachbarID);  // für jeden nachbarhn speichern wir ihre ID
        std::cout << einezelnenachbarID << " , " << stop.name << std::endl;    // wir printen dazu die ID mit namen
    }
    */
    std::string startHaltestelle = "de:12073:900341110:1:50";
    std::string zielHaltestelle = "de:12073:900340004:1:50";

    std::vector<bht::Stop> journey = beispiel.getTravelPath(startHaltestelle, zielHaltestelle); // speichert den weg von sart bis ziel

    if (journey.empty())        //wenn weg nicht existiert, dann printe das hier
    {
        std::cout << "bruh es gibt keinen weg von " << startHaltestelle << " bis " << zielHaltestelle << std::endl;
    } else {                                                                        // wenn gültiger weg existiert
        std::cout << "yo bro ich hätte nen weg gefunden: " << std::endl;        // dann printe das
        for (const auto& stop : journey) 
        {                                                 // und gehe durch jeden einzelnen stop des weges und printe ihn
            std::cout << stop.name << " mit der ID: " << stop.id << std::endl;
        }
    }

    return 0;
}
