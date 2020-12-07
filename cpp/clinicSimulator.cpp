#include "clinicSimulator.h"
#include <iostream>
#include <random>
#include <math.h>
#include <time.h>

ClinicSimulator* ClinicSimulator::instance = nullptr;

ClinicSimulator::ClinicSimulator(double time_limit, int num_of_lines, double lambda, double mu, std::vector<double> stay_probabilities) :
    _time_limit(time_limit) , _num_of_lines(num_of_lines), _lambda(lambda), _mu(mu), _stay_probabilities(stay_probabilities), 
    _waiting(), _num_of_vaccinated(0), _num_of_gave_up(0), _previous_event_time(0), _queue_length_time(stay_probabilities.size(), 0),
    // the length of line line can be between 0 and len(stay_probabilities) - 1 because the last probability is always 0
    _total_wait_time(0), _total_service_time(0){
    srand((unsigned)time(NULL));
    double time = 0;
    std::vector<Event*> arrivals;
    while (time <= time_limit) { // set all the arrival times, they are independent of everything else
        time += -( log( ((double)rand() + 1) / (double)RAND_MAX ) / _lambda); // using given formula for exponential distribution
        arrivals.push_back(new Arrival(time));
    }
    _events = std::priority_queue<Event*, std::vector<Event*, std::allocator<Event*> >, eventComparator>(eventComparator(), arrivals);
}

void ClinicSimulator::addVaccinated() { _num_of_vaccinated++; }
void ClinicSimulator::addGaveUp() { _num_of_gave_up++; }
void ClinicSimulator::addWaitTime(double time) { _total_wait_time += time; }
void ClinicSimulator::addServiceTime(double time) { _total_service_time += time; }
void ClinicSimulator::addEvent(Event* event) { _events.push(event); }
void ClinicSimulator::addQueueTime(double time) { _queue_length_time[_waiting.size()] += time - _previous_event_time; }
double ClinicSimulator::getServiceTime() const{ return -( log( ((double)rand() + 1) / (double)RAND_MAX ) / _mu ); }
bool ClinicSimulator::stays() const { return (rand() / (double)RAND_MAX) < _stay_probabilities[_waiting.size()]; }

void ClinicSimulator::run() {
    while (!_events.empty()) {
        Event* nextEvent = _events.top();
        _events.pop();
        nextEvent->processEvent();
        delete nextEvent;
    }
}

void ClinicSimulator::printResults() const{
    std::cout << _num_of_vaccinated << " " // Y - number of people who got vaccinated
              << _num_of_gave_up << " " // X - number of people who came and gave up
              << _previous_event_time << " " // T' - the finish time of all events
              << _queue_length_time[0] << " "; // T0 - the total time the line was empty
    for (int i = 1; i < _queue_length_time.size() ; i++){
        std::cout << _queue_length_time[i] / _num_of_lines << " "; // Ti - the average time any line had i people in it
    }
    std::cout << _queue_length_time[0] / _previous_event_time << " "; // Z0 - the probability of the line being empty
    for (int i = 1; i < _queue_length_time.size(); i++) {
        std::cout << (_queue_length_time[i] / _previous_event_time) / _num_of_lines << " "; // Zi - the probability a specific line has i people in it
    }
    std::cout << _total_wait_time / _num_of_vaccinated << " " // Tw - average waiting time 
              << _total_service_time / _num_of_vaccinated << " " // Ts - average service time
              << _num_of_vaccinated / _time_limit << std::endl; // lambda_A - average arrival time
}