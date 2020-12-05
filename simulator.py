import event
import heapq
from random import expovariate
from statistics import mean

T = 0

def init(time_limit, lambd, mu, stay_probabilities):
    global T
    event.lambd = lambd
    event.mu = mu
    T = time_limit
    for probability in stay_probabilities:
        event.stay_probabilities.append(probability)
    time = 0
    while time <= time_limit:
        time = time + expovariate(event.lambd)
        heapq.heappush(event.events, event.Arrival(time))
    event.queue_length_time = [0] * (len(stay_probabilities))

def run():
    while event.events:
        next_event = heapq.heappop(event.events)
        next_event.process_event()

def print_results():
    global T
    print(event.num_of_vaccinated, event.num_of_gave_up, event.previous_event_time, end=' ')
    for Ti in event.queue_length_time:
        print(Ti, end=' ')
    for Ti in event.queue_length_time:
        print(Ti / event.previous_event_time, end=' ')
    print(mean(event.wait_times), mean(event.service_times), event.num_of_vaccinated/T)
