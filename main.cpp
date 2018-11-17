#include "imageProcess.h"
#include "SVMnode.h"
#include <Eigen/Eigen>
using namespace Eigen;
ImageProcess imgPro ;
vector<string> _imagefilename ;
vector<string> _imageTime;
void ScanImage(int stepsize , std::vector<cv::Rect>& results);
int main(int argc,char* argv[]) {
    std::cout << "start Pedestrian detection!\n please enter the image file path"<< std::endl;
    if(argc!=2)
    {
        std::cout<<"Parameter is not true"<<std::endl;
    }

    string _image_path = "../../../Pedestrian_Detection/View_001/";
            //string(argv[1]);
    DIR *currentdir = opendir(_image_path.c_str());
    if(!currentdir){
        cout<<"the path of the image files is not correct~ "<<endl;
        return 0;
    }
    struct dirent *ent;
    int filecount = 0;
    while( (ent=readdir(currentdir)) != NULL )
    {
        filecount++;
        ///cout<<ent->d_name<<endl; ///这里会输出所有的图片的文件名+Linux系统下的 “.” 和 “..”
    }   ///文件数目 = filecount - 2
    filecount -= 2;
    cout<<"the number of image files is : "<<filecount<<endl;
    for(int i=0;i<filecount;i++) {
        stringstream ss;
        string path;
        ss << setfill('0') << setw(4) << i ;
        ss >> path ;
        _imagefilename.push_back(_image_path+"frame_"+path+".jpg");
    }

    ///读取SVM linear模型
    string path_linear = "linear.model";
    imgPro.LINnode.Th  =  imgPro.LINnode.GetData(path_linear,0);
    imgPro.LINnode.Th +=3.0;

    ///读取SVM HIK模型
    string path_HIK = "HIK.model";
    imgPro.HIKnode.Th  =  imgPro.HIKnode.GetData(path_HIK,1);
    imgPro.HIKnode.Th -=-1.2;

    for(int i=0;i<600;i+=1)
    {
        Mat image = imread(_imagefilename[i],1);
        imgPro.ImageInit(image);
//        imgPro.showImage();
        imgPro.showSobel();
        imgPro.BackGround(true);
        imgPro.computeCT(imgPro.srcimage,true); //开启显示
        std::cout<<"read the "<<i<<"th image file: "<<_imagefilename[i]<<endl;

        std::vector<cv::Rect> rects;
        ScanImage(2,rects);
        for(int i = 0 ; i<rects.size(); i++)
            rectangle(imgPro.srcimage,rects[i],Scalar(0,0,255));
        namedWindow("srcimage",1);
        imshow("srcimage",imgPro.srcimage);


        waitKey(100);
    }
    return 0;
}

void Integral(int stepsize)
{
    imgPro.lin_score.CreateData(imgPro.row,imgPro.col);
    double div = (12*2-2)*(9*2-2);
    imgPro.lin_score.Zero(imgPro.LINnode.Th/div);

    double* LINnode_buf = imgPro.LINnode.buf;
    for(int i =0 ; i<8 ;i++)
    {
        int x_base = 12 * i ;
        for(int j = 0 ; j<3 ; j++)
        {
            int y_base = 9 * j ;
            for(int x = 2 ; x<imgPro.row-2-x_base;x++)
            {
                uchar* ct_ptr = imgPro.ct.ptr<uchar>(x+x_base)+y_base;

                for(int y=2;y<imgPro.col-2-y_base;y++)
                {
                    int value = (int)ct_ptr[y];
//                    std::cout<<value<<std::endl;
                    imgPro.lin_score.ptr[x][y] += LINnode_buf[value];
                }
            }
            LINnode_buf += 256 ;
        }
    }

    imgPro.lin_score.CalcIntegralImageInPlace();
    std::cout<<"imgPro.row-2-12"<<(imgPro.row-2-12)<<std::endl;
    for(int i =2;i<imgPro.row-2-imgPro.height;i+=stepsize)
    {
        double* p1 = imgPro.lin_score.ptr[i];
        double* p2 = imgPro.lin_score.ptr[i+22];
        for(int j = 2 ; j<imgPro.col-2-imgPro.width; j+=stepsize)
        {
            p1[j] += (p2[j+16] - p2[j] - p1[j+16]);
        }
    }

}

