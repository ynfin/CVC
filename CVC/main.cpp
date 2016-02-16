//
//  main.cpp
//  CVC
//
//  Created by Yngve Finnestad on 15/02/16.
//  Copyright © 2016 Yngve Finnestad. All rights reserved.
//
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char * argv[]) {
    
    int mid = 35;
    
    Mat imageInput;
    Mat imageProcessedHSV;
    Mat imageProcessedRGB;
    Mat imageProcessedGray;
    Mat imageProcessedGrayGauss;
    Mat imageProcessedGrayErode;
    Mat imageProcessedGrayDilate;
    
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);
    
    imageInput = imread("/Users/yngve/Downloads/dots.jpg", CV_LOAD_IMAGE_COLOR);
    
    
    // --------- while start ---------
    
while (true)
    {
        imageProcessedRGB = imageInput.clone();
    
        cvtColor(imageProcessedRGB, imageProcessedHSV, CV_BGR2HSV);
        inRange(imageProcessedHSV, Scalar(mid-1,1,1), Scalar(mid+1,255,255), imageProcessedGray);
        
        erode(imageProcessedGray, imageProcessedGrayErode, Mat(), Point(-1, -1), 2, 1, 1);
        dilate(imageProcessedGrayErode, imageProcessedGrayDilate, Mat(), Point(-1, -1), 2, 1, 1);
        GaussianBlur(imageProcessedGrayDilate, imageProcessedGrayGauss, Size(9,9), 5);

        findContours( imageProcessedGrayGauss, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

        /// Get the moments
        vector<Moments> mu(contours.size() );
        for( int i = 0; i < contours.size(); i++ )
        { mu[i] = moments( contours[i], false ); }
    
        ///  Get the mass centers:
        vector<Point2f> mc( contours.size() );
        for( int i = 0; i < contours.size(); i++ )
        { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
    
        for( int i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours( imageProcessedRGB, contours, i, color, 2, 8, hierarchy, 0, Point() );
            circle( imageProcessedRGB, mc[i], 4, color, -1, 8, 0 );
            cout << mc[i] << endl;
            
            for( int j = 0; j< contours.size(); j++ )
            {
                if (i != j) {
                    line(imageProcessedRGB, mc[i], mc[j], Scalar(0,0,255));
                }
            }
        }
        
        cout << "\n" << endl;
        
        namedWindow( "avfterrange", CV_WINDOW_AUTOSIZE );
        imshow("avfterrange", imageProcessedRGB);
    
        // Wait until user press some key for 50ms
        int iKey = waitKey(50);
        if (iKey == 27)
        {
            break;
        }
    }

    return 0;
}
