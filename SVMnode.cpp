//
// Created by zhaolin on 11/15/18.
//

#include "SVMnode.h"
#include <fstream>


double SVMnode::GetData(string &filename,int flag) {
    if(flag==0){
        ifstream infile(filename);
        std::cout<<"加载线性linear SVM 参数模型"<<filename<<std::endl;
        if(infile.good()==false)
        {
            std::cout<<"SVM 线性模型加载失败...."<<std::endl;
            exit(-1);
        }
        std::string buffer;
        std::getline(infile,buffer);std::getline(infile,buffer);std::getline(infile,buffer);
        //读取solver_type \ nr_class \ label

        infile>>buffer; assert(buffer=="nr_feature");
        int dim_number; infile>>dim_number;std::getline(infile,buffer);         //维度数所在行结束

        std::getline(infile,buffer);
        infile>>buffer ;
        assert(buffer=="w") ;
        getline(infile,buffer);
        //bias一行, w一行

        CreateData(1,6144) ;

        for(int i=0 ; i< dim_number;i++)
        {
            infile>>buf[i];
//            std::cout<<buf[i]<<std::endl;
        }
        //导入６１４４维度参数

        double th ;
        infile >> th ;
        std::cout<<"Th : "<<th<<std::endl;
        infile.close();
        return th;
    }
    //////////////////////////以上是读取lin模型的参数
    else if(flag == 1){
        ifstream infile(filename);
        std::cout<<"加载HIK SVM模型..."<<filename<<std::endl;
        if(infile.good()==false)
        {
            cout<<"加载HIK模型失败..."<<endl;
            exit(-1);
        }
        string buffer;
        getline(infile,buffer);
        std::cout<<buffer<<std::endl;
        getline(infile,buffer);
        std::cout<<buffer<<std::endl;
     //   assert(buffer=="nr_class 2");
        getline(infile,buffer);
       // assert(buffer=="label 1 -1");
        std::cout<<buffer<<std::endl;
        //1   \   2    \   3
        infile>>buffer;
        assert(buffer=="nr_feature");
        int dim_number;
        infile>>dim_number;
        assert(dim_number==6144);
        std::getline(infile,buffer);         //维度数所在行结束  4
        std::cout<<buffer<<std::endl;
        std::getline(infile,buffer);
        std::cout<<buffer<<std::endl;
        infile>>buffer ;
        assert(buffer=="w") ;
        getline(infile,buffer);
        std::cout<<buffer<<std::endl;
        //bias一行, w一行

        CreateData(6144,353);
        for(int i= 0 ; i<6144;i++)
            for(int j=0;j<353;j++)
            {
                infile>>ptr[i][j];
//                std::cout<<ptr[i][j]<<std::endl;
            }

        double th ;
        infile >> th ;
        std::cout<<th<<std::endl;
        infile >> th ;
        std::cout<<th<<std::endl;
        infile.close();
        return th;
    }
    else {
        std::cout<<"no flag , 参数错误"<<std::endl;
        return 0;
    }
}

void SVMnode::Zero(double t) {
    std::fill(buf,buf+drow*dcol,t);
}

void SVMnode::CalcIntegralImageInPlace() {
    for(int i =1 ; i<dcol;i++) {
        buf[i] += buf[i - 1];
    }
    for(int i=1;i<drow;i++)
    {
        double partialsum = 0;
        double* curp = ptr[i];
        double* prep = ptr[i-1];
        for(int j=0;j<dcol;j++)
        {
            partialsum += double(curp[j]);
            curp[j] = prep[j] + partialsum;
        }
    }
}