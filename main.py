#!python3
from sys import argv
import simulator

time_limit = float(argv[1])
num_of_lines = float(argv[2])
lambd = float(argv[3])
mu = float(argv[4])
stay_probabilities = []
for probability in argv[5:]:
    stay_probabilities.append(float(probability))
simulator.init(time_limit, num_of_lines, lambd, mu, stay_probabilities)
simulator.run()
simulator.print_results()
