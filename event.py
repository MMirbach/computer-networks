from abc import ABC, abstractmethod
import heapq
from random import expovariate, uniform


lambd = 0
mu = 0
stay_probabilities = []
events = []
waiting = []
num_of_vaccinated = 0
num_of_gave_up = 0
previous_event_time = 0
previous_arrival_time = 0
queue_length_time = []
wait_times = []
service_times = []

class Event(ABC):
    time: int
    def __init__(self, t):
        self.time = t
    def __lt__(self, other):
        return self.time < other.time
    @abstractmethod
    def process_event(self):
        pass

class Arrival(Event):
    def __init__(self, time):
        super().__init__(time)

    def process_event(self):
        global waiting, mu, num_of_vaccinated, num_of_gave_up, queue_length_time, previous_event_time
        queue_length_time[len(waiting)] += self.time - previous_event_time
        if uniform(0,1) < stay_probabilities[len(waiting)]:
            num_of_vaccinated += 1
            if not waiting: # Can be served
                heapq.heappush(events, GetVaccinated(self.time))
            waiting.append(self.time)
        else:
            num_of_gave_up += 1
        previous_event_time = self.time

class GetVaccinated(Event):
    def __init__(self, time):
        super().__init__(time)

    def process_event(self):
        global waiting, mu
        arrival_time = waiting[0]
        wait_time = self.time - arrival_time
        wait_times.append(wait_time)
        service_time = expovariate(mu)
        service_times.append(service_time)
        finish_time = self.time + service_time
        heapq.heappush(events, Departure(finish_time))

class Departure(Event):
    def __init__(self, time):
        super().__init__(time)

    def process_event(self):
        global events, queue_length_time, previous_event_time
        queue_length_time[len(waiting)] += self.time - previous_event_time
        waiting.pop(0)
        if waiting:
            heapq.heappush(events, GetVaccinated(self.time))
        previous_event_time = self.time
