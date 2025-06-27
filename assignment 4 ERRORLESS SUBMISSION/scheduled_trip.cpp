#include "scheduled_trip.h"

namespace bht {

    NetworkScheduledTrip::NetworkScheduledTrip(const Trip& ganzetripobjekt, const std::vector<StopTime>& stops) : ganzetripobjekt(ganzetripobjekt), stoptimesvector(stops) 
    {
    }       // normale konstruktor mit initialisierung von dem trip objekt und dass der stoptimesvector die stops bekommt

    NetworkScheduledTrip::NetworkScheduledTrip(const NetworkScheduledTrip& anderes) : ganzetripobjekt(anderes.ganzetripobjekt), stoptimesvector(anderes.stoptimesvector) 
    {
    }       // kopier-konstruktor

    NetworkScheduledTrip::NetworkScheduledTrip(NetworkScheduledTrip&& anderes) : ganzetripobjekt(anderes.ganzetripobjekt) 
    {
        stoptimesvector = std::move(anderes.stoptimesvector);   // move konstruktor
    }

    NetworkScheduledTrip::iterator NetworkScheduledTrip::begin() const 
    {
        return iterator{this, 0};       // macht einen iterator auf das erste element
    }

    NetworkScheduledTrip::iterator NetworkScheduledTrip::end() const 
    {
    return iterator{this, stoptimesvector.size()};      // macht einen iterator HINTER dem letzten (mittels der kenntnis der größe von stoptimesvector)
    }

    NetworkScheduledTrip::iterator NetworkScheduledTrip::at(const std::string& stopId) const 
    {
        for (size_t laufvariable = 0; laufvariable < stoptimesvector.size(); laufvariable++)    // durchgehen des stoptimesvectors
        {
            if (stoptimesvector[laufvariable].stopId == stopId)     // wenn die gesuchte stopId innerhalb von stoptimesvector gefunden wird
            {
             return iterator{this, laufvariable};   // dann pack einen iterator genau da drauf
            }
        }

        return end();   // wenn for-schleife endet und somit die stopId nicht in stoptimesvector gefunden werden konnte, returne die end()-methode
    }

    const Trip& NetworkScheduledTrip::tripIDgetter() const 
    {
        return ganzetripobjekt;     // returnt das ganze tripobjekt
    }

    NetworkScheduledTrip::iterator::iterator(const NetworkScheduledTrip* gezeigtertrip, size_t index) : gezeigtertrip(gezeigtertrip), index(index) 
    {
    }       // initialisierung des iterators mit dem gezeigten trip und ihrer position

    NetworkScheduledTrip::iterator& NetworkScheduledTrip::iterator::operator++() 
    {
        index++;        // inkrementiert den index

        return *this;   // und returnt das stoptime, bei dem sie gerade ist (nach dem inkrement)
    }

    NetworkScheduledTrip::iterator NetworkScheduledTrip::iterator::operator++(int) 
    {
        iterator bevorinkrement{gezeigtertrip, index};  // speichert den iterator zurzeit

        ++(*this);      // inkrementiert den index

        return bevorinkrement;      // returnt den iterator vor dem inkrement
    }

    NetworkScheduledTrip::iterator& NetworkScheduledTrip::iterator::operator--() 
    {
        index--;    // dekrementiert den iterator

        return *this;       // returnt den iterator
    }

    NetworkScheduledTrip::iterator NetworkScheduledTrip::iterator::operator--(int) 
    {
        iterator bevordekrement{gezeigtertrip, index};  // speichert den iterator zurzeit

        --(*this);      // dekrementiert den iterator

        return bevordekrement;  // returnt den iterator vor dem dekrement
    }

    NetworkScheduledTrip::iterator::reference NetworkScheduledTrip::iterator::operator*() const 
    {
        return gezeigtertrip->stoptimesvector[index];       // returnt die referenz des aktuellen iterators mittels reference
    }

    const std::string& NetworkScheduledTrip::iterator::getTripId() const 
    {
        return gezeigtertrip->stoptimesvector[0].tripId;        // returnt die tripID des ersten elements aus dem vektor 
    }

    unsigned int NetworkScheduledTrip::iterator::getStopSequence() const 
    {
        return index;       // returnt die position des iterators zurzeit (also hier eher die "stopsequence")
    }  

    NetworkScheduledTrip::iterator::pointer NetworkScheduledTrip::iterator::operator->() 
    {
        return &(gezeigtertrip->stoptimesvector[index]);        // zugriff auf den iterator mittels pointer
    }

    bool operator==(const NetworkScheduledTrip::iterator& a, const NetworkScheduledTrip::iterator& b) 
    { 
        return a.getTripId() == b.getTripId() && a.getStopSequence() == b.getStopSequence();    // checkt auf gleichheit, ob zwei fahrten gleiche position (stopsequence) haben
    }

    bool operator!=(const NetworkScheduledTrip::iterator& a, const NetworkScheduledTrip::iterator& b) 
    { 
        return !(a == b);   // returnt das gegenteil von der obigen (also ob die ungleich sind)
    }

}
