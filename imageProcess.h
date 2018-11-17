//
// Created by zhaolin on 11/14/18.
//

#ifndef PEDESTRIAN_MU_IMAGEPROCESS_H
#define PEDESTRIAN_MU_IMAGEPROCESS_H

#include "SVMnode.h"

class ImageProcess{
public:
    ImageProcess():bg_model(20, 5 , 0.9 , 2.5*2.5),LINnode(1,6144),HIKnode(6144,353),lin_score(1,6144)
    {
    }
    void ImageInit(Mat &img); //初始化图像
    void showSobel();
    void showImage();
    void BackGround(bool show);
    void computeCT(Mat &image,bool show);

    SVMnode LINnode;
    SVMnode HIKnode;
    SVMnode lin_score;

    Mat srcimage,sobel;
    Mat ct;

    int** ct_ptr;

    int** blockptr;
    int* blockbuf;

    int row;
    int col; //在ImageInit()中初始化

    int height = 108;
    int width = 36;
    int x_div = 9;
    int y_div = 4;

    void Scan();
    void createData(int row,int col);
    void Data_Zero();
public:

    BackgroundSubtractorMOG bg_model;

};


//void ImageProcess::createData(int row,int col){
//    blockbuf = new int[row*col];
//    blockptr = new int*[row];
//    for(int i = 0 ; i<row ; i++)
//    {
//        blockptr[i] = blockbuf + i * col ;
//        ///一系列矩阵首行指针
//    }
//}
//void ImageProcess::Data_Zero(){
//    std::fill(blockbuf,blockbuf+6144,0);
//}


#endif //PEDESTRIAN_MU_IMAGEPROCESS_H
