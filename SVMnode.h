//
// Created by zhaolin on 11/15/18.
//

#ifndef PEDESTRIAN_MU_SVMNODE_H
#define PEDESTRIAN_MU_SVMNODE_H

#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <unistd.h>
#include <iomanip>
#include <opencv2/ocl/ocl.hpp>
using namespace std;
using namespace cv;
class SVMnode{
public:
    SVMnode(const int _drow , const int _dcol)
            :drow(_drow),dcol(_dcol)
    {

    }
    ~SVMnode(){}

public:
    double GetData(string &filename, int flag);
    void Zero(double t);
    void CalcIntegralImageInPlace();
    void CreateData(int row,int col){
        buf = new double[row*col];
        ptr = new double*[row];
        for(int i = 0 ; i<row ; i++)
        {
            ptr[i] = buf + i * col ;
            ///一系列矩阵首行指针
        }
    }
    double** ptr;
    double* buf;
    int drow;
    int dcol;
    double Th ; //阈值
};
//void SVMnode::CalcIntegralImageInPlace() {
//    for(int i =1 ; i<dcol;i++) {
//        buf[i] += buf[i - 1];
//    }
//    for(int i=1;i<drow;i++)
//    {
//        double partialsum = 0;
//        double* curp = ptr[i];
//        double* prep = ptr[i-1];
//        for(int j=0;j<dcol;j++)
//        {
//            partialsum += double(curp[j]);
//            curp[j] = prep[j] + partialsum;
//        }
//    }
//}
//void SVMnode::Zero(double t) {
//    std::fill(buf,buf+drow*dcol,t);
//}
#endif //PEDESTRIAN_MU_SVMNODE_H
