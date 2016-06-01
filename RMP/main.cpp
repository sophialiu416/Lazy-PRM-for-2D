/*
 * main.cpp
 *
 *  Created on: May 6, 2016
 *      Author: sophialiu
 */

#include "halton.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace cv;
using namespace std;

class Sample
{public:
	int x;
	int y;
//	int t;
//	Sample(int x,int y);
/*	void sample_create(int t)
	{
		std::srand(t); // use current time as seed for random generator
		x = std::rand()% 1200;
		std::srand(t);
		y = std::rand()% 850;
	}*/
};
//Sample::Sample(int x, int y): x(x), y(y){}
class Edge: public Sample{
public:
	int current_point[][2];
	int nei_point[][2];
	int nei_num;
	double dist[];
	double h_score[];
//	Edge(int,int,int,int,double);
	double Distance(int x1,int y1,int x2,int y2)
		{
		 double dist;
		 dist=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
//		 cout<<dist<<endl;
		 return dist;
		}
};

class Astarpath: public Edge{
public:

	double g_score;
	double h_score[];
	double f_score[];

};
int open_list[300][2];
int close_list[300][2];


//Edge::Edge(int x,int y, int x2,int y2, double dist): Sample(x, y){

//}

int main(int argc, char** argv)
{

	vector<vector<double> > edge_all;
	vector <double> edges;

Mat image,gray_image,binary_image; //n-dimensional array, used to store image data
image = imread("/home/sophialiu/Documents/Obstacle1.png", IMREAD_COLOR);
if(! image.data )
// Check for invalid input
{
cout << "Could not open or find the image" << endl ;
return -1;
}
cvtColor(image, gray_image, CV_RGB2GRAY);
threshold(gray_image,binary_image,0.5,255,THRESH_BINARY );
//--------------------------sample nodes1--------------------------------------
/*Sample node[200];
int t = std::time(0);
for ( int i = 1; i < 200; i++ )
{
	t = t + 5742;
	node[i].sample_create(t);
//	cout<<node[i].x<<node[i].y<<endl;
	cv::circle(binary_image,Point(node[i].x,node[i].y), 8, Scalar(100,150,100), 2, LINE_8, 0 );
};*/

//--------------------------sample nodes2--------------------------------------
    Sample node[300];

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
        	open_list[i][1] = node[i].x = (m_r[i])*displaced1;
        	open_list[i][2] = node[i].y = (m_r[i+1])*displaced2;
            cv::circle(binary_image, Point2d((m_r[i])*displaced1, (m_r[i+1])*displaced2), 8, Scalar(100,150,100), 1, 8, 0);
        }

//------------------------find neighbor nodes--------------------------------
Sample start_node;
start_node.x = 10;
start_node.y = 10;
Sample end_node;
end_node.x = 1100;
end_node.y = 800;
Edge node_nei[300];
double d;
double hd;
int n=1;
Sample current_node;
current_node.x = start_node.x;
current_node.y = start_node.y;
for ( int k = 1; k < 150; k++ )
{
	cout<<n<<endl;
	n=1;
	for ( int h = 1; h < 300; h++ )
	{
		d = node_nei[k].Distance(current_node.x,current_node.y,node[h].x,node[h].y);
		hd = node_nei[k].Distance(end_node.x,end_node.y,node[h].x,node[h].y);
		if (d <= 80)
		{
			edges.push_back(h);
			edges.push_back(d);
			edges.push_back(hd);
			node_nei[k].current_point[1][1] = current_node.x;
			node_nei[k].current_point[1][2] = current_node.y;
			node_nei[k].nei_point[n][1] = node[h].x;
			node_nei[k].nei_point[n][2] = node[h].y;
			node_nei[k].dist[n] = d;
			node_nei[k].h_score[n]=hd;
			node_nei[k].nei_num = n;
			n = n+1;
			cv::line(binary_image,Point(current_node.x,current_node.y),Point(node[h].x,node[h].y),Scalar(100,105,200),2, LINE_8, 0 );
			cout<<"the distance is "<<node_nei[k].dist[n]<<",the rest distance is "<<node_nei[k].h_score[n];
		}
		current_node.x = node[k].x;
		current_node.y = node[k].y;

	}


}

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
				node_path[num].current_point[1][1]=current_node.x;
				node_path[num].current_point[1][2]=current_node.y;
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















//------------------------show the path--------------------------------
namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
imshow( "Display window", binary_image );
// Show image inside it.
cout<<"press any key to exit"<<endl;
waitKey(0);
// Wait for a keystroke in the window
return 0;
}

