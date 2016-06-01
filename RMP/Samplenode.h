/*
 * Samplenode.h
 *
 *  Created on: May 9, 2016
 *      Author: sophialiu
 */

#ifndef SAMPLENODE_H_
#define SAMPLENODE_H_

struct Sample_node {
public:
	int Sample_node_create(int sample_x,int sample_y) ;
	//virtual ~Sample_node();
	int number;
	int sample_x;
	int sample_y;

};

#endif /* SAMPLENODE_H_ */
