//
// Created by zhaolin on 11/14/18.
//
#include "imageProcess.h"

void ImageProcess::ImageInit(Mat &img) {
    row = img.rows;
    col = img.cols;

    int scale = 1;
    int delta = 0;
    GaussianBlur(img,img,Size(3,3),0,0);
    cvtColor(img,srcimage,CV_BGR2GRAY);
    Mat grad_x,grad_y;
    Mat abs_grad_x,abs_grad_y;
    Sobel(srcimage,grad_x,CV_16S,1,0,3,1,0,BORDER_DEFAULT);
//    Scharr(srcimage,grad_x,CV_16S,1,0,scale,delta,BORDER_DEFAULT);
    convertScaleAbs(grad_x,abs_grad_x); //将中间结果转换到 CV_8U:
    Sobel(srcimage,grad_y,CV_16S,0,1,3,1,0,BORDER_DEFAULT);
//    Scharr(srcimage,grad_y,CV_16S,0,1,scale,delta,BORDER_DEFAULT);
    convertScaleAbs(grad_y,abs_grad_y);

    addWeighted(abs_grad_x,0.5,abs_grad_y,0.5,0,sobel);
    //computes weighted sum of two arrays (dst = alpha*src1 + beta*src2 + gamma)
    //将两个方向的梯度相加来求取近似 梯度

}
void ImageProcess::showSobel() {
    imshow("sobel",sobel);
}
void ImageProcess::showImage() {
    imshow("image",srcimage);
}
void ImageProcess::BackGround(bool show) {
    Mat mask;
    bg_model(srcimage,mask,0.007);
    if(show)
     imshow("mask",mask);
    else ;
}
void ImageProcess::computeCT(Mat &image ,bool show) {
//    for(int k=0;k<row;k++)
//    {
//        ct_ptr[k] = ct.ptr<int>(k);
//    }

    ct = Mat::zeros(image.size(),CV_8UC1);
    for(int i=2;i<image.rows-2;i++)
    {
        uchar* p1 = image.ptr<uchar>(i-1);
        uchar* p2 = image.ptr<uchar>(i);
        uchar* p3 = image.ptr<uchar>(i+1);
        uchar* p = ct.ptr<uchar>(i);
        for(int j = 2 ; j < image.cols - 2 ; j++)
        {
            uchar index = 0;
            if(p2[j]<=p1[j-1]) index+=128;
            if(p2[j]<=p1[j]) index+=64;
            if(p2[j]<=p1[j+1]) index+=32;
            if(p2[j]<=p2[j-1]) index+=16;
            if(p2[j]<=p2[j+1]) index+=8;
            if(p2[j]<=p3[j-1]) index+=4;
            if(p2[j]<=p3[j]) index+=2;
            if(p2[j]<=p3[j+1]) index+=1;
            *p = index ;
            p++;
        }
    }
    if(show)
     imshow("CT",ct);
    else ;
}

void ImageProcess::Scan() {
//    createData(1,6144);
}

void ImageProcess::Data_Zero(){
    std::fill(blockbuf,blockbuf+6144,0);
}

void ImageProcess::createData(int row,int col){
    blockbuf = new int[row*col];
    blockptr = new int*[row];
    for(int i = 0 ; i<row ; i++)
    {
        blockptr[i] = blockbuf + i * col ;
        ///一系列矩阵首行指针
    }
}