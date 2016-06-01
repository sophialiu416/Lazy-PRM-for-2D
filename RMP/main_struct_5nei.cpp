#include "halton.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include    <stdio.h>
#include    <ctype.h>
using namespace cv;
using namespace std;
#define PARENT(i) (i)/2
#define LEFT(i) 2*(i)+1
#define RIGHT(i) 2*(i+1)

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

void swap(int *a,int *b)
    {
        *a=*a^*b;
        *b=*a^*b;
        *a=*a^*b;
    }
    /*
    为了简单起见，这里只是单纯的选取第一个元素作为枢纽元素。这样选取枢纽，就难避免使得算法容易退化。
    */
    int k_big(int arr[],int low,int high,int k)
    {
        int pivot  = arr[low];
        int high_tmp = high;
        int low_tmp = low;
        while(low < high){
            //从右向左查找，直到找到第一个小于枢纽元素为止
            while (low < high && arr[high] >= pivot)
            {
                --high;
            }
            arr[low] = arr[high];
            //从左向右查找，直到找到第一个大于枢纽元素为止
            while (low < high && arr[low] <= pivot)
            {
                ++low;
            }
            arr[high] = arr[low];
        }
        arr[low] = pivot;

        if (low == k - 1)
        {
            return arr[low];
        }else if(low > k - 1)
        {
            return k_big(arr,low_tmp,low-1,k);
        }else
        {
            return k_big(arr,low+1,high_tmp,k);
        }
    }

float Distance(int x1,int y1,int x2,int y2)
	{
	 float dist;
	 dist=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
	 return dist;
	}

