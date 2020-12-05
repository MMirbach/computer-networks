import event
import heapq
from random import expovariate

time_limit = 0
num_types_of_vaccines = 0

def init(stop_at, num_of_lines, lambd, mu, stay_probabilities):
    global time_limit, num_types_of_vaccines
    event.lambd = lambd
    event.mu = mu
    time_limit = stop_at
    num_types_of_vaccines = num_of_lines
    event.stay_probabilities = stay_probabilities
    event.queue_length_time = [0] * (len(stay_probabilities)) # the length of line line can be between 0
    # and len(stay_probabilities)-1 because the last probability is always 0
    time = 0
    while time <= time_limit: # set all the arrival times, they are independent of everything else
        time = time + expovariate(event.lambd)
        heapq.heappush(event.events, event.Arrival(time))

def run():
    while event.events:
        next_event = heapq.heappop(event.events)
        next_event.process_event()

def print_results():
    global time_limit, num_types_of_vaccines
    print(event.num_of_vaccinated), # Y - number of people who got vaccinated
    print(event.num_of_gave_up), # X - number of people who came and gave up
    print(event.previous_event_time), # T' - the finish time of all events
    print(event.queue_length_time[0]), # T0 - the total time the line was empty
    for Ti in event.queue_length_time[1:]:
        print(Ti/num_types_of_vaccines), # Ti - the average time any line had i people in it
    print(event.queue_length_time[0] / event.previous_event_time), # Z0 - the probability of the line being empty
    for Zi in event.queue_length_time[1:]:
        print((Zi / event.previous_event_time) / num_types_of_vaccines), # Zi - the probability a specific line has i people in it
    print(sum(event.wait_times) / len(event.wait_times)), # Tw - average waiting time
    print(sum(event.service_times) / len(event.service_times)), # Ts - average service time
    print(event.num_of_vaccinated / time_limit) # T_lambda - average arrival time
