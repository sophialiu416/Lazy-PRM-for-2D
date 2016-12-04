/*
 * main_07_10.cpp
 *
 *  Created on: Oct 7, 2016
 *      Author: sophialiu
 */

#include "halton.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <list>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <stdio.h>
#include <ctype.h>
#include <string>

#include "main_07_10.hpp"
using namespace cv;
using namespace std;





string IntToString (int a)
{
	ostringstream temp;
	temp<<a;
	return temp.str();
}

float Distance(int x1,int y1,int x2,int y2)
{
	float dist;
	dist=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
	return dist;
}

std::vector<_NodeListElement> make_sample_regular(std::vector<_NodeListElement> NodeList, std::vector<_NodeDistanceElement>& NodeDistanceList, int sx, int sy, int ex, int ey)
{
	int steps_x  = 30;
	int steps_y = 15;

	_NodeListElement StartPoint;
    StartPoint.x = sx;
	StartPoint.y = sy;
	StartPoint.num = 0;
	NodeList.push_back(StartPoint);

	_NodeDistanceElement startDistance;
	startDistance.hasBeenSet = true;
	startDistance.shortestPath = 0;
	NodeDistanceList.push_back(startDistance);
	_NodeDistanceElement endDistance;
	endDistance.hasBeenSet = false;
	endDistance.shortestPath = 0;
	NodeDistanceList.push_back(endDistance);

	_NodeListElement GoalPoint;
	GoalPoint.x = ex;
	GoalPoint.y = ey;
	GoalPoint.num = 1;
	NodeList.push_back(GoalPoint);

	int x_max = 1200;
	int y_max = 850;

	int dx = x_max/steps_x;
	int dy = y_max/steps_y;
	srand(4);
	for (int i = 0; i < steps_x; i++)
	{
		for (int j = 0; j < steps_y; j++)
		{
			_NodeListElement NewNode;
			NewNode.num = i*steps_y+j+2;
			double rx = (rand() % 10 + 1);
			double ry = (rand() % 10 + 1);
			rx = rx/10;
			ry = ry/10;
			NewNode.x = i*(dx-1)+dx*rx;
			NewNode.y = j*(dy-1)+dy*ry;
			NodeList.push_back(NewNode);


			// this pushes a new node corresponding to the node NUMBER into the index of shortest paths to a node
			_NodeDistanceElement newDistance;
			newDistance.hasBeenSet = false;
			newDistance.shortestPath = 0;
			NodeDistanceList.push_back(newDistance);
		}
	}

	return NodeList;
}

void expand_Node(int h,std::vector<_NodeListElement>* NodeList)
{
	cout << "eN called with initial size of " << NodeList->size() << endl;
	cout<<"the node is "<<(*NodeList)[h].num<<", now the neighbors num are ";
	int disx = 0;
	int disy = 0;
	int last_num=(*NodeList)[NodeList->size()-1].num;
	(*NodeList)[h].nei_num=(*NodeList)[h].nei_num+3;
	cout<<(*NodeList)[h].nei_num<<endl;
	for(int n=0;n<3;n++)
	{
		disx = rand()%160 +(*NodeList)[h].x-80;
		disy = rand()%160 +(*NodeList)[h].y-80;
		_NodeListElement NewNode;
		last_num=last_num+1;
		NewNode.num = last_num;
		NewNode.x = disx;
		NewNode.y = disy;
		NodeList->push_back(NewNode);
				cout<<"his new neib is "<<NewNode.num<<", x axis is "<<NewNode.x<<", y axis is "<<NewNode.y<<endl;
	}
	cout<<"eN ends with size of "<<NodeList->size()<<endl;
}


void print_path(int index, std::vector<_PathListElement> &PathList,std::vector<_EdgeListElement> &EdgeList,std::vector<_NodeListElement> &NodeList)
{
	cout << endl;
	for (unsigned int n = 0; n < PathList[index].formernodes.size(); n++)
	{
		printf("%3i:",PathList[index].formernodes[n]);
	}
	for(unsigned int n = 120; n > (PathList[index].formernodes.size())*4; n--) {
		cout << " ";
	}
	//printf("dse %10f dng %10f dt %10f \n", PathList[index].distance_start_to_next, PathList[index].distance_next_to_goal, PathList[index].distance_total_path);
}

