/*
 * main2.cpp
 *
 *  Created on: May 10, 2016
 *      Author: sophialiu
 */

#include "halton.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <map>
using namespace cv;
using namespace std;



class Sample{
	public:
	int x;
	int y;
	int num;
};
class Edge: public Sample{
	public:
	int current_point;
	int nei_point[];
	int nei_num;
	float dist;//float dist[];
	float h_score;
//	Edge(Sample &start, Sample &goal)
//	{
//		nodeStart = &start;
//	}
	double Distance(int x1,int y1,int x2,int y2)
	{
		 double dist;
		 dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
		 return dist;
		}
//	Sample *nodeStart;

};

class Astarpath: public Edge{
//	class Astarpath{
	public:
	int node_num;
    int node_been[];
	float g_score;
	float h_score;
	float f_score;

};
int open_list[300][2];
int close_list[300][2];

//-----------------------------read image-------------------------------------------
int main(int argc, char** argv)
{

//	std::vector<Edge> edges;
//	edges.push_back(..);
//
//	for (Edge &e : edges)
//	{
//		e.nodeStart->
//	}
Mat image,gray_image,binary_image; //n-dimensional array, used to store image data
image = imread("/home/sophialiu/Documents/Obstacle1.png", IMREAD_COLOR);
/*if(! image.data )
{
cout << "Could not open or find the image" << endl ;
return -1;
}
cvtColor(image, gray_image, CV_RGB2GRAY);
threshold(gray_image,binary_image,0.5,255,THRESH_BINARY );*/

//--------------------------sample nodes--------------------------------------
    Sample node[301];
    node[0].x = 100;
    node[0].y = 130;
    node[0].num = 0;
    cv::circle(image, Point2d(node[0].x, node[0].y), 8, Scalar(100,150,100), 1, 8, 0);
    int m_dim_num = 2;
    int m_n = 150;
    int m_seed[2], m_leap[2], m_base[2];
    double m_r[300];
    for (int i = 0; i < m_dim_num; i++)
    {
        m_seed[i] = 0;
        m_leap[i] = 1;
        m_base[i] = 2+i;
    }
    i4_to_halton_sequence( m_dim_num, m_n, 0, m_seed, m_leap, m_base, m_r);
    int displaced1 = 1200;
        int displaced2 = 850;
        for (int i = 0; i < 300; i=i+1)
        {
        	node[i].num = i+1;
        	open_list[i][1] = node[i+1].x = (m_r[i])*displaced1;
        	open_list[i][2] = node[i+1].y = (m_r[i+1])*displaced2;
            cv::circle(image, Point2d((m_r[i])*displaced1, (m_r[i+1])*displaced2), 8, Scalar(255,0,0), 1, 8, 0);
        }
        node[301].x = 1000;
        node[301].y = 850;
        node[301].num = 301;
        cv::circle(image, Point2d(node[301].x, node[301].y), 8, Scalar(100,150,100), 1, 8, 0);

//------------------------find neighbor nodes--------------------------------
Edge node_nei[301];
float distance;
float dist_to_end;
int n=1;
for ( int k = 0; k < 300; k++ )
{
	node_nei[k].x = node[k].x;
	node_nei[k].y = node[k].y;
	node_nei[k].nei_num = 0;
	for ( int h = 0; h < 300; h++ )
	{
		distance = node_nei[k].Distance(node_nei[k].x,node_nei[k].y,node[h].x,node[h].y);
		if(distance > 0)
			if(distance < 80)
		{ //int s=node_nei[k].x;
		    //cout<<"node"<<s;
			node_nei[k].current_point = k;
			cout<<"node "<<node_nei[k].current_point;
			node_nei[k].nei_point[n] = node[h].num;
			cout<<" to node "<<node_nei[k].nei_point[n];
			node_nei[k].dist[n] = distance;
			cout<<" ,distance "<<node_nei[k].dist[n];
			node_nei[k].h_score[n]=	node_nei[k].Distance(node[301].x,node[301].y,node[h].x,node[h].y);
			cout<<" ,the rest distance "<<node_nei[k].h_score[n];
			cv::line(image,Point(node_nei[k].x,node_nei[k].y),Point(node[h].x,node[h].y),Scalar(0,0,255),2, LINE_8, 0 );
			cout<<"                      the distance is "<<node_nei[k].dist[n]<<",the rest distance is "<<node_nei[k].h_score[n];
			node_nei[k].nei_num ++;
			n = node_nei[k].nei_num;
		}
	}

}


Astarpath node_path[100];

for ( int h = 0; h < node_nei[1].nei_num ; h++ )
{
node_path[h].node_num = node_nei[1].nei_point[h];
node_path[h].g_score = node_nei[1].dist[h];
node_path[h].f_score = node_nei[1].h_score[h]+node_path[h].g_score;
node_path[h].node_been[0] = node_nei[1].current_point;
cout<<"node_number "<<node_nei[1].nei_num<<" Point "<<node_path[h].node_num<<",g_score "<<node_path[h].g_score<<",f_score "<<node_path[h].f_score<<endl;
cout<<"              Point "<<node_nei[1].nei_point[h]<<",g_score "<<node_nei[1].dist[h]<<",f_score "<<node_nei[1].h_score[h];
}

	/*float min = node_path[0].f_score
	int	min_num=0;
    for(int i= 1;i< n ;i++)
    {
        if(min > node_path[i].f_score)
            {
        	min = node_path[i].f_score;
        	min_num = i;
            }
    }*/



//------------------------show the path--------------------------------
namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
imshow( "Display window", image );
// Show image inside it.
cout<<"press any key to exit"<<endl;
waitKey(0);
// Wait for a keystroke in the window
return 0;
}



/*
//------------------------A star algorithm--------------------------------
//open_list[] = Astarpath::Sample::node[];
Astarpath node_path[100];
current_node.x = start_node.x;
current_node.y = start_node.y;
close_list[1][1]=current_node.x;
close_list[1][2]=current_node.y;
//	cout<<close_list[1][1]<<close_list[1][2];
int search_value_x = current_node.x;
int search_value_y = current_node.y;
int num = 0;
node_path[0].g_score = 0.0;
for ( int k = 1; k < 150; k++ )
{

	if (open_list[k][1]=search_value_x)
	{
		if (open_list[k][1]=search_value_y)
		{
			cout<<"Number "<<k<<endl;
			double f_score [k];
            num = num +1;
			for (int a = 1; a < node_nei[k].nei_num; a++)
			{
				node_path[num].current_point=current_node.x;
				node_path[num].current_point=current_node.y;
				//node_path[num].g_score=node_path[num-1].g_score+node_nei[k].dist[a];
				f_score[a]=node_nei[k].h_score[a]+node_path[num-1].g_score+node_nei[k].dist[a];
			}
			int smallest_f = 1;
			for (int b = 2; b < node_nei[k].nei_num; b++)
							{
							if (f_score[b]<f_score[b-1])
								smallest_f = b;

							}
			node_path[num].g_score=node_path[num-1].g_score+node_nei[k].dist[smallest_f];
			cout<<"The smallest value is"<<smallest_f<<endl;
			open_list[k][1]=0;
			open_list[k][2]=0;
			close_list[num][1]=current_node.x;
			close_list[num][2]=current_node.y;
			search_value_x= node_nei[k].x;
			search_value_y= node_nei[k].y;
		}
	}
//	result = find(node_nei[1].current_point[1][1] , node_nei[149].current_point[1][1] , search_value);
//	cout<<"The value "<<search_value<<(result == node_nei[149].current_point[1][1] ? " is not present" : "is present")<<endl;
}
*/











