std::vector<bht::Stop> Network::getTravelPath(const std::string& fromStopId, const std::string& toStopId)   // verlangt methode für die berechnung des kürzesten weg von start bis ziel
{
   std::unordered_map<std::string, int> abstand;        // speicherung des abstands (im format: haltestelle und distanz-zahl)
   
   std::unordered_map<std::string, std::string> vorgaenger; // speicherung der vorgänger (im format: aktueller knoten und sein vorgänger)

   std::unordered_set<std::string> unbesucht;   // ein set für die noch nicht besucht wurden

   for (const auto& [stopId, stop] : stops) // durchgehen aller stops (einzelne einteilung der haltestelle in ihrer ID und dem rest ihres stop-objekts)
   {
    abstand[stopId] = INT_MAX;  // laut dem dijkstra wird die anfangsdistanz auf unendlich gesetzt, also packen wirs auf die größte int zahl (für jeden stop)
    unbesucht.insert(stopId);   // die stop markieren wir jetzt als unbesucht (indem wirs in den set packen)
   }
   if(abstand.find(fromStopId) == abstand.end() || abstand.find(toStopId) == abstand.end()) // falls entweder start oder ziel nicht existieren (also sie entsprechen dann "end()")
    {
        return{};   // returne was leeres
    }
    abstand[fromStopId] = 0;    // anfangspunkt bekommt die distanz 0

    while(!unbesucht.empty())   // solange unbesuchte stops existieren
    {
        std::string stelle_aktuell;     // speicherung für die aktuelle stelle
        int kleinster_abstand = INT_MAX;    // kleinste distanz ist größte INT_MAX
        for(const auto& stopId : unbesucht) //durchgehen aller unbesuchten stops
        {
            if(abstand[stopId] < kleinster_abstand) // wenn der aktuell ausgewählte abstand kleiner als unseren aktuell "kleinst"-gespeicherten abstand ist
            {
                kleinster_abstand = abstand[stopId];    // dann wird der aktuell "kleinst"-gespeicherte abstand zu diesem neuen abstand
                stelle_aktuell = stopId;    // diese neue stop wird gespeichert
            }
        }
        if(kleinster_abstand == INT_MAX)    // ist der kleinste abstand die größte int zahl (also keine übrigen haltestellen)
        {
            break;  // dann brich ab
        }
        unbesucht.erase(stelle_aktuell);    // die aktuelle stelle ist also nicht mehr besucht (entfernung aus "unbesucht")

        if(stelle_aktuell == toStopId)      // wenn wir am ziel sind
        {
            break;          // dann brich ab
        }

        auto nachbarn = getNeighbors(stelle_aktuell);   // alle nachbarn der aktuellen stellen werden gespeichert

        for(const auto& nachbar : nachbarn)     // durchgehen der nachbarn
        {
            if(unbesucht.find(nachbar) == unbesucht.end())  // wenn ein nachbar NICHT in unbesucht ist, also bereits besucht wurde
            {
                continue;       // dann gehen wir weiter
            }
            int geupdateter_abstand = abstand[stelle_aktuell] + 1;  // wir gehen einen schritt weiter (mit neuem abstand)

            if(geupdateter_abstand < abstand[nachbar])  // wenn der aktualisierte abstand nun kleiner erscheint
            {
                abstand[nachbar] = geupdateter_abstand;     // speichern wir diesen neuen abstand

                vorgaenger[nachbar] = stelle_aktuell;       // und ihren vorgänger
            }
        }
    }
    std::vector<bht::Stop> createdpath;     // jetzt für die rückkehr (speicherung der stops als vector)
    std::string currently_here = toStopId;  // das ziel ist unser startpunkt

    if(vorgaenger.find(currently_here) == vorgaenger.end() && fromStopId != toStopId)   // wenn kein möglicher pfad zum ziel zustande kommen kann
    {
        return{};       // dann returne was leeres
    }
    while(currently_here != fromStopId) // solange wir nicht beim start sind
    {
        try{
            createdpath.push_back(getStopById(currently_here));    // via try-catch-block versuche das stop-objekt mittels ihrer ID zu extrahieren
        }
        catch(const std::out_of_range&)     // wenn das nicht klappt
        {
            return{};   // returne was leeres
        }
        if(vorgaenger.find(currently_here) == vorgaenger.end())     // wenn wir von unserer aktuellen stelle keinen vorgänger finden können
        {
            return{};   // returne was leeres
        }
        currently_here = vorgaenger[currently_here];    // wenn ein vorgänger tatsächlich existiert, dann gehe einen schritt weiter (also hier "rückwärts")
    }  
    try{
        createdpath.push_back(getStopById(fromStopId)); // mittels try-catch-block : den startpunkt (also das objekt mittels ihrer ID) noch in den pfad hinzufügen
    }
    catch(const std::out_of_range&)     // wenn nicht klappt
    {
        return{};   // dann returne was leeres
    }
    std::reverse(createdpath.begin(), createdpath.end());   // mit unserer "rückwärts"-gegangenen journey, kehren wir die reihenfolge der haltestellen um, um die reihenfolge start zu ziel zu bekommen

    return createdpath; // printe den pfad
}