int indexOfNumInNodeList(std::vector<_NodeListElement>* NodeList, int num) {

	for(unsigned int m=0; m<NodeList->size(); m++) {
		if(num == (*NodeList)[m].num) {
			return m;
		}
	}
	return -1;
}

void update_edge(std::vector<_NodeListElement>* NodeList,std::vector<_EdgeListElement>* EdgeList,int node)
{
	for ( uint k = 0; k < EdgeList->size(); )
	{
		if ((*EdgeList)[k].node_start==node)
		{
			(*NodeList)[indexOfNumInNodeList(NodeList, (*EdgeList)[k].node_end)].nei_num--;
			EdgeList->erase(EdgeList->begin()+k);
		}
		if ((*EdgeList)[k].node_end==node)
		{
			(*NodeList)[indexOfNumInNodeList(NodeList, (*EdgeList)[k].node_start)].nei_num--;
			EdgeList->erase(EdgeList->begin()+k);
		}
		else
		{
			k++;
		}

	}
}

void remove_orphaned_edges(std::vector<_NodeListElement>* NodeList,std::vector<_EdgeListElement>* EdgeList) {
	for (unsigned int i = 0; i < EdgeList->size(); i++)
	{
		if (indexOfNumInNodeList(NodeList, (*EdgeList)[i].node_start) == -1
				|| indexOfNumInNodeList(NodeList, (*EdgeList)[i].node_end) == -1)
		{
			cout << "removed and edge from " << (*EdgeList)[i].node_start << " to " << (*EdgeList)[i].node_end << endl;
		}
	}
}
void find_edge(std::vector<_EdgeListElement>* EdgeList,std::vector<_NodeListElement>* NodeList)
{
	ofstream myfile;
	myfile.open ("EdgeList.txt", ios::out | ios::app );
	float d;
	float hd;
	int nei_num=0;
	EdgeList->clear();
	for ( uint k = 0; k < NodeList->size(); k++ )
	{
		nei_num=0;
		for ( uint h = 0; h< NodeList->size(); h++)
		{
			d = Distance((*NodeList)[k].x,(*NodeList)[k].y,(*NodeList)[h].x,(*NodeList)[h].y);
			hd = Distance((*NodeList)[indexOfNumInNodeList(NodeList, 1)].x,(*NodeList)[indexOfNumInNodeList(NodeList, 1)].y,(*NodeList)[h].x,(*NodeList)[h].y);
			if (d > 0)
			{
				if(d<=80)
				{
					_EdgeListElement NewEdge;
		            NewEdge.node_start=(*NodeList)[k].num;
		        	NewEdge.node_end=(*NodeList)[h].num;
		            NewEdge.distance_s_to_e=d;
		        	NewEdge.distance_e_to_goal=hd; // TODO: böse
		        	EdgeList->push_back(NewEdge);
		        	myfile<<NewEdge.node_start<<", "<<NewEdge.node_end<<", "<<NewEdge.distance_s_to_e<<", "<<NewEdge.distance_e_to_goal<<endl;
		        	nei_num=nei_num+1;
				}
			}
		}
		(*NodeList)[k].nei_num=nei_num;
	}
	myfile.close();
}

void show_nodes(std::vector<_NodeListElement> &NodeList) {

}

