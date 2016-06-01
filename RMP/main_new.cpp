/*
 * main_new.cpp
 *
 *  Created on: May 21, 2016
 *      Author: sophialiu
 */


#include "halton.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <strstream>
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
using namespace cv;
using namespace std;

float Distance(int x1,int y1,int x2,int y2)
	{
	 float dist;
	 dist=sqrt(((x1-x2)*(x1-x2))+((y1-y2)*(y1-y2)));
//		 cout<<dist<<endl;
	 return dist;
	}

class Sample
{public:
	int x;
	int y;
	int num;
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
	int current_point[];
	int nei_point[];
	int nei_num;
	float dist[];
	float h_score[];
//	Edge(int,int,int,int,double);

};

class Astarpath{
public:
	int start_point;
	int current_point;
	string point_been;
	int point_been_number;
	float g_score;
	float h_score;
	float f_score;


};
int open_list[302];
int close_list[302];
float former_node[301][2]= {0};
float edges[1500][4];


//Edge::Edge(int x,int y, int x2,int y2, double dist): Sample(x, y){

//}

int main(int argc, char** argv)
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
//--------------------------sample nodes1--------------------------------------
	Sample node[302];
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
    for (int i = 0; i < 301; i++)
    {
    	node[i+1].num = i+1;
     	node[i+1].x = (m_r[i])*displaced1;
     	node[i+1].y = (m_r[i+1])*displaced2;
        cv::circle(image, Point2d((m_r[i])*displaced1, (m_r[i+1])*displaced2), 8, Scalar(255,0,0), 1, 8, 0);
 //       cv::putText(image,node[i].num , Point2d((m_r[i])*displaced1, (m_r[i+1])*displaced2), FONT_HERSHEY_SIMPLEX, 5 , Scalar(255,0,0), 1, 8, false );
     }
     node[301].x = 1000;
     node[301].y = 850;
     node[301].num = 301;
     cv::circle(image, Point2d(node[300].x, node[300].y), 8, Scalar(100,150,100), 1, 8, 0);
     //------------------------find neighbor nodes--------------------------------
     Edge node_nei[302];
     float d;
     float hd;
          int edge_totalnum = 0;
          int n=0;
          for ( int k = 0; k < 302; k++ )
          {
        	  n = 0;
         	 for ( int h = 0; h < 302; h++ )
         	 {
         		 d = Distance(node[k].x,node[k].y,node[h].x,node[h].y);
         		 hd = Distance(node[301].x,node[301].y,node[h].x,node[h].y);
         		 if (d > 0)
         		 {
         			 if (d <= 70)
         			 {
         				 n = n+1;
         				edge_totalnum = edge_totalnum +1;
         				 edges[edge_totalnum][0]=k;
         				 edges[edge_totalnum][1]=h;
         				 edges[edge_totalnum][2]=d;
         				 edges[edge_totalnum][3]=hd;

     //			node_nei[k].current_point[1][1] = current_node.x;
     	//		node_nei[k].current_point[1][2] = current_node.y;
     //			node_nei[k].nei_point[n][1] = node[h].x;
     	//		node_nei[k].nei_point[n][2] = node[h].y;
     	//		node_nei[k].dist[n] = d;
     	//		node_nei[k].h_score[n]=hd;

         				 node_nei[k+1].nei_num = n;

         				// cv::line(image,Point(node[k].x,node[k].y),Point(node[h].x,node[h].y),Scalar(100,105,200),2, LINE_8, 0 );
         				 cout<<edges[edge_totalnum][0]<<" ,the number is "<<edges[edge_totalnum][1]<<
         						 " ,the distance is "<<edges[edge_totalnum][2]<<" ,the rest distance is "<<
								 edges[edge_totalnum][3]<<endl;
         				 //cout<<" the former number is "<<former_node[h][0]<<endl;
         			 }
         		 }
         	 }

          }
     //     cout<<node_nei[0].nei_num<<" , "<<node_nei[1].nei_num<<" , "<<node_nei[2].nei_num<<" , "<<node_nei[3].nei_num<<endl;


          //------------------------A star algorithm--------------------------------
          //open_list[] = Astarpath::Sample::node[];
          Astarpath node_path[100];
          cout<<"total path is "<<edge_totalnum<<endl;
          //close_list[1]=node[0].num;
          //	cout<<close_list[1][1]<<close_list[1][2];
          int num = 0;
          int path_num = 1;
          int last = 0;
          int start = 0;
          node_path[0].point_been = "0";
          node_path[0].g_score = 0.0;
          int num_edge = 1;
          int edge_number_of_node = 0;
          node_path[0].point_been_number = 0;
          former_node[0][0]=0;
          former_node[0][1]=0;
          int formernum;
          int node_deleted;

          for ( int run = 1; run < 100; run++ )
          {
        	  edge_number_of_node = 0;
        	  //num_edge = 1;
        	  for( int a = 0; a < num+1; a++)
        	  {


        		  edge_number_of_node = edge_number_of_node + node_nei[a].nei_num;
        	  }

              cout<<" start at "<<edge_number_of_node<<" finish at "<<edge_number_of_node + node_nei[num+1].nei_num<<endl;

      //        node_path[path_num].point_been_number=node_path[path_num-1].point_been_number+1;

        	  for ( int k = edge_number_of_node+1; k < edge_number_of_node + node_nei[num+1].nei_num+1; k++ )
        	  {
       		  if(edges[k][0] < 2000){
        			  //strstream ss;
        		//  string s;
        		//  ss << num;
        		//  ss >> s;
        		//      cout<<s<<endl;
        			  cout<<"Number "<<k<<" and ";
        			  node_path[path_num+1].point_been_number=node_path[path_num].point_been_number;
        		//	  cout<<node_path[path_num].point_been_number<<endl;
        			  node_path[path_num].start_point=edges[k][0];
        			  node_path[path_num].current_point=edges[k][1];
      				 former_node[node_path[path_num].current_point][0] = edges[k][0];
      				 former_node[node_path[path_num].current_point][1] = edges[k][2];
        		//	  node_path[path_num].g_score=node_path[node_path[path_num].point_been_number].g_score+edges[k][3];
        		//	  node_path[path_num].g_score=edges[k][3];
        		//	  node_path[path_num].f_score=node_path[path_num].g_score+edges[k][4];

        		//	  cout<<"start x: "<<node[num].x<<" start y: "<<node[num].y<<endl;
        		//	  cout<<"end x: "<<node[node_path[path_num].current_point].x<<" end y: "<<node[node_path[path_num].current_point].y<<endl;

        		//	  cv::line(image,Point(node[num].x,node[num].y),Point(node[node_path[path_num].current_point].x,node[node_path[path_num].current_point].y),Scalar(100,105,200),2, LINE_8, 0 );
        		//	  cout<< "path num "<<edges[k][2] << " : " <<node_path[path_num].f_score<< " : " << last << endl;
        			  //	  node_path[path_num].point_been=node_path[last].point_been+" "+s;

        		//	  int pb[7]={999,999,999,999,999,999,999};
        		//	  sscanf(((node_path[path_num].point_been).c_str()),"%d %d %d %d %d %d %d", &pb[0], &pb[1], &pb[2], &pb[3], &pb[4], &pb[5], &pb[6]);
        		//	  printf("%d , %d , %d , %d , %d , %d , %d  \n", pb[0], pb[1], pb[2], pb[3], pb[4], pb[5], pb[6]);
        		//	  double dis_g=0;
        			  float dis_total=0;
        			  int position=edges[k][0];
        			  cout<<" the former number now is "<<edges[k][0]<<", the current number now is "<<edges[k][1]<<endl;
        		//	  while(1){
        				  if(position < 1)
        				  {
        					  dis_total=dis_total+edges[k][2];
        					  node_path[path_num].g_score= dis_total;
        					  node_path[path_num].f_score=node_path[path_num].g_score+edges[k][3];
        					//  break;
        				  }
        				  else
        				  {
        					  formernum = former_node[position][0];
        					  cout<<"the former number of the former number is"<<formernum;
        					  dis_total=dis_total+ former_node[formernum][1];
        					  cout<<" and the g_score now is "<<dis_total<<endl;
        				  }


        		//	  }
        		//	  node_path[path_num].g_score= dis_total+edges[k][2];
        		//	  node_path[path_num].f_score=node_path[path_num].g_score+edges[k][3];
        		//	  cout<<edges[k][1]<<" , "<<edges[k][2] <<" , "<<edges[k][3]<<endl;

        		//	  for(int r=1;r<node_path[path_num].point_been_number;r++)
        		//	  {
        		//		  dis_g=Distance(node[pb[r]].x,node[pb[r]].y,node[pb[r+1]].x,node[pb[r+1]].y);
        		//		  cout<<dis_g;
        		//		  dis_total=dis_total+dis_g;
        		//	  }
        		//	  node_path[path_num].g_score=dis_total;
        		//	  node_path[path_num].f_score=node_path[path_num].g_score+edges[k][4];
        		//	  cout<<pb[node_path[path_num].point_been_number]<<endl;
        			  cout<<"the g score is "<<node_path[path_num].g_score<<", the former one is "<<edges[k][2]<<endl;
        		//	  cout<<node_path[path_num].point_been<<endl;
        			  path_num = path_num + 1;

        	  }
        	  }
        	  int smallest_f = 1;
        	  for  (int b = 2; b < path_num ; b++)
        	  {
        		  if(node_path[b].f_score<node_path[smallest_f].f_score)
        			  smallest_f = b;
        	  }
        	  last = smallest_f ;

        //	  int node_should_be_deleted = node_path[last]
        //	  start= node_path[last].current_point;
        //	  cv::line(image,Point(node[start].x,node[start].y),Point(node[num].x,node[num].y),Scalar(100,105,200),2, LINE_8, 0 );
        	  num = node_path[last].current_point;
              node_deleted = node_path[last].start_point;
              cv::line(image,Point(node[num].x,node[num].y),Point(node[node_deleted].x,node[node_deleted].y),Scalar(100,105,200),2, LINE_8, 0 );
              cout<<node_deleted<<endl;
              for(int kk=0;kk<302;kk++)
              {
            	  if(edges[kk][1] == node_deleted)
            	  {
            		  cout<<"find node "<<edges[kk][0]<<endl;
            		  edges[kk][0] = 2000;
            		  edges[kk][1] = 2000;
            		  edges[kk][2] = 2000;
            		  edges[kk][3] = 2000;
            	  }
              }
        	  cout<<"the one I choose now is Number "<<num<<"the f_score is "<<node_path[last].g_score<<endl;

        	  //num = num - 1;
        	  node_path[last].f_score=9999;

        	  if(edges[num][3]<200)
        	  {
        		//  strstream ss;
        		//  string s;
        		//  ss << num-1;
        		//  ss >> s;
        		//  node_path[path_num].point_been=node_path[path_num-1].point_been+" "+s;

        		    //  int len=strlen((node_path[path_num].point_been).c_str());
        		//  int pbn=node_path[path_num].point_been_number;
        		//      int a[pbn];

        		//      sscanf(((node_path[path_num].point_been).c_str()),"%d %d %d %d %d %d %d", &a[0], &a[1], &a[2], &a[3], &a[4], &a[5], &a[6]);
        		    //  sscanf (sentence,"%s %*s %d",str,&i);
        		//      printf("\n#####\n%d , %d , %d , %d , %d , %d , %d  \n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
        		//      for(int b=1;b<6;b++){
        		//      cv::line(image,Point(node[a[b]].x,node[a[b]].y),Point(node[a[b+1]].x,node[a[b+1]].y),Scalar(100,105,200),2, LINE_8, 0 );
        		//      }



            	//  cout<<"the node has been reached through "<<node_path[path_num].point_been<<endl;
        		  cout<<"finish"<<endl;
        		  break;

        	  }
    //    	  }

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



