/*
 * main_07_10.hpp
 *
 *  Created on: Oct 9, 2016
 *      Author: sophialiu
 */

#ifndef MAIN_07_10_HPP_
#define MAIN_07_10_HPP_

#include "opencv2/opencv.hpp"

typedef struct
{
	int x;
	int y;
	int num;
	int nei_num;
} _NodeListElement;

std::vector<_NodeListElement> NodeList;

typedef struct
{
	float shortestPath;
	bool hasBeenSet;
} _NodeDistanceElement;

std::vector<_NodeDistanceElement> NodeDistanceList;

typedef struct
{
	int node_start;
	int node_end;
	double distance_s_to_e;
	double distance_e_to_goal;
} _EdgeListElement;

std::vector<_EdgeListElement> EdgeList;

typedef struct
{
	int node_now;
	int node_next;
	double distance_start_to_next;
	double distance_next_to_goal;
	double distance_total_path;
	std::vector<int> formernodes;
} _PathListElement;

std::vector<_PathListElement> PathList;


void drawEdges(std::vector<_NodeListElement> NodeList, std::vector<_EdgeListElement> EdgeList, cv::Mat image);
void drawPoints(std::vector<_NodeListElement> NodeList, cv::Mat image);
void display_Path(int round, int shortest_path_index, cv::Mat image, std::vector<_PathListElement> &PathList,std::vector<_EdgeListElement> &EdgeList,std::vector<_NodeListElement> &NodeList);

#endif /* MAIN_07_10_HPP_ */
