#pragma once
#include "distributions.h"
#include "linear_stability.h"


// ------------------------------------------------------------------------------------------// 
//							FUNCTIONS FOR MANIPULATING FOODWEB								 //
// ------------------------------------------------------------------------------------------//  



//	INTERACTIONS
	bool add_link(species S[], producer P[], int resource, int consumer);
	// creating link between resource and consumer

	void remove_link(species S[], producer P[], int resource, int consumer);
	// removing link between resource and consumer

	void remove_all_links(species S[], producer P[], int index);
	// removing all links of a species

	void trophic_level(species S[]);
	// computing trophic level of all species



//	ADDING AND REMOVING SPECIES
	int free_index(species S[], producer P[], bool primary);
	// finding lowest free index in species array, keeping producers at lowest indices

	void add_species(species S[], producer P[], int iteration);
	// adding number'th species to the foodweb

	void remove_species(species s[], producer p[], int index);
	// removing species with index from foodweb

	void reinitializing(species S[], species S_copy[], producer P[], producer P_copy[]);
	// reinitilazing species arrays to food web before invasion



//	COMPUTING DERIVATIVES
	double available_nutrients(producer P[]);
	// computing available nutrients from the nutreint source
	// assuming all producers consume at the same rate

	double strengthen(species S[], int index);
	// computing the amount species index is strengthened when consuming on its resources

	double weaken(species S[], int index);
	// computing the amount species index is weakened when being consumed by its consumers

	void derivatives(species S[], producer P[]);
	// computing derivatives of all species in foodweb
