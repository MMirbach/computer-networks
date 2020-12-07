#include "clinicSimulator.h"
#include <queue>

void Arrival::processEvent() {
    ClinicSimulator* sim = sim->getInstance();
    sim->addQueueTime(_time);
    if (sim->stays()) {
        sim->addVaccinated();
        if (sim->_waiting.empty()) // no-one is waiting, can be vaccinated right away
            sim->addEvent(new GetVaccinated(_time));
        sim->_waiting.push(_time);
    }
    else sim->addGaveUp();
    sim->_previous_event_time = _time;
}

void GetVaccinated::processEvent() {
    ClinicSimulator* sim = sim->getInstance();
    sim->addWaitTime(_time - sim->_waiting.front()); // the first person in the waiting line is the next to get vaccinated, 
                                        // therefore the first element in _waiting is the time this person started waiting
    double service_time = sim->getServiceTime();
    sim->addServiceTime(service_time);
    sim->addEvent(new Departure(_time + service_time));
}

void Departure::processEvent() {
    ClinicSimulator* sim = sim->getInstance();
    sim->addQueueTime(_time);
    sim->_waiting.pop();
    if (!sim->_waiting.empty()) // there is a next in line
        sim->addEvent(new GetVaccinated(_time));
    sim->_previous_event_time = _time;
}