void expand_edge(unsigned int point,std::vector<_EdgeListElement>* EdgeList,std::vector<_NodeListElement>* NodeList)
{
	ofstream myfile;
	myfile.open ("EdgeList.txt", ios::out | ios::app );
	float d;
	float hd,hd2;
	int nei_num=0;
//	int Distance_list[302];
	myfile<<"before expand, the size of EdgeList is "<<EdgeList->size()<<endl;
	for ( uint k = point+1; k < NodeList->size(); k++ )
	{
		nei_num=0;
		for (unsigned int h = 0; h< point+1; h++)
		{
			if (h != k) // if comparing two different nodes...
			{
				d = Distance((*NodeList)[k].x,(*NodeList)[k].y,(*NodeList)[h].x,(*NodeList)[h].y);
				hd = Distance(1100,750,(*NodeList)[h].x,(*NodeList)[h].y);
				if(d<=80) // the distance between two nodes is less than 80
				{
					_EdgeListElement NewEdge;
		            NewEdge.node_start=(*NodeList)[k].num;
		        	NewEdge.node_end=(*NodeList)[h].num;
		            NewEdge.distance_s_to_e=d;
		        	NewEdge.distance_e_to_goal=hd;
		        	(*EdgeList).push_back(NewEdge);
		        	myfile<<NewEdge.node_start<<", "<<NewEdge.node_end<<", "<<NewEdge.distance_s_to_e<<", "<<NewEdge.distance_e_to_goal<<endl;
		        	nei_num++;
					_EdgeListElement NewEdge2;
		            NewEdge2.node_start=(*NodeList)[h].num;
		        	NewEdge2.node_end=(*NodeList)[k].num;
		            NewEdge2.distance_s_to_e=d;
		            hd2=Distance(1100,750,(*NodeList)[k].x,(*NodeList)[k].y);
		        	NewEdge2.distance_e_to_goal=hd2;
		        	(*EdgeList).push_back(NewEdge2);
		        	(*NodeList)[h].nei_num++;
		        	myfile<<"in compare, "<<NewEdge2.node_start<<", "<<NewEdge2.node_end<<", "<<NewEdge2.distance_s_to_e<<", "<<NewEdge2.distance_e_to_goal<<endl;
				}
			}
		}
		(*NodeList)[k].nei_num=nei_num;
	}
	myfile.close();
}


int edgeIsBlocked(std::vector<_NodeListElement>* NodeList, _EdgeListElement edge, Mat binary_image){

	// given an edgeListElement it this function returns 1 of there is a black (blocked) pixel in the corresponding image
	// (which represents a blocked path in a star)

	int steps = 60; // that should be enough together with a step size of 80

	_NodeListElement start = (*NodeList)[indexOfNumInNodeList(NodeList, edge.node_start)];
	_NodeListElement end = (*NodeList)[indexOfNumInNodeList(NodeList, edge.node_end)];
	int x_max, x_min, y_max, y_min;
	if(start.x > end.x){
		x_max = start.x;
		x_min = end.x;
	} else {
		x_max = end.x;
		x_min = start.x;
	}
	if(start.y > end.y) {
		y_max = start.y;
		y_min = end.y;
	} else {
		y_max = end.y;
		y_min = start.y;
	}

	double dx = (x_max-x_min)/steps;
	double dy = (y_max-y_min)/steps;

	for(int cnt = 0; cnt < steps; cnt++) {
		if(binary_image.at<uchar>(Point(   x_min+cnt*dx,	y_min+cnt*dy)) < 128)
			return 1;
	}
	return 0;
}

int edgeIsBlocked(std::vector<_NodeListElement>* NodeList, int node1, int node2, Mat binary_image){

	// given an edgeListElement it this function returns 1 of there is a black (blocked) pixel in the corresponding image
	// (which represents a blocked path in a star)

	int steps = 40; // that should be enough together with a step size of 80

	_NodeListElement start = (*NodeList)[indexOfNumInNodeList(NodeList, node1)];
	_NodeListElement end = (*NodeList)[indexOfNumInNodeList(NodeList, node2)];
	int x_max, x_min, y_max, y_min;
	if(start.x > end.x){
		x_max = start.x;
		x_min = end.x;
	} else {
		x_max = end.x;
		x_min = start.x;
	}
	if(start.y > end.y) {
		y_max = start.y;
		y_min = end.y;
	} else {
		y_max = end.y;
		y_min = start.y;
	}

	int dx = (x_max-x_min)/steps;
	int dy = (y_max-y_min)/steps;

	for(int cnt = 0; cnt < steps; cnt++) {
		if(binary_image.at<uchar>(Point(   x_min+cnt*dx,	y_min+cnt*dy)) == 0)
			return 1;
	}
	return 0;
}


