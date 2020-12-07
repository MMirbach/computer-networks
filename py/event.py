from heapq import heappush
from random import expovariate, uniform
from collections import deque


lambd = 0 # arrival time parameter
mu = 0 # vaccination time parameter
stay_probabilities = [] # at index i is the probability a person will stay if lhe line has i people in it when he arrives
events = [] # list of events, used as a heap sorted bt time
waiting = deque() # list of people waiting in the line, represented by there time of arrival
num_of_vaccinated = 0 # number of people who got vaccinated
num_of_gave_up = 0 # number of people who came to get vaccinated but gave up because the line was too ling
previous_event_time = 0 # holds the time the previous event happened
queue_length_time = [] # at index i holds the amount of time the line had i people in it (including the man being vaccinated)
total_wait_time = 0 # total time people spent waiting in line 
total_service_time = 0 # total time people spent getting vaccinated

class Event: # generic event class
    def __init__(self, t):
        self.time = t
    def __lt__(self, other):
        return self.time < other.time
    def process_event(self):
        pass

class Arrival(Event): # the event of someone arriving to get vaccinated
    def __init__(self, time):
        Event.__init__(self, time)

    def process_event(self):
        global waiting, mu, num_of_vaccinated, num_of_gave_up, queue_length_time, previous_event_time
        # the queue had len(waiting) people in it for this amount of time before this person arrived
        queue_length_time[len(waiting)] += self.time - previous_event_time
        if uniform(0,1) < stay_probabilities[len(waiting)]: # check if the man gave up
            num_of_vaccinated += 1
            if not waiting: # no-one is waiting, can be vaccinated right away
                heappush(events, GetVaccinated(self.time))
            waiting.append(self.time)
        else:
            num_of_gave_up += 1
        previous_event_time = self.time

class GetVaccinated(Event):
    def __init__(self, time):
        Event.__init__(self, time)

    def process_event(self):
        global waiting, mu, total_wait_time, total_service_time
        total_wait_time += self.time - waiting[0] # time this person spent waiting in line
        service_time = expovariate(mu)
        total_service_time += service_time
        heappush(events, Departure(self.time + service_time))

class Departure(Event):
    def __init__(self, time):
        Event.__init__(self, time)

    def process_event(self):
        global waiting, events, queue_length_time, previous_event_time
        # the queue had len(waiting) people in it for this amount of time before this person left
        queue_length_time[len(waiting)] += self.time - previous_event_time
        waiting.popleft()
        if waiting: # there is a next in line
            heappush(events, GetVaccinated(self.time))
        previous_event_time = self.time
