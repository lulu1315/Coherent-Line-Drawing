#include <cstdio>
#include <opencv2/opencv.hpp>

#include "CLD.h"
#include "postProcessing.h"

using namespace std;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    //create a cdl object    
    CLD cld;
    
    char *inputfile = argv[1];
    char *inputext  = argv[2];
    int ETF_kernel=    atoi(argv[3]);
	int ETF_iteration= atoi(argv[4]);
	int FDoG_iteration=atoi(argv[5]);
    
    cld.sigma_m =  atof(argv[6]); //3.0
	cld.sigma_c =  atof(argv[7]); //1.0
	cld.rho =      atof(argv[8]); //0.98
	cld.tau =      atof(argv[9]); //0.98
    
    bool doetf = atoi(argv[10]);
    //string outputfile = argv[10];
    
    //read source image
    char inputname[200];
    sprintf(inputname,"%s.%s",inputfile,inputext);
    cld.readSrc(inputname);
    
    //etf
    char flowfieldname[200];
    sprintf(flowfieldname,"%s_flowfield.exr",inputfile);
    
    if (doetf)
        {
        for(int i=0; i<ETF_iteration; i++) 
            {
            cout << "\rETF  " << i+1 << "/" << ETF_iteration << flush;
            cld.etf.refine_ETF(ETF_kernel);
            }
        cout << endl;
        cv::imwrite(flowfieldname, cld.etf.flowField);
        }
    else
        {
        cld.etf.flowField = cv::imread(flowfieldname,CV_LOAD_IMAGE_UNCHANGED);  
        if(! cld.etf.flowField.data )                              // Check for invalid input
            {
            cout <<  "Could not open or find a precomputed flowfield" << std::endl ;
            return -1;
            }
        }
    cld.genCLD();
    
    for(int i=0; i<FDoG_iteration; i++) 
        {
        cout << "\rFDoG iteration  " << i+1 << "/" << FDoG_iteration << flush;
        cld.combineImage();
        cld.genCLD();
        }
        cout << endl;
    
    //cld.binaryThresholding(FDoG,result,tau);
    char outputname[200];
    //sprintf(outputname,"%s_result.%s",inputfile,inputext);
    sprintf(outputname,"%s_result.pgm",inputfile);
    cv::imwrite(outputname, cld.result);
    //preview
    cv::namedWindow("cld");
    cv::imshow("cld", cld.result);
    cv::waitKey(0);
}

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
