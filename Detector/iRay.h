#pragma once

#include <vector>
#include <algorithm>
#include <string.h>
//#include <window.h>

using namespace std;

//struct ApplicatioMode  // ** 17/01/2022 for Mode Selection
//{
//	int Index;   // Index form 1 to 6 for now
//	int PGA;    // PGA1, PGA2, PGA3, PGA4, PGA5, PGA6
//	int Binning; // 0 - No Binning, 1x1 , 2x2 etc
//	int Zoom;   // 0 - No zoom, 1, 2 etc
//	double Freq;  // Frame Rate Per Second : 1, 2 etc
//	char subset[32];  // Mode1, Mode2, Mode3, Mode4 etc
	
//};

//class iRay
//{
//public:

//	iRay();  // Default Constructor

//	//void CThread(); // CThread function 30/06/2022

//	int Initializte();
//	void Deinit();
	
//	int ConnectGrab();


//	void ParseApplicationModeInfo();  // for Mode Selection
//	void DisplayModeInfo();    // for Mode Selection
//	//ApplicatioMode GetAppModeAttr();  // for mode selection

//	// static CDetector* gs_pDetInstance = NULL;
//	  // for mode selection
//	void SwitchApplicationMode_HWGainDefectCorrection();  // for mode selection
//	void SwitchApplicationMode_SWGainDefectCorrection();  // for mode selection

//	bool ConnectDetector();  // Additional function to Connect Detector
//	int ModeSelection(bool status); // Additional function to Select Modet
//	//unsigned short* ImageGrabbing(string SavePath, bool grabstatus); // Additionl function to Grab Image 09/05/2022
//	int StopGrabbing(); // Additional function to Stop Image Grabbing
//	//int StartAcquisition(int i); // Start Image Acquisition
//	//void StartAcquisition(int i); // Start Image Acquisition

//	int userSync(bool SyncStatus); // user Sync 19/05/2022
//	int Contribution_In_Mode_Selection_And_other_Operations(bool status, int index);  // perform some operation to take input of mode seletion
//	                                                            //and further that will set to Detector and also included
//	                                                            // some other functionality
	                                          
//	int OffsetCalibration(); // It will Generate offset Template and set to detector
//	void ImageGrabbingRAD(string SavePath, bool grabstatus);// Image Grabbing  RAD
//	void ImageGrabbing(string SavePath, bool grabstatus); // Image Grabbing  09/05/2022

//	//unsigned short * ImageGrabbing();

//	unsigned short * GlobalVariableTest; // Test that Global Variable we can access or not from Visual Studio code to qt code :
//	int *bufferCount=0;

//	unsigned short * Imagebuffer();
//	//unsigned short * dummy_image();
//	int ImageGrabcount();
//	int buffer_write_check = 0;

//	int dummy_value = 0;
//	volatile int grabcount;

//	~iRay(); // Default Destructor

//};

#pragma once

#include <QObject>

struct ApplicatioMode
{
    int Index;
    int PGA;
    int Binning;
    int Zoom;
    double Freq;
    char subset[32];
};

class iRay : public QObject
{
    Q_OBJECT

public:
    explicit iRay(QObject *parent = nullptr);  // Constructor with QObject parent
    ~iRay();

    //void CThread(); // CThread function 30/06/2022

    int Initializte();
    void Deinit();

    int ConnectGrab();


    void ParseApplicationModeInfo();  // for Mode Selection
    void DisplayModeInfo();    // for Mode Selection
    //ApplicatioMode GetAppModeAttr();  // for mode selection

    // static CDetector* gs_pDetInstance = NULL;
    // for mode selection
    void SwitchApplicationMode_HWGainDefectCorrection();  // for mode selection
    void SwitchApplicationMode_SWGainDefectCorrection();  // for mode selection

    Q_INVOKABLE bool connectDetector();  // Additional function to Connect Detector
    int ModeSelection(bool status); // Additional function to Select Modet
    //unsigned short* ImageGrabbing(string SavePath, bool grabstatus); // Additionl function to Grab Image 09/05/2022
    int StopGrabbing(); // Additional function to Stop Image Grabbing
    //int StartAcquisition(int i); // Start Image Acquisition
    //void StartAcquisition(int i); // Start Image Acquisition

    int userSync(bool SyncStatus); // user Sync 19/05/2022
//    int contribution_In_Mode_Selection_And_other_Operations(bool status, int index);  // perform some operation to take input of mode seletion
    //and further that will set to Detector and also included
    // some other functionality

    int OffsetCalibration(); // It will Generate offset Template and set to detector
    void ImageGrabbingRAD(string SavePath, bool grabstatus);// Image Grabbing  RAD
//    void imageGrabbing(string SavePath, bool grabstatus); // Image Grabbing  09/05/2022

    Q_INVOKABLE int contribution_In_Mode_Selection_And_other_Operations(bool flag, int number);

    Q_INVOKABLE void imageGrabbing(const QString &path, bool flag);

    //unsigned short * ImageGrabbing();

    unsigned short * GlobalVariableTest; // Test that Global Variable we can access or not from Visual Studio code to qt code :
    int *bufferCount=0;

    unsigned short * Imagebuffer();
    //unsigned short * dummy_image();
    int ImageGrabcount();
    int buffer_write_check = 0;

    int dummy_value = 0;
    volatile int grabcount;


};
