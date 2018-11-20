#include <cstdio>
#include <opencv2/opencv.hpp>

#include "CLD.h"
#include "postProcessing.h"

using namespace std;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    
    //minimal usage
    if (argc == 1)
        {
        string usagetxt ="Usage : cld-cli inputfile inputflowfile FDoGiterations sigma_m sigma_c rho tau outputfile";
        string usagetxt1="        cld-cli path/to/ima.0001.png path/to/tangentflow.0001.exr 3 2 1 .98 .98 path/to/cld.0001.png";
        string usagetxt2="        will generate cld.0001.png from ima.0001.png and tangentflow.0001.exr";
        //also save the Line integral convolution in a jpg image
        cout << usagetxt << "\n" << usagetxt1 << "\n" << usagetxt2 << endl;
        return 0;
        }
        
    //create a cdl object    
    CLD cld;
    
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
    bool withpreview=0;
    if (withpreview)
        {
        cv::namedWindow("cld");
        cv::imshow("cld", cld.result);
        cv::waitKey(0);
        }
}
