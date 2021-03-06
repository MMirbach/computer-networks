#!python3
from sys import argv
import simulator

stay_probabilities = [float(probability) for probability in argv[5:]]
simulator.init(float(argv[1]), float(argv[2]), float(argv[3]), float(argv[4]), stay_probabilities)
simulator.run()
simulator.print_results()
