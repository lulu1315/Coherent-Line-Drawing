#include <cstdio>
#include <opencv2/opencv.hpp>

#include "CLD.h"
#include "postProcessing.h"

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    //create a cdl object    
    CLD cld;
    //init
        //originalImg
        //result
        //DoG
        //FDoG
    //etf.Init
        //flowField
        //refinedETF
        //gradientMag
        //sigma_m = 3.0;
        //sigma_c = 1.0;
        //rho = 0.997;
        //tau = 0.8;
    //refine etf 
    //combine image
    //create a etf object    
    //ETF etf;
    
    int ETF_kernel=atoi(argv[2]);
	int ETF_iteration=0;
	int FDoG_iteration=0;
    
    string inputfile = argv[1];
    cld.readSrc(inputfile);
    
    cld.sigma_m = 3.0;
	cld.sigma_c = 1.0;
	cld.rho = 0.997;
	cld.tau = atof(argv[3]);
    
    cld.etf.refine_ETF(ETF_kernel);
    //cld.combineImage();
    cld.genCLD();
    //cld.binaryThresholding(FDoG,result,tau);
    
    cv::namedWindow("cld");
    cv::imshow("cld", cld.result);
    cv::waitKey(5000);

}