void initial_Path(std::vector<int>* firstformernodes, std::vector<_PathListElement>* PathList, std::vector<_EdgeListElement>* EdgeList, std::vector<_NodeListElement>* NodeList){
	
	for (unsigned int i = 0; i < EdgeList->size(); i++)
	{
		if ((*EdgeList)[i].node_start == 0) // starting node is special start node
		{
			_PathListElement NewPath;
			NewPath.node_now=0;
			NewPath.node_next=(*EdgeList)[i].node_end;
			NewPath.distance_next_to_goal=(*EdgeList)[i].distance_e_to_goal;
			NewPath.distance_start_to_next=(*EdgeList)[i].distance_s_to_e;
			NewPath.distance_total_path=NewPath.distance_next_to_goal+NewPath.distance_start_to_next;
			NewPath.formernodes = *firstformernodes;
			NewPath.formernodes.push_back((*EdgeList)[i].node_end); // push back number of end node
			PathList->push_back(NewPath);
		}
	}
	return;
}

void cal_Path(int current_node, int shortest_path_index, std::vector<int>* formerNodes, std::vector<_PathListElement>* PathList, std::vector<_EdgeListElement>* EdgeList, std::vector<_NodeListElement>* NodeList, std::vector<_NodeDistanceElement>* NodeDistanceList, Mat image){
	// this function calculates more paths extending the current node (which is the shortest path so far)

	if(999999 == (*PathList)[shortest_path_index].distance_total_path) {

		// pathifinding is not possible, so just fail and exit with 1

		exit(1);


		for (unsigned int i = 0; i < PathList->size(); i++)	{
			print_path(i,*PathList, *EdgeList, *NodeList);
			display_Path(i, i, image, *PathList, *EdgeList, *NodeList);

		}
		imshow( "Display window", image );
		waitKey(50000);
		exit(0);
	}

	// idea: find all edges starting at this point and then check if they are already
	int last_node = (*PathList)[shortest_path_index].node_next;

	vector<int> newNodesForThisLastNode;
	newNodesForThisLastNode.clear();
	unsigned int eSize = EdgeList->size();


	for (unsigned int m = 0; m < eSize; m++)
	{
		if((*EdgeList)[m].node_start > last_node)
		{
			break; // edgelist is sorted so search is over here
		}
		if ((*EdgeList)[m].node_start == last_node)
		{
			// here is an edge that starts at the last node of the current path, but it might already be in the path 
			int isNotInFormerNodes = 1;

			for (unsigned int n = 0; n < newNodesForThisLastNode.size(); n++)
			{
				if (newNodesForThisLastNode[n] == (*EdgeList)[m].node_end)
				{
					isNotInFormerNodes = 0;
					break;
				}
			}
			for (unsigned int n = 0; n < (*PathList)[shortest_path_index].formernodes.size(); n++)
			{
				if ((*PathList)[shortest_path_index].formernodes[n] == (*EdgeList)[m].node_end)
				{
					isNotInFormerNodes = 0;
					break;
				}
			}

			if (isNotInFormerNodes == 1)
			{
				newNodesForThisLastNode.push_back((*EdgeList)[m].node_end); // this is to avoid duplicate new points
				_PathListElement NewPath;
				NewPath.node_now=(*EdgeList)[m].node_start;
				NewPath.node_next=(*EdgeList)[m].node_end;
				NewPath.distance_next_to_goal=(*EdgeList)[m].distance_e_to_goal;
				NewPath.distance_start_to_next=(*EdgeList)[m].distance_s_to_e+(*PathList)[shortest_path_index].distance_start_to_next;

				if((*NodeDistanceList)[NewPath.node_next].hasBeenSet == false || NewPath.distance_start_to_next < (*NodeDistanceList)[NewPath.node_next].shortestPath) {
					(*NodeDistanceList)[NewPath.node_next].hasBeenSet = true;
					(*NodeDistanceList)[NewPath.node_next].shortestPath = NewPath.distance_start_to_next;
					NewPath.distance_total_path=NewPath.distance_next_to_goal+NewPath.distance_start_to_next;
					*formerNodes= (*PathList)[shortest_path_index].formernodes;
					formerNodes->push_back((*EdgeList)[m].node_end);
					NewPath.formernodes= *formerNodes;
					PathList->push_back(NewPath);
				} else {
					// the new path is longer than an already existing path, so it makes no sense to add it
				}


			}
		}


	}

	(*PathList)[shortest_path_index].distance_total_path = 999999; // use this instead of deleting, as deletion is really slow in vector
	return;

}

