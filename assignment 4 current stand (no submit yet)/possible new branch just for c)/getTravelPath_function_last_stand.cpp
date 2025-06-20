std::vector<bht::Stop> Network::getTravelPath(const std::string& fromStopId, const std::string& toStopId)   // verlangt methode für die berechnung des kürzesten weg von start bis ziel
{
    std::unordered_map<std::string, int> entfernung;    // unordered map (eingeteilt mit "string" und "int") als distanz (also die distanz als int und die haltestelle-ID als string)
    std::unordered_map<std::string, std::string> letzte;    // unordered map (eingeteilt mit "string" und "string") als vorgänger knoten (einmal wird gespeichert, wer der vorgänger war und wohin es uns gebracht hat bzw. der aktuelle knoten)
    std::unordered_set<std::string> bereitsbesucht;         // unordered set für die bereits besuchten knoten (ist nur ein set, denn wir wollen nur checken ob etwas in "bereitsbesucht" ist)

    if (fromStopId == toStopId) 
    {
        if (stops.find(fromStopId) != stops.end()) 
        {
            return { getStopById(fromStopId) };
        }
        else 
        {
            return {};
        }
    }

    using PQEntry = std::pair<int, std::string>;        // warteschlangen-datenformat fürs zusammensetzen von entfernung und haltestellen-ID (selbstrecherche!!!)

    std::priority_queue<PQEntry, std::vector<PQEntry>, std::greater<PQEntry>> prioschlange; // erstellung der prioritätswarteschlange, in der immer die kleinsten werte herausgefiltert werden sollten (also die kleinsten entfernungen) (selbstrecherche!!!)
    prioschlange.push(std::make_pair(0, fromStopId));       // einfügen des startknotens in die warteschlange (initialisierung mit 0, da start auf sich selbst 0 entfernung hat)
    entfernung[fromStopId] = 0;     // zuweisung, das die aktuelle entfernung zum start 0 ist

    while(!prioschlange.empty())    // while schleife, die so lange aktiv ist bis warteschlange leer ist
    {
        PQEntry naechsterknoten = prioschlange.top();   // gibt die kleinste distanz aus der warteschlange zurück (wir gucken uns den nächsten knoten an)
        prioschlange.pop();                             // das wird nun aus der warteschlange gelöscht

        int vonstartbisaktuellerknoten = naechsterknoten.first;     // hier wird die entfernung "vom start bis zum aktuellen knoten" gespeichert
        std::string aktuell = naechsterknoten.second;           //die haltestellen-ID des aktuellen knotens

        if(bereitsbesucht.find(aktuell) != bereitsbesucht.end())    // wenn der aktuelle knoten bereits besucht wurde, dann gehen wir einfach weiter
        {
            continue;
        }   
        bereitsbesucht.insert(aktuell);     // wenn nicht, dann inserten wir die haltestellen-ID des aktuellen knotens in die "bereitsbesucht"-set

        if(aktuell == toStopId) // wenn wir jemals das ende erreichen, beende
        {
            break;
        }
        
        std::unordered_set<std::string> nachbarn = getNeighbors(aktuell);   // wir holen uns die nachbarn des aktuellen knotens
        for (const std::string& nachbar : nachbarn)     // wir gehen durch alle nachbarn durch und gucken uns jeden einzelnen nachbarn an
        {
            if (bereitsbesucht.find(nachbar) != bereitsbesucht.end())   // wenn der ausgewählte nachbar bereits besucht wurde, dann gehen wir einfach weiter
            {
                continue;
            }

            int neuerabstandzuneuemnachbar = vonstartbisaktuellerknoten + 1;    // wir updaten die entfernung fürs übergehen zum nächsten knoten
            if (entfernung.find(nachbar) == entfernung.end() || neuerabstandzuneuemnachbar < entfernung[nachbar])   // wenn der nachbar bisher keine entfernung gespeichert bekam oder die neue entfernung kürzer ist als die vorherige, dann gehen wir weiter
            {
                entfernung[nachbar] = neuerabstandzuneuemnachbar;   // wir speichern die (wenn vorhanden) kürzere entfernung
                letzte[nachbar] = aktuell;      // wir merken uns den vorgänger (für den rückweg)
                prioschlange.push(std::make_pair(neuerabstandzuneuemnachbar, nachbar)); // aktualisierter nachbar mit aktualisierter entfernung wird in die prio-warteschlange gesteckt
            }
        }

    }
    if (letzte.find(toStopId) == letzte.end() && fromStopId != toStopId) // wenn kein möglicher weg zustande kommt und wir nicht am ziel sind, dann returne nichts
    {
        return {};
    }

    std::vector<std::string> stationIDsaufweg;      // speicherung für die haltestellen-IDs auf dem weg
    std::string aktuell = toStopId;         // der aktuelle knoten ist jetzt das ziel
    std::unordered_set<std::string> besucht_reverse;    // der bereitsbesucht-checker aber nur fürs zurückgehen

    while(aktuell != fromStopId)        // solange wir nicht beim start sind
    {
        if (besucht_reverse.count(aktuell)) // wenn der aktuelle knoten bereits besucht wurde (beim zurückgehen)
        {
            return{};   // brich ab
        }
        besucht_reverse.insert(aktuell);        // dieser (aktuelle) knoten wird in den set gesteckt

        stationIDsaufweg.push_back(aktuell);    // füge den aktuellen knoten in den vektor

        auto vorgaengerchecker = letzte.find(aktuell);  // suche der vorgänger des aktuellen knotens in der "letzte"-map
        if(vorgaengerchecker == letzte.end())   // wenn kein vorgänger gefunden wurde
        {
            return{};           // brich ab
        }
        aktuell = vorgaengerchecker->second;    // der aktuelle knoten wird der vorgänger beim zurückgehen
    }
    stationIDsaufweg.push_back(fromStopId); // und füge den startknoten ebenfalls in den vektor 

    std::reverse(stationIDsaufweg.begin(), stationIDsaufweg.end());     // da wir den vektor in der ursprünglichen richtung haben wollten aber nun "rückwärts" gegangen sind, machen wir jetzt mit "reverse" die richtung wieder zurück (also wieder start zu ziel)

    std::vector<bht::Stop> ergebnis;                // erstellung des vektors für die ergebnisse
    for(const auto& einzelne_ID : stationIDsaufweg)     // durchgehen der einzelnen haltestellen auf dem weg von start zu ziel 
    {
        try {
        if (stops.find(einzelne_ID) == stops.end())  
        {
            return {}; 
        }    
            ergebnis.push_back(getStopById(einzelne_ID));   // ihr stop-objekt wird in den ergebnisvektor gesteckt
            } 
            catch (const std::out_of_range&) 
            {
                 return {};
            }
    }
    return ergebnis;                    // returne das ergebnis
}
