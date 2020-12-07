#pragma once
#include <queue>
#include <vector>

class Event { // abstract class of a generic event
public:
    const double _time;
    Event(double time) : _time(time) {}
    virtual ~Event() = default;
    virtual void processEvent() = 0;
};

class Arrival : public Event { // the event of someone arriving to get vaccinated
public:
    Arrival(double time) : Event(time) {}
    ~Arrival() = default;
    void processEvent();
};

class GetVaccinated : public Event {
public:
    GetVaccinated(double time) : Event(time) {}
    ~GetVaccinated() = default;
    void processEvent();
};

class Departure : public Event {
public:
    Departure(double time) : Event(time) {}
    ~Departure() = default;
    void processEvent();
};

struct eventComparator {
    bool operator() (const Event* left, const Event* right) const {
        return left->_time > right->_time;
    }
};

class ClinicSimulator {
    const double _time_limit; // time limit given for arrivals
    const int _num_of_lines; // number of lines in the clinic
    double _mu; // service pace
    const std::vector<double> _stay_probabilities; // at index i is the probability a person will stay if lhe line has i people in it when he arrives
    std::priority_queue<Event*, std::vector<Event*, std::allocator<Event*> >, eventComparator> _events; // list of events, used as a priority queue sorted bt time
    int _num_of_vaccinated; // number of people who got vaccinated
    int _num_of_gave_up; // number of people who came to get vaccinated but gave up because the line was too ling
    std::vector<double> _queue_length_time; // at index i holds the amount of time the line had i people in it(including the man being vaccinated)
    double _total_wait_time; // total time people spent waiting in line
    double _total_service_time; // total time people spent getting vaccinated
    static ClinicSimulator* instance;

    ClinicSimulator(double time_limit, int num_of_lines, double lambd, double mu, std::vector<double> stay_probabilities);

public:
    double _previous_event_time; // holds the time the previous event happened
    std::queue<double> _waiting; // list of people waiting in the line, represented by there time of arrival
    ClinicSimulator(ClinicSimulator&) = delete;
    ClinicSimulator& operator=(ClinicSimulator&) = delete;
    ClinicSimulator(const ClinicSimulator&) = delete;
    ClinicSimulator& operator=(const ClinicSimulator&) = delete;
    static ClinicSimulator* getInstance(double time_limit = 0, int num_of_lines = 0, double lambda = 0, double mu = 0,
        std::vector<double> stay_probabilities = std::vector<double>()) {
        if (!instance)
            instance = new ClinicSimulator(time_limit, num_of_lines, lambda, mu, stay_probabilities);
        return instance;
    }
    void addVaccinated(); // adds 1 to the number of people who got vaccinated
    void addGaveUp(); // adds 1 to the number of people who came to the clinic and gave up
    void addWaitTime(double time); // adds given time to total time people spent waiting in the clinic
    void addServiceTime(double time); // adds given time to total time spent servicing people
    void addEvent(Event* event); // adds a new event to the priority queue of events
    void addQueueTime(double time); // adds the time the queue was at its current length to the total time the queue was of this length
    double getServiceTime() const; // returns a randomly generated service time exponentialy distributed 
    bool stays() const; // checks if a person who will arrive now will stay
    void run();
    void printResults() const;
};
