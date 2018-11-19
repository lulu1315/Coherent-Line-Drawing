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
    int ETF_kernel=5;
	int ETF_iteration=0;
    
    char *inputfile = argv[1];
    char *inputflowfile  = argv[2];
	int FDoG_iteration=atoi(argv[3]);
    cld.sigma_m =  atof(argv[4]); //3.0
	cld.sigma_c =  atof(argv[5]); //1.0
	cld.rho =      atof(argv[6]); //0.98
	cld.tau =      atof(argv[7]); //0.98
    char *outputfile  = argv[8];
    
    //read source image
    //do the etf once , should avoid that
    cld.readSrc(inputfile);

    cld.etf.flowField = cv::imread(inputflowfile,CV_LOAD_IMAGE_UNCHANGED);  
    if(! cld.etf.flowField.data )                              // Check for invalid input
        {
        cout <<  "Could not open or find a precomputed flowfield" << std::endl ;
        return -1;
        }

    cv::cvtColor(cld.etf.flowField, cld.etf.flowField, CV_RGB2BGR);
    cld.genCLD();
    
    for(int i=0; i<FDoG_iteration; i++) 
        {
        cout << "\rFDoG iteration  " << i+1 << "/" << FDoG_iteration << flush;
        cld.combineImage();
        cld.genCLD();
        }
        cout << endl;
    
    cv::imwrite(outputfile, cld.result);
    //preview
    bool withpreview=1;
    if (withpreview)
        {
        cv::namedWindow("cld");
        cv::imshow("cld", cld.result);
        cv::waitKey(0);
        }
}
