# simulating-omnivory
Code written in C++ for simulating the evolution of omnivorous food web. Written for a bachelor project.

The code allows evolution of omnivorous food webs with successive additions of invasive species.

species.h defines the classes 'species', 'producer' and 'foodweb'. 
food_web.h defines the functions for adding and removing species, in addition to the interspecies dynamics.
linear_stabiliy.h defines the functions for computing feasiblity and linear stability, as well as thefunctions for writing the data in files.
time_series.h defines the functions for performing the numerical integration of the food web.
distributions.h defines the distributions and ratios used in all other files.

The parameter distributions, ratio of producers to species and probability of consuming on two resources can bemodified in ditributions.h.

Before running the simulation the number of invasions and size of food web must be determined:
j_max in main.cpp determines the total number of additions.
N in species.h determines the total number of species allowed in the food web.


