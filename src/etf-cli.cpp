#include <cstdio>
#include <opencv2/opencv.hpp>

#include "CLD.h"
#include "postProcessing.h"

using namespace std;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    bool withpreview=0;
    
    //minimal usage
    if (argc == 1)
        {
        string usagetxt ="Usage : etf-cli inputfile kernelsize iterations outputgradient outputtangent";
        string usagetxt1="        etf-cli /path/to/input/ima.0001.png 5 3 /path/to/result/gradient.0001.exr /path/to/result/tangentflow.0001.exr";
        string usagetxt2="        will generate gradient.0001.exr and tangentflow.0001.exr from ima.0001.png";
        //also save the Line integral convolution in a jpg image
        cout << usagetxt << "\n" << usagetxt1 << "\n" << usagetxt2 << endl;
        return 0;
        }
    
    //create a cdl object    
    CLD cld;
    
    char *inputfile = argv[1];
    int ETF_kernel=    atoi(argv[2]);
	int ETF_iteration= atoi(argv[3]);
    char *outputgradient = argv[4];
    char *outputtangent  = argv[5];
    
    //read source image
    //char inputname[200];
    //sprintf(inputname,"%s.%s",inputfile,inputext);
    cld.readSrc(inputfile);
    
    //etf
    
    for(int i=0; i<ETF_iteration; i++) 
        {
        cout << "\rETF  " << i+1 << "/" << ETF_iteration << flush;
        cld.etf.refine_ETF(ETF_kernel);
        }
    cout << endl;
    
    //preview 
    if (withpreview)
        {
        PP processing(cv::Size(cld.originalImg.cols, cld.originalImg.rows));
        cv::Mat preview = cld.originalImg.clone();
        cv::cvtColor(preview, preview, CV_GRAY2BGR);
        processing.ETF(cld.etf.flowField, preview);
    
        //preview.convertTo(preview, CV_8U,255,0);
        //also save the Line integral convolution in a jpg image
        //https://github.com/philogb/LIC would be nice with OLIC or 
        //https://blog.kitware.com/evenly-spaced-streamlines-2d/
        //vector<int> compression_params;
        //compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
        //compression_params.push_back(100);
        //cv::imwrite(previewname, preview,compression_params);
    
        cv::namedWindow("etf");
        cv::imshow("etf", preview);
        cv::waitKey(0);
        }
        
    //convert to RGB for outside compatibility
    //and write as .exr
    cv::cvtColor(cld.etf.gradientField, cld.etf.gradientField, CV_BGR2RGB);
    cv::imwrite(outputgradient, cld.etf.gradientField);
    cv::cvtColor(cld.etf.flowField, cld.etf.flowField, CV_BGR2RGB);
    cv::imwrite(outputtangent, cld.etf.flowField);
    
}
