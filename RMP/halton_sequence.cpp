/*
 * halton_sequence.cpp
 *
 *  Created on: May 8, 2016
 *      Author: sophialiu
 */

#include "halton.hpp"
#include "opencv2/opencv.hpp"
int main()
{
    int m_dim_num = 2;
    int m_n = 50;
    int m_seed[2], m_leap[2], m_base[2];
    double m_r[100];
    for (int i = 0; i < m_dim_num; i++)
    {
        m_seed[i] = 0;
        m_leap[i] = 1;
        m_base[i] = 2+i;
    }

    cv::Mat out(1000, 1000, CV_8UC1);
    i4_to_halton_sequence( m_dim_num, m_n, 0, m_seed, m_leap, m_base, m_r);

    int displaced = 100;
    for (int i = 0; i < 100; i=i+2)
    {
        cv::circle(out, cv::Point2d((m_r[i])*displaced, (m_r[i+1])*displaced), 1, cv::Scalar(255, 255, 255), 1, 8, 0);
    }
    cv::imshow("test", out);
    cv::waitKey(0);

    return 0;
}
