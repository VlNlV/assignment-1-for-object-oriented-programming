#pragma once
// DIE ENGLISCHEN KOMMENTARE AUS DER AUFGABEN-VORLAGE HABE ICH DRIN GELASSEN (fyi)
#include "types.h"
#include <iterator>
#include <vector>

namespace bht {

/**
 * Defines a class representing a single trip with convenient accessor get methods to 
 * related route, stop and stop times information
 */
class NetworkScheduledTrip {
  private:
    // TODO: THIS CLASS IS INCOMPLETE AND YOU NEED TO IMPLEMENT THE REQUIRED ATTRIBUTES
    std::vector<StopTime> stoptimesvector;  // speicherung der stoptimes haltestellen (in fahrt-reihenfolge)
    std::string einetripID;   // speicherung der trip id (aus stoptimes)


  public:

  NetworkScheduledTrip(const std::string& einetripID, const std::vector<StopTime>& stoptimesvector);  // konstruktur fürs erstellen NetworkScheduledTrip objekts (mit übergabe einer tripID und der liste der stoptime objekte)

  const std::vector<StopTime>& stoptimesgetter() const;   // getter methode für stoptimes
  
  const std::string& tripIDgetter() const;    // getter methode für trip ID

    /**
     * Define an iterator to navigate the stops of a scheduled trip
     */
    class iterator {
      private:
        // TODO: THIS CLASS IS INCOMPLETE AND YOU NEED TO IMPLEMENT THE REQUIRED ATTRIBUTES
          const NetworkScheduledTrip* gezeigtertrip;    // pointer auf den trip den der iterator gerade durchläuft (er zeigt somit auf den trip)
          size_t index;     // für die aktuelle position
        
      public:
        // Define properties for this iterator
        using iterator_category = std::bidirectional_iterator_tag;      // war aus aufgaben-vorlage vorgegeben
        using difference_type   = int;
        using value_type        = const StopTime;
        using pointer           = const StopTime*; 
        using reference         = const StopTime&;
        // TODO: THIS CLASS IS INCOMPLETE AND YOU NEED TO IMPLEMENT THE REQUIRED ATTRIBUTES

            iterator(const NetworkScheduledTrip* gezeigtertrip, size_t index);    // konstruktor für den iterator (zeigt auf einen trip an position "index")
        /**
         * Move the iterator to the next stop in this trip
         */
        iterator& operator++();    // war aus aufgaben-vorlage vorgegeben
        iterator operator++(int);    // war aus aufgaben-vorlage vorgegeben

        /**
         * Move the iterator to the previous stop in this trip
         */
        iterator& operator--();    // war aus aufgaben-vorlage vorgegeben
        iterator operator--(int);    // war aus aufgaben-vorlage vorgegeben
        
        /**
         * Return the current stop time this iterator points to
         * HINT operator* is the dereference operator; NOT the multiplication operator
        */
        const StopTime& operator*() const;     // war aus aufgaben-vorlage vorgegeben

        /**
         * Return a pointer to the stop time the current iterator points to
         */
        const StopTime* operator->();    // war aus aufgaben-vorlage vorgegeben

        /**
         * Return the referenced trip of the current stop 
         */
        const std::string& getTripId() const;    // war aus aufgaben-vorlage vorgegeben

        /**
         * return the current stop sequence number on the trip
         * HINT: This sequence number might be invalid to indicate the end of a trip
         */
        unsigned int getStopSequence() const;      // war aus aufgaben-vorlage vorgegeben

        /**
         * Two stops are considered equal if they are in the same trip
         * and represent the same stop in the sequence of stops
         */
        friend bool operator==(const iterator& a, const iterator& b);      // war aus aufgaben-vorlage vorgegeben
        friend bool operator!= (const iterator& a, const iterator& b);     // war aus aufgaben-vorlage vorgegeben
    };

    iterator stopfinderbyitsID(const std::string& stopID) const;    // funktion fürs finden des iterators für eine bestimmte haltestelle (übergabe einer stopID)

    /**
     * Create a new iterator representing the start of a trip
     */
    iterator begin() const;    // war aus aufgaben-vorlage vorgegeben

    /**
     * Create a new iterator pointing beyond the last element of the trip
     */
    iterator end() const;      // war aus aufgaben-vorlage vorgegeben
};

}
