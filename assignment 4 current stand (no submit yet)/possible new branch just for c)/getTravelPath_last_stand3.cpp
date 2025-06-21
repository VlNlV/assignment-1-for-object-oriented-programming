std::vector<bht::Stop> Network::getTravelPath(const std::string& fromStopId, const std::string& toStopId)   // verlangt methode für die berechnung des kürzesten weg von start bis ziel
{
   std::map<std::string, float> abstand;            // eine map für die speicherung im format (string, float) für die stopId und deren distanz
   std::map<std::string, std::string> vorgaenger;   // eine map für die vorgänger im format (string, string) für den vorgänger und der aktuellen position
   std::set<std::string> unbesucht;                 // ein set für die speicherung von stopIds die unbesucht sind

   for(const auto& [id, stop] : stops)      // durchgehen aller stops (einteilung in ihre ID und ihr rest des stop-objekts)
   {
    abstand[id] = std::numeric_limits<float>::infinity();   // setzung des abstandes für jeden stop (mittels ihrer ID) zu unendlich (so halt funktioniert der dijkstra algorithmus, denn er setzt alles auf infinity)
    unbesucht.insert(id);   // alle stops werden dann einzeln in die "unbesucht"-set reingesteckt
   }

   if(abstand.find(fromStopId) == abstand.end() || abstand.find(toStopId) == abstand.end()) // wenn entweder das ziel oder der anfang nicht in der "abstand"-map sind (also sie sind ungültig bzw. existieren nicht)
   {
        return{};   // dann returne was leeres
   }
   abstand[fromStopId] = 0.0f;  // der anfangsabstand wird auf 0 gesetzt (hier 0.0 wegen float)

   auto abstandsvergleicher = [&abstand](const std::string& a, const std::string& b)
   {                                                                            // lambda-funktion (selbstrecherche!!!) : hilft uns wenn wir zwei abstände vergleichen wollen
        return abstand[a] < abstand[b];     // überprüft ob eine distanz kleiner ist als die andere
   };

    while(!unbesucht.empty())       // solange es stops in "unbesucht" existieren
    {       
        auto kleinster = std::min_element(unbesucht.begin(), unbesucht.end(), abstandsvergleicher); // unter allen stops wird derjenige mit dem kleinsten abstand herausgefiltert
        if(kleinster == unbesucht.end())        // wenn überhaupt etwas existiert, der ein kleinster abstand sein kann
        {
            break;      // dann breake
        }

        std::string stelle_aktuell = *kleinster;        // der stop mit dem kleinsten abstand wird als aktuelle stelle gespeichert
        unbesucht.erase(kleinster);     // da wir ihn jetzt besuchen, wird er jetzt aus dem "unbesucht"-set entfernt

        if(abstand[stelle_aktuell] == std::numeric_limits<float>::infinity())   // wenn unsere aktuelle stelle einen unendlichen abstand hat
        {
            break;  // dann breake
        }

        if(stelle_aktuell == toStopId)      // wenn wir am ziel sind
        {
            break;      // dann breake
        }

        for(const auto& nachbar : getNeighbors(stelle_aktuell)) // geh durch alle nachbarn der aktuellen stelle durch
        {
            if(unbesucht.count(nachbar) == 0)       // wenn der nachbar nicht in "unbesucht" ist (also schon besucht wurde)
            {   
                continue;   // dann geh weiter
            }

            float geupdateterabstand = abstand[stelle_aktuell] + 1.0f;  // wir updaten unseren abstand zum nächsten nachbarn (also +1)

            if(geupdateterabstand < abstand[nachbar])   // wenn der neue abstand kürzer ist als den abstand, den wir zurzeit als kürzesten betrachten
            {
                abstand[nachbar] = geupdateterabstand;  // dann wird der neu kürzesten abstand zu diesem abstand
                vorgaenger[nachbar] = stelle_aktuell;   // die stelle wo wir waren, wird zum vorgänger (um vorherigen weg zu speichern)
            }
        }
    }
    std::vector<bht::Stop> createdpath; // vektor, der die endgültige journey speichert 
    auto reversegeher = vorgaenger.find(toStopId);  // wir gehen jetzt rückwärts von ziel zu start, also wird der vorgänger vom ziel zunächstmal gespeichert (aber nur die position)

    if(reversegeher == vorgaenger.end() && fromStopId != toStopId)  // wenn ziel und start keinen gemeinsamen weg haben aber beide auch gleichzeitig unterschiedliche orte sind
    {
        return{};   // returne was leeres
    }

    std::string aktuell_for_reverse = toStopId; // ziel wird gespeichert für die "rückkehr"-journey

    while(aktuell_for_reverse != fromStopId)    //  solange wir vom ziel nicht zum start gelangen sind
    {
        if(stops.find(aktuell_for_reverse) == stops.end())  // wenn die aktuelle position fürs zurückgehen nicht in den stops ist
        {
            return{};       // dann returne was leeres
        }
        createdpath.push_back(stops.at(aktuell_for_reverse));   // wenn die aktuelle position tatsächlich existiert, dann füge sie in den end-vektor ein

        auto vorgaenger_neu = vorgaenger.find(aktuell_for_reverse); // der neue vorgänger wird bestimmt
        if(vorgaenger_neu == vorgaenger.end())  // wenn kein vorgänger existiert
        {
            return{};       // dann returne was leeres
        }
        aktuell_for_reverse = vorgaenger_neu->second;   // wir gehen rückwärts weiter, also wird die neue aktuelle position der vorgänger
    }

    if(stops.find(fromStopId) != stops.end())   // wenn der start überhaupt in stops existiert
    {
        createdpath.push_back(stops.at(fromStopId));    // dann wird er auch in den end-vektor gesteckt
    }
    else    // wenn aber nicht
    {
        return{};   // dann returne was leeres
    }

    std::reverse(createdpath.begin(), createdpath.end());   // der end-vektor ist zurzeit ziel bis start, also machen wir's umgekehrt, damit es start bis ziel ist

    return createdpath; // wir returnen den neu sortierten end-vektor
}
