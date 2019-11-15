#pragma once
#include "distributions.h"
#include "linear_stability.h"


// ------------------------------------------------------------------------------------------// 
//							FUNCTIONS FOR MANIPULATING FOODWEB								 //
// ------------------------------------------------------------------------------------------//  



//	INTERACTIONS
	bool addLink(Species S[], Producer P[], int resource, int consumer);
	// creating link between resource and consumer

	void removeLink(Species S[], Producer P[], int resource, int consumer);
	// removing link between resource and consumer

	void removeAllLinks(Species S[], Producer P[], int index);
	// removing all links of a Species

	void updateTrophicLevel(Species S[]);
	// computing trophic level of all Species



//	ADDING AND REMOVING Species
	int freeIndex(Species S[], Producer P[], bool isProducer);
	// finding lowest free index in Species array, keeping Producers at lowest indices

	void addSpecies(Species S[], Producer P[], int addAttempt);
	// adding number'th Species to the foodweb

	void removeSpecies(Species s[], Producer p[], int index);
	// removing Species with index from foodweb

	// void reinitializing(Species S[], Species S_copy[], Producer P[], Producer P_copy[]);
	// reinitilazing Species arrays to food web before invasion



//	COMPUTING DERIVATIVES
	double availableNutrients(Producer P[]);
	// computing available nutrients from the nutreint source
	// assuming all Producers consume at the same rate

	double strengthen(Species S[], int index);
	// computing the amount Species[index] is strengthened when consuming its resources

	double weaken(Species S[], int index);
	// computing the amount Species[index] is weakened when being consumed by its consumers

	void derivatives(Species S[], Producer P[]);
	// computing derivatives of all Species in foodweb
