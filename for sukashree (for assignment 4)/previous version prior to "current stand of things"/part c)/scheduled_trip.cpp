#include "scheduled_trip.h"
#include <vector>

namespace bht {

NetworkScheduledTrip::NetworkScheduledTrip(const std::string& einetripID, const std::vector<StopTime>& stoptimesvector) : stoptimesvector(stoptimesvector), einetripID(einetripID){}    // konstruktor für das objekt "NetworkScheduledTrip" (mit initialisierung von einer tripID und eienr liste von stoptimes)

const std::string& NetworkScheduledTrip::tripIDgetter()const    // getter methode für tripID
{
  return einetripID;
}

const std::vector<StopTime>& NetworkScheduledTrip::stoptimesgetter()const   // getter methode für stoptimes
{
    return stoptimesvector;
}

NetworkScheduledTrip::iterator NetworkScheduledTrip::begin()const   // anfangsiteration im stoptime an index 0
{
  return iterator(this, 0);   // returnt also den iterator an anfangsposition
}

NetworkScheduledTrip::iterator NetworkScheduledTrip::end()const
{
  return iterator(this, stoptimesvector.size());    // gibt iterator hinten dem letzten element (so funktioniert halt ".end()")
}

NetworkScheduledTrip::iterator NetworkScheduledTrip::stopfinderbyitsID(const std::string& stopID)const  //die suche nach einer stopId in stoptimes und setzung eines iterators dort (übergabe einer stopId)
{
  for(size_t i = 0; i < stoptimesvector.size(); ++i)    // durchgehen der stoptimes
  {
    if(stoptimesvector[i].stopId == stopID)   // wenn stopId in stoptimes mit der input-stopId übereinstimmt
    {
      return iterator(this, i);   // setze dort den iterator
    }
  }
  return end();   // wenn nicht gefunden mach end()
}

NetworkScheduledTrip::iterator::iterator(const NetworkScheduledTrip* gezeigtertrip, size_t index) : gezeigtertrip(gezeigtertrip), index(index) {}   // konstruktor des iterators (initialisierung von gezeigtertrip und index)

NetworkScheduledTrip::iterator& NetworkScheduledTrip::iterator::operator++()
{
  ++index;
  return *this;     // inkrementiert und gibt neuen zeiger zurück 
}

NetworkScheduledTrip::iterator NetworkScheduledTrip::iterator::operator++(int)
{
  iterator hello = *this;   // aktueller zeiger in "hello" (mir ist kein besserer name eingefallen)
  ++(*this);              // inkrementiert den zeiger
  return hello;     // vorherige zeiger wird returnt
}

NetworkScheduledTrip::iterator& NetworkScheduledTrip::iterator::operator--()
{
  --index;        //dekrementiert index
  return *this;   // gibt veränderten zeiger zurück
}

NetworkScheduledTrip::iterator NetworkScheduledTrip::iterator::operator--(int)
{
  iterator hello = *this;   // aktueller zeiger in "hello" 
  --(*this);              //dekrementiert zeiger
  return hello;     // returnt den vorherigen zeiger
}

const StopTime& NetworkScheduledTrip::iterator::operator*()const
{
  return gezeigtertrip->stoptimesvector[index];   // mit dereferenzierung wird das aktuelle element (stoptime) zugreifbar (rückgabe als referenz)
}

const StopTime* NetworkScheduledTrip::iterator::operator->()
{
  return &(gezeigtertrip->stoptimesvector[index]);    //pfeiloperator auf stoptime-objekt (für zugriff auf felder mit pointer)
}

const std::string& NetworkScheduledTrip::iterator::getTripId()const
{
  return gezeigtertrip->einetripID;   // returnt die tripID des iterators
}

unsigned int NetworkScheduledTrip::iterator::getStopSequence()const
{
  return gezeigtertrip->stoptimesvector[index].stopSequence;      // returnt die stopsequence (in der reihenfolge) des iterators 
}

bool operator==(const NetworkScheduledTrip::iterator& a, const NetworkScheduledTrip::iterator& b) {   // war bereits aus aufgaben-vorlage vorgegeben
  return a.getTripId() == b.getTripId() && a.getStopSequence() == b.getStopSequence();
}

bool operator!=(const NetworkScheduledTrip::iterator& a, const NetworkScheduledTrip::iterator& b) {  // war bereits aus aufgaben-vorlage vorgegeben
  return !(a == b); 
}

}