int shortest_Path(int &shortest_path_index, int &current_node,std::vector<_PathListElement>* PathList, std::vector<_EdgeListElement>* EdgeList, std::vector<_NodeListElement>* NodeList)
{
	ofstream myfile;
    myfile.open ("example.txt", ios::out | ios::app );
	shortest_path_index = 0;
	double shortest_path_length = (*PathList)[shortest_path_index].distance_total_path;
	unsigned int pl_size = PathList->size();
	for (unsigned int k = 1; k < pl_size; k++)
	{
		if((*PathList)[k].distance_total_path < shortest_path_length)
		{
			shortest_path_index = k;
			shortest_path_length = (*PathList)[k].distance_total_path;
		}
	}
	if((*PathList)[shortest_path_index].node_next == 1)//  end node
	{
		return 1;
	}
	else
	{
		return 0;
	}
	/*
	myfile<< "the shortest path number is "<<shortest_path_index<<endl;
	current_node = (*PathList)[shortest_path_index].node_next;
	for (unsigned int n = 0; n < (*PathList)[shortest_path_index].formernodes.size(); n++)
	{
		myfile << (*PathList)[shortest_path_index].formernodes[n] << " : ";
	}
	myfile << ", distance: "<<(*PathList)[shortest_path_index].distance_total_path << " "<< (*PathList)[shortest_path_index].distance_next_to_goal << endl;
	if((*NodeList)[indexOfNumInNodeList(NodeList, current_node)].nei_num==0)
	{
		myfile<<(*NodeList)[indexOfNumInNodeList(NodeList, current_node)].num<<" have no nei_num"<<endl;
		return 0;

	}

	if((*PathList)[shortest_path_index].node_next == 1)//  end node
	{
		return 1;
	}
	else
	{
		return 0;
	}
	myfile.close();
	return 0; // only for ide
	*/
}

void showDeletedNode(_NodeListElement node, Mat image) {
	cv::circle(image, Point2d(node.x, node.y), 8, Scalar(0,0,0), 10, 8, 0);
}

void Collision_detection(int* collision_true, int shortest_path_index,Mat binary_image,Mat image,std::vector<_PathListElement>* PathList,std::vector<_EdgeListElement>* EdgeList,std::vector<_NodeListElement>* NodeList, char* image_name)
{

	// this function checks the currently shortest path for collisions and removes the cooliding edges and node from nodelist and edgelist


	// remove nodes which are colliding and mark them for deletion of their edges
	std::vector<int> deleted_nodes;
	for(unsigned int m = 0; m < (*PathList)[shortest_path_index].formernodes.size(); m++) {
		// iterate through all nodes and check them for collisions

		// current number of the node to be checked
		int indexOfNum = indexOfNumInNodeList(NodeList, (*PathList)[shortest_path_index].formernodes[m]);

		// is it valid and also white?
		if (indexOfNum != -1 && binary_image.at<uchar>(Point((*NodeList)[indexOfNum].x, (*NodeList)[indexOfNum].y)) > 0) {
			//cout<<(*PathList)[shortest_path_index].formernodes[m]<<" is not in obstacle"<<endl;
		} else {

			//cout<<(*PathList)[shortest_path_index].formernodes[m]<<" is in obstacle"<<endl;
			// make it known that there was a collision
			*collision_true = 1;
			for (unsigned int i=0;i<(*NodeList).size();i++)
			{
				if((*NodeList)[i].num == (*PathList)[shortest_path_index].formernodes[m])
				{
					deleted_nodes.push_back((*PathList)[shortest_path_index].formernodes[m]);
					showDeletedNode(NodeList->at(i), image);
					NodeList->erase(NodeList->begin()+i);
				}
			}
		}
	}

	// done deleting the nodes which were inside a blocked area
	for (unsigned int i = 0; i < (*PathList)[shortest_path_index].formernodes.size() -1; i++)
	{

		if (edgeIsBlocked(NodeList, (*PathList)[shortest_path_index].formernodes[i], (*PathList)[shortest_path_index].formernodes[i+1], binary_image) > 0)
		{
			// edge is blocked so we need to delete this edge and mark the path as broken
			for (unsigned int m = 0; m < EdgeList->size(); m++)
			{
				if (((*EdgeList)[m].node_start == (*PathList)[shortest_path_index].formernodes[i] && (*EdgeList)[m].node_end == (*PathList)[shortest_path_index].formernodes[i+1]) 
					|| ((*EdgeList)[m].node_end == (*PathList)[shortest_path_index].formernodes[i] && (*EdgeList)[m].node_start == (*PathList)[shortest_path_index].formernodes[i+1]))
					
				{
					// found the edge we were looking for to delete it
					EdgeList->erase(EdgeList->begin()+m);

					// make it known that there was a collision
					*collision_true = 1;


				} else {
				}
			}
		}
	}
	std::vector<_EdgeListElement>::iterator it = EdgeList->begin();
	while(it != EdgeList->end()) {
		int oneOfTheEdgesNodesGotDeleted = 0; // assume it's okay until proven not

		for(std::vector<int>::size_type m = 0; m != deleted_nodes.size(); m++) {
			if (it->node_end == deleted_nodes[m] || it->node_start == deleted_nodes[m])
			{
				oneOfTheEdgesNodesGotDeleted = 1;
				break;
			}
		}

		if(oneOfTheEdgesNodesGotDeleted == 1) {
			// make it known that there was a collision
			*collision_true = 1;
			it = EdgeList->erase(it);
		} else {
			++it;
		}
	}

	image = imread(image_name, IMREAD_COLOR);
	drawEdges(*NodeList, *EdgeList, image);
	imshow( "Display window", image );
	drawPoints(*NodeList, image);
}