int main()
{

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
//--------------------------sample nodes--------------------------------------
//	SampleNodes(NodeList,image);
	_NodeListElement StartPoint;
    StartPoint.x = 100;
	StartPoint.y = 50;
	StartPoint.num = 0;
	NodeList.push_back (StartPoint);
	cv::circle(image, Point2d(StartPoint.x, StartPoint.y), 8, Scalar(50,150,100), 3, 8, 0);
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
	for (int i = 0; i < 301; i++)
	{
		_NodeListElement NewNode;
	    NewNode.num = i+1;
	    NewNode.x = (m_r[i])*displaced1;
	    NewNode.y = (m_r[i+1])*displaced2;
	    cv::circle(image, Point2d((m_r[i])*displaced1, (m_r[i+1])*displaced2), 8, Scalar(255,0,0), 1, 8, 0);
	 //       cv::putText(image,node[i].num , Point2d((m_r[i])*displaced1, (m_r[i+1])*displaced2), FONT_HERSHEY_SIMPLEX, 5 , Scalar(255,0,0), 1, 8, false );
	    NodeList.push_back(NewNode);
	 }
	 _NodeListElement GoalPoint;
	 GoalPoint.x = 1100;
	 GoalPoint.y = 750;
	 GoalPoint.num = 301;
	 NodeList.push_back(GoalPoint);
	 cout<<NodeList.size()<<endl;
	 cv::circle(image, Point2d(GoalPoint.x, GoalPoint.y), 8, Scalar(50,150,100), 3, 8, 0);


//-------------------------find edge---------------------------------------------------------------
//	    int Node_total_number = NodeList.size();
	 float d;
	 float hd;
	 int nei_num=0;
	 int Distance_list[302];
	 for ( int k = 0; k < NodeList.size(); k++ )
	 {
		 nei_num=0;
	     for ( int h = 0; h < NodeList.size(); h++ )
	     {
	    	 d = Distance(NodeList[k].x,NodeList[k].y,NodeList[h].x,NodeList[h].y);
	         hd = Distance(NodeList[NodeList.size()-1].x,NodeList[NodeList.size()-1].y,NodeList[h].x,NodeList[h].y);
	         Distance_list[h]=d;
	     }
	     k_big(Distance_list,0,301,6);
	     cout<<Distance_list[0]<<" , "<<Distance_list[2]<<" , "<<Distance_list[3]<<" , "<<Distance_list[4]<<" , "<<Distance_list[5]<<endl;
	     for ( int h = 0; h< NodeList.size(); h++)
	     {
	    	 d = Distance(NodeList[k].x,NodeList[k].y,NodeList[h].x,NodeList[h].y);
             hd = Distance(NodeList[NodeList.size()-1].x,NodeList[NodeList.size()-1].y,NodeList[h].x,NodeList[h].y);
             int dint= d;
             if (dint == Distance_list[1])
             {
            	 cout<<"new1"<<endl;
            	 _EdgeListElement NewEdge;
            	 NewEdge.node_start=k;
            	 NewEdge.node_end=h;
            	 NewEdge.distance_s_to_e=d;
            	 NewEdge.distance_e_to_goal=hd;
            	 EdgeList.push_back(NewEdge);
             }
             else if(dint == Distance_list[2])
             {
            	 cout<<"new2"<<endl;
	        	 _EdgeListElement NewEdge;
	        	 NewEdge.node_start=k;
	        	 NewEdge.node_end=h;
	        	 NewEdge.distance_s_to_e=d;
	        	 NewEdge.distance_e_to_goal=hd;
	        	 EdgeList.push_back(NewEdge);
	        	 //	        				 cv::line(image,Point(NodeList[k].x,NodeList[k].y),Point(NodeList[h].x,NodeList[h].y),Scalar(100,105,200),2, LINE_8, 0 );
	        	 /*	        				 cout<<NewEdge.node_start<<" ,the number is "<<NewEdge.node_end<<
	        	 	        						 " ,the distance is "<<NewEdge.distance_s_to_e<<" ,the rest distance is "<<
	        	 									 NewEdge.distance_e_to_goal<<endl;*/
	        	 	        				 //cout<<" the former number is "<<former_node[h][0]<<endl;
             }
             else if(dint == Distance_list[3])
             {
            	 cout<<"new3"<<endl;
	        				 _EdgeListElement NewEdge;
	        				 NewEdge.node_start=k;
	        				 NewEdge.node_end=h;
	        				 NewEdge.distance_s_to_e=d;
	        				 NewEdge.distance_e_to_goal=hd;
	        				 EdgeList.push_back(NewEdge);
	        }
	        else if(dint == Distance_list[4])
	        {
	        	cout<<"new4"<<endl;
	        				  _EdgeListElement NewEdge;
	        				  NewEdge.node_start=k;
	        				  NewEdge.node_end=h;
	        				  NewEdge.distance_s_to_e=d;
	        				  NewEdge.distance_e_to_goal=hd;
	        				  EdgeList.push_back(NewEdge);
	        }
	        else if(dint == Distance_list[5])
	        {
	        	cout<<"new5"<<endl;
	        				  _EdgeListElement NewEdge;
	        				  NewEdge.node_start=k;
	        				  NewEdge.node_end=h;
	        				  NewEdge.distance_s_to_e=d;
	        				  NewEdge.distance_e_to_goal=hd;
	        				  EdgeList.push_back(NewEdge);
	         }
	         NodeList[k].nei_num=nei_num;
	     }
	 }


	         //---------------------A star algorithm-----------------------------------
//	         int Edge_total_number = EdgeList.size();
//	         cout<<Edge_total_number<<endl;
//	         int Num_Looking_For = 0;
//	         int former_number = -1;
	     std::vector<int> firstformernodes;
    	 firstformernodes.push_back(0);
	     for (int k = 0; k <EdgeList.size();k++)
	     {
	    	 if (EdgeList[k].node_end==0)
	         {
	        		 NodeList[EdgeList[k].node_start].nei_num--;
	        		 EdgeList.erase(EdgeList.begin()+k);
	        	 	  cout<<"erase"<<k<<endl;

	         }
             if (EdgeList[k].node_start==0)
	        {
//	        		 cout<<EdgeList[k].node_end<<endl;
	        		 _PathListElement NewPath;
	        		 NewPath.node_now=0;
	        		 NewPath.node_next=EdgeList[k].node_end;
	        		 NewPath.distance_next_to_goal=EdgeList[k].distance_e_to_goal;
	        		 NewPath.distance_start_to_next=EdgeList[k].distance_s_to_e;
	        		 NewPath.distance_total_path=NewPath.distance_next_to_goal+NewPath.distance_start_to_next;
	        		 NewPath.formernodes= firstformernodes;
	        		 PathList.push_back(NewPath);
	       // 		 cout<<NewPath.node_next<<"the total distance is "<<NewPath.distance_total_path<<endl;

	        }
	     }
	     for (int n = 1; n<3000; n++)
	     {
	    	 int smallest_number = 0;
//	         cout<<EdgeList.size()<<endl;
//	         int Path_total_number = PathList.size();
	         cout<<"there are "<<PathList.size()<<" Paths"<<endl;
	         for (int k = 1; k < PathList.size(); k++)
	         {
	        		 //cout<<" path number: "<<k<<" ,distance: "<<PathList[k].distance_total_path<<endl;
	        	 if(PathList[k].distance_total_path<PathList[smallest_number].distance_total_path)
	        	 {
	        			 smallest_number = k;
	        	 }
	         }
	         cout<<"the smallest one is "<<PathList[smallest_number].node_next<<endl;
	         int a = PathList[smallest_number].node_now;
	       	 int b = 0;
	       	 int current_node = PathList[smallest_number].node_next;
	       	 PathList[smallest_number].distance_total_path=2000;
	       	 if(NodeList[current_node].nei_num==0)
	       	 {
	        		 cout<<"there are no nei_num"<<endl;
	         }
	         for (int k = 0; k <NodeList.size();k++)
	         {
	        		 if(NodeList[k].num==a)
	        			 a=k;
	        		 if(NodeList[k].num==current_node)
	        			 b=k;
	         }
	        	 //cv::line(image,Point(NodeList[a].x,NodeList[a].y),Point(NodeList[b].x,NodeList[b].y),Scalar(100,105,200),2, LINE_8, 0 );
	         for (int k = 0; k <EdgeList.size();k++)
	         {
	        	 if (EdgeList[k].node_end==current_node)
	        	 {
	        			 NodeList[EdgeList[k].node_start].nei_num--;
	        			 EdgeList.erase(EdgeList.begin()+k);
	        			 cout<<"erase"<<k<<endl;
	        	 }
	        	 if (EdgeList[k].node_start==current_node)
	        	 {
	        			 cout<<EdgeList[k].node_end<<endl;
	        		     _PathListElement NewPath;
	        		     NewPath.node_now=EdgeList[k].node_start;
	        		     NewPath.node_next=EdgeList[k].node_end;
	        		     NewPath.distance_next_to_goal=EdgeList[k].distance_e_to_goal;
	        		     NewPath.distance_start_to_next=EdgeList[k].distance_s_to_e+PathList[smallest_number].distance_start_to_next;
	        		     NewPath.distance_total_path=NewPath.distance_next_to_goal+NewPath.distance_start_to_next;
	        		     std::vector<int> formerNodes;
	        		     formerNodes= PathList[smallest_number].formernodes;
/*		                 for(std::vector<int>::iterator m = PathList[smallest_number].formernodes.begin(); m != PathList[smallest_number].formernodes.end(); m++ ) //用迭代器的方式输出容器对象的值
		                    {
		                   	 cout<<"shortest path node:  ";
		                    cout<<*m<<endl; //这里依次输出vector的值
		                    }
*/
	        		     formerNodes.push_back(EdgeList[k].node_start);
	        		     NewPath.formernodes= formerNodes;
	        		     PathList.push_back(NewPath);
	        		     if(NewPath.distance_next_to_goal<300)
	        		     {
	        		    	 cout<<"finish"<<endl;
	        		    	 break;
	        		     }
	        		    	// break;
		        		 cout<<NewPath.node_next<<"the total distance is "<<NewPath.distance_total_path<<endl;
//	        		 cout<<PathList[0].node_next<<PathList[0].distance_total_path<<endl;
		         }
	         }
             int Shortest_path = PathList.size()-1;
             cout<<Shortest_path<<endl;
             //std::vector<int> Nodes;
             //Nodes = PathList[Shortest_path].formernodes;
             int begin =*PathList[Shortest_path].formernodes.begin();
             for(std::vector<int>::iterator m = PathList[Shortest_path].formernodes.begin(); m != PathList[Shortest_path].formernodes.end(); m++ ) //用迭代器的方式输出容器对象的值
             {

            	  int end =*m;
	        	 cv::line(image,Point(NodeList[begin].x,NodeList[begin].y),Point(NodeList[end].x,NodeList[end].y),Scalar(100,105,200),2, LINE_8, 0 );
	        	 begin =*m;
             cout<<*m<<endl; //这里依次输出vector的值
             }


     //------------------------show the path--------------------------------
               namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
               imshow( "Display window", image );
     // Show image inside it.
               cout<<"press any key to exit"<<endl;
               waitKey(0);
     // Wait for a keystroke in the window
               return 0;
	     }
	 }
