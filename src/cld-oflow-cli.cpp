#include <cstdio>
#include <opencv2/opencv.hpp>

#include "CLD.h"
#include "postProcessing.h"

using namespace std;
using namespace cv;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    
    //minimal usage
    if (argc == 1)
        {
        string usagetxt ="Usage : cld-oflow-cli input_file_root input_file_ext input_etf_root input_oflow_root input_consistency_root FDoGiterations sigma_m sigma_c rho tau oflowblend output_file_root startframe endframe";
        string usagetxt1="  use precomputed backward optical flow and consistency maps to enhance the temporal coherence of the lines";
        string usagetxt2="  opticalflow files should be in the format : backward_0010_0009.exr and reliable_0010_0009.pgm (here for frame 10)";
        string usagetxt3="  oflowblend is used to reblend % of original output (should be 1.0 for full opticalflow)";
        string usagetxt4="  ex : CLD-oflow-cli path/to/input png path/to/etf path/to/backward path/to/reliable 3 5 1 .995 .999 1 path/to/output 1 50";
        //also save the Line integral convolution in a jpg image
        cout << usagetxt << "\n" << usagetxt1 << "\n" << usagetxt2 << "\n" << usagetxt3 << "\n" << usagetxt4 << endl;
        return 0;
        }
        
    //create a cdl object    
    CLD cld;
    
    char *input_file_root=  argv[1];
    char *input_file_ext=   argv[2];
    char *input_etf_root=   argv[3];
    char *input_oflow_root= argv[4];
    char *input_consistency_root= argv[5];
    char *output_file_root= argv[12];
    
	int FDoG_iteration=atoi(argv[6]);
    cld.sigma_m =  atof(argv[7]); //3.0
	cld.sigma_c =  atof(argv[8]); //1.0
	cld.rho =      atof(argv[9]); //0.98
	cld.tau =      atof(argv[10]); //0.98
    float oflowblend =   atof(argv[11]); //0.9
    int fstart= atoi(argv[13]);
    int fend=   atoi(argv[14]);
    
    //
    cv::Mat oflow,mapx,mapy,previous,warped,consistency,element;
    int kernel_size;
    //main loop
    for (int frame = fstart; frame <= fend; frame++) 
        {
        //dealing with filename
        char inputname[200];
        sprintf(inputname,"%s.%04d.%s",input_file_root,frame,input_file_ext);
        //cout << "inputfile : " << inputname << endl;
        char etfname[200];
        sprintf(etfname,"%s.%04d.exr",input_etf_root,frame);
        //cout << "etf : " << etfname << endl;
        char outputname[200];
        sprintf(outputname,"%s.%04d.%s",output_file_root,frame,input_file_ext);
        //cout << "output : " << outputname << endl;
        
        //read source image
        //do the etf once , should avoid that
        cld.readSrc(inputname);
        //read etf
        cld.etf.flowField = cv::imread(etfname,CV_LOAD_IMAGE_UNCHANGED);  
        if(! cld.etf.flowField.data )                              // Check for invalid input
            {
            cout <<  "Could not open or find a precomputed flowfield" << std::endl ;
            return -1;
            }
        
        //permut etf rgb to bgr
        cv::cvtColor(cld.etf.flowField, cld.etf.flowField, CV_RGB2BGR);
        //generate drawing
        cld.genCLD();
        
        //FDoG iterations
        for(int i=0; i<FDoG_iteration; i++) 
            {
            cout << "\rFDoG iteration  " << i+1 << "/" << FDoG_iteration << flush;
            cld.combineImage();
            cld.genCLD();
            }
            cout << endl;
        
        //use optical flow (skip first frame)
        if (frame > fstart)
        {
            //read oflow
            char oflowname[200];
            sprintf(oflowname,"%s_%04d_%04d.exr",input_oflow_root,frame,frame-1);
            //cout << "oflow : " << oflowname << endl;
            oflow = cv::imread(oflowname, IMREAD_UNCHANGED);  
            if(! oflow.data )                              // Check for invalid input
            {
                cout <<  "Could not open or find a precomputed optical flow field" << std::endl ;
                return -1;
            }
            //read consistency
            char consistencyname[200];
            sprintf(consistencyname,"%s_%04d_%04d.pgm",input_consistency_root,frame,frame-1);
            //cout << "consistency : " << consistencyname << endl;
            consistency = cv::imread(consistencyname, CV_LOAD_IMAGE_GRAYSCALE); 
            //bad idea ... GaussianBlur(consistency, consistency, Size(5, 5), 0, 0);
            if(! consistency.data )                              // Check for invalid input
            {
                cout <<  "Could not open or find a consistency descriptor" << std::endl ;
                return -1;
            }
            
            //read previous frame
            char previousname[200];
            sprintf(previousname,"%s.%04d.%s",output_file_root,frame-1,input_file_ext);
            //cout << "previousresult : " << previousname << endl;
            previous = cv::imread(previousname,CV_LOAD_IMAGE_GRAYSCALE); 
            
            //warp with optical flow
            warped.create( oflow.size(), CV_8UC1);
            mapx = cv::Mat::zeros(oflow.size(), CV_32FC1);
            mapy = cv::Mat::zeros(oflow.size(), CV_32FC1);
            for (int y = 0; y < oflow.rows; ++y)
                {
                for (int x = 0; x < oflow.cols; ++x)
                    {
                    Vec3f of = oflow.at<Vec3f>(y, x);
                    //cout <<  x << " " << y << "pixel : " << of  << std::endl ;
                    mapx.at<float>(y, x) = x + of.val[2];
                    mapy.at<float>(y, x) = y + of.val[1];
                    }
                }
            cv::remap(previous, warped, mapx, mapy, INTER_CUBIC , BORDER_CONSTANT, Scalar(255, 255, 255) );
            
            //inject warped into result
            cld.combineResultOflow(warped,consistency,oflowblend);
        }

        cout << "--> processed : frame " << frame << endl;
        //bad idea ... despeckle
        //kernel_size=0;
        //element = getStructuringElement( MORPH_RECT, Size( 2*kernel_size + 1, 2*kernel_size+1 ),Point( -1, -1 ) );
        //cv::morphologyEx(cld.result, cld.result, MORPH_CLOSE, element);
        cv::imwrite(outputname, cld.result);
        //preview
        bool withpreview=0;
        if (withpreview)
            {
            cv::namedWindow("cld");
            cv::imshow("cld", cld.result);
            cv::waitKey(0);
            }
    }
}