void display_Path(int round, int shortest_path_index, Mat image, std::vector<_PathListElement> &PathList,std::vector<_EdgeListElement> &EdgeList,std::vector<_NodeListElement> &NodeList)
{
	ofstream myfile;
	cv::putText(image, IntToString(PathList[shortest_path_index].distance_total_path), Point2f(50, 50),
			FONT_HERSHEY_SIMPLEX, (double)0.5, Scalar(0,0,0), 1, true);
    myfile.open ("example.txt", ios::out | ios::app );
	myfile << "Showing path for index " << shortest_path_index << "now" << endl;

	for (unsigned int m = 0; m< PathList[shortest_path_index].formernodes.size()-1; m++) {

		int node_num = PathList[shortest_path_index].formernodes[m];
		int node_num2 = PathList[shortest_path_index].formernodes[m+1];
		cv::circle(image, Point2d(NodeList[indexOfNumInNodeList(&NodeList, node_num)].x, NodeList[indexOfNumInNodeList(&NodeList, node_num)].y), 8, Scalar(50,150,100), 1, 8, 0);
		cv::line(image,Point(NodeList[indexOfNumInNodeList(&NodeList, node_num)].x,NodeList[indexOfNumInNodeList(&NodeList, node_num)].y),Point(NodeList[indexOfNumInNodeList(&NodeList, node_num2)].x,NodeList[indexOfNumInNodeList(&NodeList, node_num2)].y),Scalar(0+40*round,0+40*round,255-40*round),2, LINE_8, 0 );
	}
	myfile.close();
}



int A_star(vector<int> * firstFormerNodes, std::vector<_PathListElement>* PathList,std::vector<_EdgeListElement>* EdgeList,std::vector<_NodeListElement>* NodeList, Mat image, std::vector<_NodeDistanceElement>* NodeDistanceList)
{

	PathList->clear();

	int shortest_path_index = 0;
	int current_node = 0; // this is the node's number, not it's index
	initial_Path(firstFormerNodes,PathList,EdgeList,NodeList);

	for(int times=0;times<600000;times++) // 3000 as safeguard for infinite loops
	{

		int sP = shortest_Path(shortest_path_index,current_node,PathList,EdgeList,NodeList);

		if(sP == 1) // end node found
		{
			return shortest_path_index;
		}
		else if(sP==2) // added more neighbors
		{
			shortest_path_index = -1;
		}
		else
		{
			std::vector<int> formerNodes;
			cal_Path(current_node, shortest_path_index, &formerNodes,PathList,EdgeList, NodeList, NodeDistanceList, image); // calculate the next steps from the original shortest path
		}

	}
	display_Path(1,shortest_path_index,image,*PathList,*EdgeList,*NodeList);
	waitKey(30000);
	return -4;
}