void ScanImage(int stepsize , std::vector<cv::Rect>& results)
{
    imgPro.createData(1,256*(8*3));  //256x24 = 6144
    results.clear();
    Integral(2) ; //积分图 步长2
    for(int i = 2 ; i <(imgPro.row - 2 - imgPro.height) ; i = i + stepsize){
        for(int j = 2 ;j<(imgPro.col - 2 -imgPro.width) ; j = j + stepsize)
        {
            if(imgPro.lin_score.ptr[i][j]<=0)
                continue; //根据线性核的分类得分提高检测速率
            imgPro.Data_Zero();  //超级块直方图置0

            int* blockbuf_ptr = imgPro.blockbuf ;

            ///前面是(108x36)大小的检测窗口遍历整张图像的循环;
            for(int k = 0 ; k < 8 ; k++)
                for(int t = 0 ; t < 3 ; t++)
                {
                    ///前面是 8x3 超级块循环,下面统计直方图信息
                    for(int x=i+12*k+1; x<i+12*(k+2)-1 ; x++ )
                    {
                        //超级块内部循环 24x18的超级块 去掉边缘单位=1的像素,遍历为22x16=352维度
                        for(int y=j+9*t+1 ; y<j+9*(t+2)-1; y++)
                        {
//                            int value = (int)imgPro.ct.ptr<uchar>(x)[y];
                            int CT_value = (int)imgPro.ct.ptr<uchar>(x)[y];
                            blockbuf_ptr[CT_value]++;
                        }
                    }

                    if(0) //第一个超级块
                    {
                        Matrix<int, 16, 16> po_1;
//                            int po[16][16];
                        for (int ii = 0; ii < 16; ii++) {
                            for (int jj = 0; jj < 16; jj++) {
                                po_1(ii, jj) = blockbuf_ptr[jj + ii * 16]; //p是int* 指针, []可取值赋值
                            }
                        }
                        std::cout << "第一个超级块：\n" << po_1 << std::endl;
                        /*
                         * 第一个超级块：
18  2 20  3  2  0  1  2  6  4  0  1  2  0  3  2
2  1  0  1  3  2  1  2  2  0  0  0  0  1  0  1
0  0  0  1  0  0  3  0  2  5  2  7  0  1  0  0
1  0  0  1  0  0  1  0  0  1  0  1  0  1  1  4
11  2  1  0  1  0  1  0  0  3  0  2  0  0  0  0
0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  1
1  1  0  1  1  0  0  0  3 16  0  6  1  3  1  1
0  0  0  0  0  1  0  0  1  2  0  1  0  4  0  1
3  0  1  0  0  0  0  0  0  0  0  2  0  0  0  1
2  1  0  0  7  0 18  2  0  0  0  1  2  0  0  1
0  0  0  0  0  0  0  0  0  2  0  0  0  0  0  0
1  0  0  0  1  0  1  4  1  3  0  1  0  1  2  5
1  0  2  0  0  1  0  0  0  1  0  1  0  0  0  0
4  0  0  0  5  1  5  0  0  0  0  0  0  2  0  4
0  1  0  1  0  1  0  1  0  3  1  3  1  2  1  4
0  1  0  2  3  0  2  5  2  2  0  2  0  7  6 30
../View_001/frame_0000.jpg
                         * */
                    }
                    blockbuf_ptr +=256;
                }
            //---------------------24个256维直方图
            double thresh = imgPro.HIKnode.Th ;
//            std::cout<<"thresh"<<thresh<<std::endl;
            for(int a=0;a<6144;a++)
            {
                int Hist_256 = imgPro.blockbuf[a];
                thresh  += imgPro.HIKnode.ptr[a][ Hist_256 ];
            }
//            std::cout<<"thresh"<<thresh<<std::endl;
            if(thresh>0)
            {
                cv::Rect rect(i,j,36,108);
                results.push_back(rect);
            }

        }
    }
}