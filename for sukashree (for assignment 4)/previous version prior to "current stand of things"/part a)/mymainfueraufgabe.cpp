#include <iostream>
#include "network.h" 

int main() {
    std::string verzeichnismitdendateien = "C:\\Users\\varun\\OneDrive\\Desktop\\SoSe25CODE\\ubung-4-abgabe-so-se-25\\allfiles";    // damit das objekt dann zugriff auf die dateien hat
    Network beispiel(verzeichnismitdendateien);     // objekt bekommt zugriff auf dateien

    std::string beispielfuerbahnhof = "de:11000:900003103"; // wir definieren eine beispiel id (der tiergarten bahnhof)

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
    
    return 0;
}