void drawPoints(std::vector<_NodeListElement> NodeList, Mat image) {
	for(unsigned int i = 0; i< NodeList.size(); i++) {
		Point2f p(NodeList[i].x, NodeList[i].y);
		Scalar color = Scalar( 0, 0, 255 );
		cv::circle(image, p,1,color,2,2,0);
		cv::putText(image, IntToString(NodeList[i].num), p, FONT_HERSHEY_SIMPLEX, (double)0.5, color, 1, true);
		imshow("Display window", image);
	}
}

void drawEdges(std::vector<_NodeListElement> NodeList, std::vector<_EdgeListElement> EdgeList, Mat image) {

	for(unsigned int i = 0; i< EdgeList.size(); i++) {
		Point2f p1(NodeList[indexOfNumInNodeList(&NodeList,EdgeList[i].node_start)].x, NodeList[indexOfNumInNodeList(&NodeList,EdgeList[i].node_start)].y);
		Point2f p2(NodeList[indexOfNumInNodeList(&NodeList,EdgeList[i].node_end)] .x, NodeList[indexOfNumInNodeList(&NodeList,EdgeList[i].node_end)].y);
		Scalar color = Scalar( 255, 174, 0 );
		cv::line(image,p1,p2,color,1);
	}
}


int main(int argc, char ** argv)
{

	vector<int> firstFormerNodes;
	firstFormerNodes.push_back(0);
	bool test=false;
	if(argc > 6){
		test=true;
	}

	//--------------------------------Read Image------------------------------------------
	Mat image,gray_image,binary_image; //n-dimensional array, used to store image data
	image = imread(argv[1], IMREAD_COLOR);
	if(! image.data )
	{
		cout << "Could not open or find the image" << endl ;
		return 1;
	}

	//-------------------------------Process Image----------------------------------------
	cvtColor(image, gray_image, CV_RGB2GRAY);
	threshold(gray_image,binary_image,0.5,255,THRESH_BINARY );

	NodeList = make_sample_regular(NodeList, NodeDistanceList, strtol(argv[2], &argv[3], 10),strtol(argv[3], &argv[4], 10),strtol(argv[4], &argv[5], 10),strtol(argv[5], &argv[6], 10));
	ofstream nodeListFile;
	nodeListFile.open("nodes.txt");
	for(unsigned int i = 0; i< NodeList.size(); i++) {
		nodeListFile <<  i << ": " << NodeList[i].x << "," << NodeList[i].y << "," << NodeList[i].num << endl;
	}
	nodeListFile.close();


	namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
	imshow( "Display window", image );
	find_edge(&EdgeList,&NodeList);
	drawEdges(NodeList, EdgeList, image);
	drawPoints(NodeList, image);
	waitKey(50);

	// Show image inside it.
	imshow( "Display window", image );
	int shortest_path_index;
	for(int round=0;round<27;round++)
	{
		image = imread(argv[1], IMREAD_COLOR);
		drawEdges(NodeList, EdgeList, image);
		drawPoints(NodeList, image);
		imshow( "Display window", image );


		shortest_path_index = A_star(&firstFormerNodes, &PathList,&EdgeList,&NodeList, image, &NodeDistanceList);
		display_Path(1,shortest_path_index,image,PathList,EdgeList,NodeList);

		imshow( "Display window", image );
		//print_path(shortest_path_index, PathList, EdgeList, NodeList);
		waitKey(300);
		int collision_true = 0;
		Collision_detection(&collision_true, shortest_path_index,binary_image,image,&PathList,&EdgeList,&NodeList, argv[1]);
		PathList.clear();
		for(unsigned int i=0; i < NodeDistanceList.size()-1; i++) {
			NodeDistanceList[i].hasBeenSet = false;
		}
		drawEdges(NodeList, EdgeList, image);
		if(collision_true == 0)
		{
			//cout<<"finish path"<<endl;

			break;
		}
	}
	unsigned int result;
	if (shortest_path_index>-1)
	{
		result = 0;
	} else {
		result = 1;
	}
	//------------------------show the path--------------------------------
	// Show image inside it.
	imshow( "Display window", image );
	waitKey(500);
//    myfile.close();
	if(!test) {
		cout<<"press any key to exit"<<endl;
		waitKey(0); // Wait for a keystroke in the window
	}

	return result;
}


