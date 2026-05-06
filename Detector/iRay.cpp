#include "iRay.h"

#pragma comment(lib, "ws2_32.lib")  // Added to resolve fatal Error LNK1120 6 unresolved external  
#pragma comment(lib, "irprops.lib")  //  Added to resolve fatal Error LNK1120 6 unresolved external 

#include "DisplayProgressbar.h"
#include "Detector.h"
#include "iniParser.h"  
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <thread>  // cpp multithread; syntax :-> std::thread thread_object(callable) 28_06_2022
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <thread>

#include <timeapi.h>
#include <QDebug>


// ***** Now for C Thread  30/06/2022
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>  // Header file for sleep(). 
//#include <pthread.h>
// *****
int UserFrequency; // 
int hgrabIndex = 0;

ApplicatioMode mode;// = GetAppModeAttr();
IRayImage* GrabbedImage;

static IRayTimer s_timer;

int detState=1;

ofstream MyFile("D://iRayFileName.txt");

#include <utility> // This header file contains : pairs : pair<T,T>pairName
                   //                           : Generic Relational Approcah ( !=, > , = under specific namespace: rel_ops)
                   //                           : Generic swap function (This as standard definition used by default by components of the standard library for all types that do not provide their own overload : swap)
                   // Here I addeded to return a pair of Image count and corresponding Image Buffer : Raviranjan K.

pair<int, unsigned short*> Img_Bufcount_Pair;  // It will return the Image count and respective Image count in a pair 

//vector<pair<int, unsigned short*>> Img_Bufcount_Vector;

#define TRACE printf  // #define 

static CDetector* gs_pDetInstance = NULL;
static int s_nAcquireTime = 5000;

std::string savePath; // = "D:\\tempfile\\"; // save path for now In machine schenerio will get path from user side, here if grabbe without giving path then it would be save in programe file

static std::vector<ApplicatioMode> s_appmode;

int hgrabstatus = 1;
int grabindex = 0;

int countllll = -1; // for image count to grab (named as countllll because count is allready a variable in the programme for ohter purpose )
int countRad = -1;

unsigned short * radbuffer; // Test that Global Variable we can access or not from Visual Studio code to qt code :

static HEVENT s_hNextStep = NULL;
static HEVENT s_hErrorEvent = NULL;
bool WaitForMultObjects(HEVENT *hEvents);


/*
 This DLL will use for RRPune, RRDelhi and BHU
*/

unsigned short *fluorobuffer;

bool syncInstatus = false;

unsigned short * pImageData_buffer;
unsigned short * ExternBuffer;
unsigned short * buffer;

int Imagegrabcnt;
int totaliteration = 0;



void StartAcquisition(int i) // function to Acquire the Image  19/05/2022
{
	countllll = -1;
	//bufferCount = &countllll;
	countRad = -1;

	gs_pDetInstance->SyncInvoke(Cmd_StartAcq,10000);  // 28/06/2022 comment and added gloabl function void StartAcq(int In) 	

	//thread thrStartAcqcpp(StartAcqcpp, 1);  // cpp thread initiated 28/06/2022
	//thrStartAcqcpp.join(); // wait till the end of the operations 28/06/2022

}


DWORD WINAPI CThread(LPVOID lpParam)
{
//void CThread() // CThread function (Global function) 30/06/2022  
//{
	/* Here will call the Acquisition function that that is call SDKCallCalbackHandler inside that's actuall 
	 * responsible for Image Grabbing 
	 * then This will call into the crate thread in ------- function i.e actually called by the QT programe
	*/

	//MyFile << " *Enter into CThread (StartAcquisition() function called here)* " << endl; // 11/08

	StartAcquisition(1); // start Acquisition 30/06/2022


	return 0;

}

DWORD WINAPI radThread(LPVOID lpParam)
{
	while (true)
	{
		AttrResult attr;
		gs_pDetInstance->GetAttr(Attr_State, attr);
		detState = attr.nVal;

		if (detState == 1 && countllll >= 0)
			break;
	}

	return 0;

}

iRay::iRay(QObject *parent)
    : QObject(parent)
{
	//fluorobuffer = new unsigned short[900 * 1536 * 1536]; // Taking buffer of maximum 50 Number of Images each of size 3072*3072

	pImageData_buffer = new unsigned short[ 1536 * 1536];
	ExternBuffer = new unsigned short[1536 * 1536 * 900];
	buffer = new unsigned short[1536 * 1536 * 900];

	//pImgBuffer = new unsigned short[3072*3072];

	/*GlobalVariableTest = new unsigned short[900 * 1536 * 1536];

	radbuffer = new unsigned short[3072 * 3072 * 100];*/

	//GlobalVariableTest = buffer;
	//bufferCount = &countllll;


	//savePath = "D:\\tempfile\\proj_";
	 // constructor 
//	ConnectDetector(); //
	//ModeSelection(); // 

}
//unsigned short* pImageData; // Image Buffer to return 

iRay::~iRay()
{
	// Destructor 
}

void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
	const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam)
{
	gs_pDetInstance->SDKCallback(nDetectorID, nEventID, nEventLevel, pszMsg, nParam1, nParam2, nPtrParamLen, pParam);
	switch (nEventID)
	{
	case Evt_Image:

	{

		//hgrabstatus = 1;
		//unsigned short* pData; // Image Data type and Image pointer -> Image Data type : unsigned short and Image pointer : pData
		//pair<int, unsigned short*> Img_Bufcount_Pair;  // It will return the Image count and respective Image count in a pair 

		TRACE("Got image\n");
		//must make deep copies of pParam
		IRayImage* pImg = (IRayImage*)pParam;

		unsigned short* pImageData = pImg->pData;
		pImageData = pImg->pData;
		int nImageSize = pImg->nWidth * pImg->nHeight * pImg->nBytesPerPixel;
		int nFrameNo = gs_pDetInstance->GetImagePropertyInt(&pImg->propList, Enm_ImageTag_FrameNo);

		std::cout << "Image Pixel Value: " << pImageData[pImg->nWidth * pImg->nHeight / 4] << "Frame Number" << nFrameNo << "   " << countllll;
		countllll++; // increment by 1 : This will count the number of image in real time   // comment for a while 
		//countllll = nFrameNo;

		if (mode.Freq != 1)
			memcpy(buffer + countllll, pImageData, sizeof(unsigned short) * 1536 * 1536);

		countRad = 0;

		if (hgrabstatus == 0)
		{
			memcpy(radbuffer, pImageData, sizeof(unsigned short) * 3072 * 3072);
			//GrabbedImage = pImg;
		}

		if (hgrabstatus == 1)
		{
            const std::string s1 = savePath + std::to_string(countllll) + ".raw";   // naming image according to number of grabbed image in particular setups
			const char  *s2 = s1.c_str();
			FILE *fp;
			fopen_s(&fp, s2, "wb+");
			fwrite(pImageData, sizeof(unsigned short), pImg->nWidth * pImg->nHeight, fp);

			fclose(fp);

		}
        qDebug()<<"image Acquired";


	//	hgrabstatus = 1;

		break;

	}

	//case Evt_WaitImage_Timeout:
	//{
	//	syncInstatus = false;
	//	break;
	//}
	//case Evt_Exp_Timeout:
	//{
	//	syncInstatus = false;
	//	break;
	//}

	default:
		//AttrResult attr;
		//gs_pDetInstance->GetAttr(Attr_State, attr);

		//detState = 1;
		break;
	}
}




bool iRay::connectDetector() // Connect Detector
{
	TRACE("ConnectDetector function\n");
	int ret = Initializte();  // create instance, create object, connect Detector 
	if (ret == 0)
	{
		//MyFile << "	**ConnectDetector() function called return true** " << endl;

		return true;

	}
	

	else
	{
		//MyFile << "	**ConnectDetector() function called return false from else condition** " << endl;
		return false;


	}

		
}

int iRay::OffsetCalibration()  // Offset Calibration Template Generate and Implement to the Detector 19/05/2022
{

	int ret1 = gs_pDetInstance->SyncInvoke(Cmd_OffsetGeneration, 30000 /*5000*/);  // Offset Calibration Template Generated

	if (Err_OK != ret1)
	{
		TRACE("\t\t[No ]\n");
	}
	else
	{
		TRACE("\t\t[Yes]\n");
		TRACE("Offset Calibration Generated Successfully");
	}

	int ret = gs_pDetInstance->SyncInvoke(Cmd_SetCorrectOption, Enm_CorrectOp_SW_PreOffset /*| Enm_CorrectOp_SW_Gain | Enm_CorrectOp_SW_Defect */, 5000);  // Offset Calibration Result applied
	if (Err_OK != ret)
	{
		TRACE("\t\t[No ]\n");
		return ret;
	}
	else
		TRACE("\t\t[Yes]\n");
	return ret; // added 19/05/2022

}

//void StartAcqcpp(int In) // It is global function // just argument to use in cpp thread : 28/06/2022
//{
//  // will use in the cpp thread 
// // Note: To use cpp thread we need to make a global function of void data type and then use that function in cpp thread : synatx -> thread thread_Object(function_name, function_argument) then use thread_Object.join() 
//	static CDetector *gs_pDetInstance1 = NULL;
//	gs_pDetInstance1 = new CDetector();
//	gs_pDetInstance1->Invoke(Cmd_StartAcq);
//	gs_pDetInstance = gs_pDetInstance1;  // we may use gs_pDetInstance in place of gs_pDetInstance1 no need to create gs_pDetInstance1 but for now using it then will use gs_pDetInstance only
//
//}



/*int iRay::StartAcquisition(int i) // function to Acquire the Image  19/05/2022
{
	countllll = -1;
	bufferCount = &countllll;
	countRad = -1;
	gs_pDetInstance->Invoke(Cmd_StartAcq);
	return 0;
}
*/

int iRay::ModeSelection(bool status)  // Mode Selection 
{
	TRACE("ModeSelection function\n");
	
	// code to select Mode option NEED TO WRITE HERE
    contribution_In_Mode_Selection_And_other_Operations(status, UserFrequency);
	//ParseApplicationModeInfo(); // Mode Selection from ini file as per selected option
	//GetAppModeAttr(); // Selected Mode Appllied on Detector system for further operation 
	//DisplayModeInfo(); // Display selected Mode in console for Developer purpuse 

	
	return 0;
}



void PullImageTimer(int uTimerID)
{
	int nFrameNumber = 0;
	int nImgSize = 0;
	int nPropSize = 0;
	int ret = gs_pDetInstance->QueryImageBuf(nFrameNumber, nImgSize, nPropSize);
	if (ret != Err_OK)
	{
		return;
	}
	int nFrameNo = 0;
	countllll++;
	TRACE("nimagesize %d\n", nImgSize);
	unsigned short* pImgBuffer = new unsigned short[nImgSize / sizeof(short)];
	gs_pDetInstance->GetImageFromBuf(pImgBuffer, nImgSize, nPropSize, nFrameNo);
	TRACE("Pull image, frameNo:%d,%d, %d\n", nFrameNumber, nFrameNo,nImgSize);
	TRACE("countllll image, %d\n", countllll);

	if(grabindex==1)
	{
//		savePath = "D:\\tempfile\\proj_";
		const std::string s1 = savePath + std::to_string(countllll) + ".raw";
		const char  *s2 = s1.c_str();
		FILE *fp;
		fopen_s(&fp, s2, "wb+");

		fwrite(pImgBuffer, sizeof(unsigned short), 3072*3072, fp);

		fclose(fp);


	}

	if (grabindex == 2)
	{
//		savePath = "D:\\tempfile\\proj_";
		const std::string s1 = savePath + std::to_string(countllll) + ".raw";
		const char  *s2 = s1.c_str();
		FILE *fp;
		fopen_s(&fp, s2, "wb+");

		fwrite(pImgBuffer, sizeof(unsigned short), 1536 * 1536, fp);

		fclose(fp);


	}



}


void iRay::imageGrabbing(const QString &path, bool flag) // Image Grabbing
{
    qDebug()<<"imageGrabbing"<<path;

	ofstream MyFile("iRayImagin_check.txt");
	TRACE("Image acquire fucntion\n");

	DWORD mythreadid;  // 
	HANDLE thread_h[1]; // thread_h[N_THREADS]

    savePath = path.toStdString();
	
	//WaitForMultipleObjects(1, thread_h, TRUE, INFINITE);  // arg: (N_THREADS, thread_h, TRUE, INFINITE)
	//close Handler (thread Handler)
	//CloseHandle(thread_h);

	//static HEVENT s_hNextStep = NULL;
	//static HEVENT s_hErrorEvent = NULL;

	s_hNextStep = CreateEvent(NULL, false, false, NULL);
	s_hErrorEvent = CreateEvent(NULL, false, false, NULL);
	HEVENT hEvents[2] = { s_hNextStep, s_hErrorEvent }; 




	gs_pDetInstance->SyncInvoke(Cmd_ClearAcq, 2000);  // 28/06/2022 comment and added gloabl function void StartAcq(int In) 	

	//gs_pDetInstance->SyncInvoke(Cmd_StartAcq, 2000);

	if (!WaitForMultObjects(hEvents))
	{
		MyFile << "Continue::" << endl;
	}


	//Sleep(7000);




	//StartAcquisition(1);
	//TRACE("state %p", hEvents[0]);  // Print the first event handle
	//TRACE("state %p", hEvents[1]);  // Print the second event handle
	 
	TRACE("state %s", hEvents);
	TRACE("\n");
	TRACE("ImageGrabbing fucntion\n");


}


ApplicatioMode GetAppModeAttr()  // ** Add for Mode Selection 17/01/2022  
{
	//Attr_CurrentCorrectOption, Attr_UROM_PGA, Attr_UROM_BinningMode, Attr_UROM_ZoomMode, Attr_UROM_SequenceIntervalTime
	ApplicatioMode mode = { 0 };
	AttrResult attr;
	gs_pDetInstance->GetAttr(Attr_UROM_PGA, attr);
	mode.PGA = attr.nVal;
	gs_pDetInstance->GetAttr(Attr_UROM_BinningMode, attr);
	mode.Binning = attr.nVal;
	gs_pDetInstance->GetAttr(Attr_UROM_ZoomMode, attr);
	mode.Zoom = attr.nVal;
	gs_pDetInstance->GetAttr(Attr_UROM_SequenceIntervalTime, attr);

	mode.Freq = attr.nVal;
	mode.Freq = (attr.nVal == 0) ? 0 : 1000.0 / attr.nVal;

	return mode;
}


bool WaitForMultObjects(HEVENT *hEvents)
{
	ofstream MyFile("iRayImagin_Grabbing.txt");
	iRay instance;
	TRACE(" WaitForMultObjects to entered\n");
	int wait = WaitForMultipleObjects(2, hEvents, false, 7000);  //WAIT_FOREVER
	MyFile << "value of wait::" << wait << endl;

	if (WAIT_OBJECT_0 + 1 == wait)
	{
		MyFile << "Entering Wait_Object_0 + 1 ; returning false" << endl;
		TRACE("Press [Enter] to restart\n");
		getchar();
		return false;
	}

	MyFile << "Not entering if loop returning true" << endl;
	return true;
}

int iRay::ConnectGrab()
{
	int ret = Initializte();
	if (ret == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}

//return 0;
}
void iRay::ImageGrabbingRAD(string SavePath, bool grabstatus) // Image Grabbing  rad 
{
//	savePath = SavePath;

	hgrabstatus = grabstatus;

	//DWORD mythreadid;  // 
	//HANDLE thread_h[1]; // thread_h[N_THREADS]



//	savePath = SavePath;

	//// Thread create
	//thread_h[0] = CreateThread(NULL, 0, radThread, 0, 0, 0);  // crated thread 
	//// wait for the thread to finish
	//WaitForMultipleObjects(1, thread_h, TRUE, INFINITE);  // arg: (N_THREADS, thread_h, TRUE, INFINITE)
	//// close Handler (thread Handler)
	//CloseHandle(thread_h);

	//OffsetCalibration(); // call offsetcalibration 19/05/2022
	//gs_pDetInstance->Invoke(Cmd_StartAcq);  // 19/05/2022

	StartAcquisition(0);  // 19/05/2022

	MyFile << "	Entering Start  Acquisition" << endl;

	/*Sleep(200);

	while (hgrabstatus == 0)
	{

	}*/

	//do
	//{
	//	//OffsetCalibration(); // offset calibration template generate and set to the detector  19/05/2022
	//	//StartAcquisition();  // 19/05/2022
	//	gs_pDetInstance->Invoke(Cmd_StartAcq); // commented 
	//	
	//} while (hgrabstatus == true);

	TRACE("ImageGrabbing fucntion\n");

	//	gs_pDetInstance->Invoke(Cmd_StartAcq);

	//	void SDKCallbackHandler(int nDetectorID, int nEventID, int nEventLevel,
	//	const char* pszMsg, int nParam1, int nParam2, int nPtrParamLen, void* pParam);

	//return pImageData;
	//return 0;
}

int iRay::StopGrabbing() // Stop Image Grabbing 
{
	TRACE("\n");
	TRACE("StopGrabbing function\n");
	hgrabstatus = 1;
	
	//if (hgrabIndex == 1 && countRad == 0)
	//{
	//	const std::string sRad = savePath + std::to_string(0) + ".raw";
	//	TRACE("Save Path ", sRad);

	//	FILE *fp;
	//	fopen_s(&fp, sRad.c_str(), "wb+");

	//	fwrite(GrabbedImage->pData, sizeof(unsigned short), GrabbedImage->nWidth * GrabbedImage->nHeight, fp);

	//	fclose(fp);

	//}


	gs_pDetInstance->SyncInvoke(Cmd_StopAcq, 2000);
	return 0;

}


unsigned short* iRay::Imagebuffer()
{

	pImageData_buffer = new unsigned short[1536 * 1536];

	return ExternBuffer;

}

//unsigned short * iRay::dummy_image()
//{
//
//	for (int i = 0; i < 52; ++i) {
//		// Construct the file name using standard C++ string manipulation
//		std::string fileName = "D:\\tempfile\\tempfile\\proj_" + std::to_string(i) + ".raw";
//
//		FILE *fp = nullptr;
//		if (fopen_s(&fp, fileName.c_str(), "rb") == 0 && fp) { // Check if file opened successfully
//			size_t bufferOffset = i * (1536 * 1536); // Calculate the buffer offset
//			fread(ExternBuffer + bufferOffset, sizeof(unsigned short), 1536 * 1536, fp);
//			fclose(fp); // Close the file
//		}
//		else {
//			std::cerr << "Failed to open file: " << fileName << std::endl;
//		}
//	}
//
//	return ExternBuffer;
//
//
//}


int iRay::ImageGrabcount()
{

	return Imagegrabcnt;
}

int iRay::userSync(bool SyncStatus) // User Sync 19/05/2022
{

	if (SyncStatus == true) // External  Mode 
	{
		int ret;

		ret = gs_pDetInstance->SetAttr(Attr_UROM_FluroSync_W, Enm_FluroSync_SyncIn); // SyncIn Mode 
		ret = gs_pDetInstance->SetAttr(Attr_UROM_TriggerMode_W, Enm_TriggerMode_Prep); // Outer Mode

		if (Err_OK != ret)
		{
			TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
			return ret;
		}
		else
		{
			TRACE("\t\t\t TriggerModePrep Set[Yes]\n");
		}



		TRACE("External mode entered \n");
	}

	else // Internal Mode 
	{
		int ret;

		ret = gs_pDetInstance->SetAttr(Attr_UROM_FluroSync_W, Enm_FluroSync_SyncOut); // SyncOut Mode 
		ret = gs_pDetInstance->SetAttr(Attr_UROM_TriggerMode_W, Enm_TriggerMode_Outer); // Inner Mode 

		if (Err_OK != ret)
		{
			TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
		}
		else
		{
			TRACE("\t\t\t TriggerModeouter Set[Yes]\n");
		}
	}

	//gs_pDetInstance->SyncInvoke(Cmd_WriteUserRAM, 10000); // Write the Sync Mode to Detector 

	return 0;

}



int iRay::contribution_In_Mode_Selection_And_other_Operations(bool status, int index) // status : false -> internal mode , Status : true -> external mode
{
	TRACE("Contribution_In_Mode_Selection_And_other_Operations\n");
    qDebug()<<"Contribution_In_Mode_Selection_And_other_Operations";
	hgrabIndex = index;
	grabindex = index;

	countllll = -1;
	bufferCount = &countllll;

	if (hgrabIndex == 1)
		syncInstatus = true;

	DisplayModeInfo();
	userSync(status); // User Sync 19/05/2022

	//do
	//{




	return 0;


}

int iRay::Initializte()
{

	gs_pDetInstance = new CDetector();
	TRACE("Load libray");

	MyFile << "Constructor Initialized" << endl;

	int ret = gs_pDetInstance->LoadIRayLibrary();
	if (Err_OK != ret)
	{
		TRACE("\t\t\t[No ]\n");
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

	MyFile << "	int ret = gs_pDetInstance->LoadIRayLibrary()  " << endl;

	TRACE("Create instance");
     qDebug() << "Create instance , WorkDirPath:" << QString::fromStdString(GetWorkDirPath());
	ret = gs_pDetInstance->Create(GetWorkDirPath().c_str(), SDKCallbackHandler);
	if (Err_OK != ret)
	{
		MyFile << gs_pDetInstance->GetErrorInfo(ret).c_str() << endl;
		TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
        TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
        qDebug()<<"Create instance";
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

	MyFile << "	GetWorkDirPath" << endl;

	//TRACE("Create instance");

	TRACE("Connect device");
	ret = gs_pDetInstance->SyncInvoke(Cmd_Connect, 30000);
	if (Err_OK != ret)
	{
		TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
		return ret;
	}
	else
		TRACE("\t\t\t[Yes]\n");

//	TRACE("Connect device");
    TRACE("\t\t\t[No ] - error:%s\n", gs_pDetInstance->GetErrorInfo(ret).c_str());
    qDebug()<<"Connected device";

	MyFile << "	SyncInvoke" << endl;

	

	//return ret;
}


void iRay::Deinit()
{
	if (gs_pDetInstance)
	{
		gs_pDetInstance->Destroy();
		gs_pDetInstance->FreeIRayLibrary();
		delete gs_pDetInstance;
		gs_pDetInstance = NULL;
		
	}
}


void iRay::ParseApplicationModeInfo()  // ** Add for Mode Selection 
{
	char section[32] = { 0 };
    const char *path;
	// sprintf(path, "%s/%s", GetWorkDirPath().c_str(), "DynamicApplicationMode.ini");  
	int index = grabindex;


	// change path for DynamicApplicationMode.ini folder 
	//path = "../../work_dir/Mercu1717V/DynamicApplicationMode.ini";
	//path = "D:\\PMTRadiotherapy\\IdetectorConfigFiles\\work_dir\\Mercu1717V\\DynamicApplicationMode.ini"; // It's need to have PMTRadiotherapy folder in D drive with all the required files

	//path = "D:\\iRay_BackUp_09_03_2022\\Cpp - Copy\\work_dir\\Mercu1717V\\DynamicApplicationMode.ini";





	path = "D:\\dastagiri\\IRAY_DR_CARM\\New folder\\Tools\\iDetector\\x64\\work_dir\\Mars1717V3\\DynamicApplicationMode.ini";

	//do
	//{
		ApplicatioMode mode = { 0 };
		sprintf(section, "%s%d", "ApplicationMode", index);
		TRACE("\n");
		mode.Index = index;
		CIniParser ini;
		if (false == ini.ReadFile(path))
		{
			TRACE("read file failed:%s\n", path);
		//	break;
		}
		if (false == ini.GetItemValueI(section, "PGA", mode.PGA))
		{
		//	break;
		}



		ini.GetItemValueI(section, "Binning", mode.Binning);
		ini.GetItemValueI(section, "Zoom", mode.Zoom);
		ini.GetItemValueF(section, "Frequency", mode.Freq);




		std::string tmp_subset;
		ini.GetItemValueS(section, "subset", tmp_subset);
		memset(mode.subset, 0, 32);
		strncpy(mode.subset, tmp_subset.c_str(), 31);
		s_appmode.push_back(mode);

		TRACE("\n");
		TRACE("index_value %d", index);
		TRACE("\n");
		TRACE("Binning_parse %d", mode.Binning);
		TRACE("\n");
		TRACE("zoom_parse %d", mode.Zoom);
		TRACE("\n");
		TRACE("Frequency_parse  %.2f", mode.Freq);
		TRACE("\n");
		TRACE("PGA %d", mode.PGA);
		TRACE("\n");
		TRACE("section_parse %s", section);


		//++index;
	//} while (true);
}

void iRay::DisplayModeInfo()  // Add for Mode Selection 
{
	int result;
	int ret;
	AttrResult attr;
	string strGainSubset = "";

	ParseApplicationModeInfo();

	ApplicatioMode mode = GetAppModeAttr();

	TRACE("\n");
	TRACE("grabindex %d", grabindex);
	TRACE("\n");
	TRACE("Frequency above  %.2f", mode.Freq);
	TRACE("\n");

	if (grabindex == 1)
	{
		mode.PGA = 6;
		mode.Binning = 0;
		mode.Freq = 1;
		mode.Zoom = 0;
		TRACE("Updated Frequency for grabindex 1:  %.2f", mode.Freq);
	}
	if (grabindex == 2)
	{

		mode.PGA = 6;
		mode.Binning = 0;
		mode.Freq = 1;
		mode.Zoom = 0;
		TRACE("Updated Frequency for grabindex 1:  %.2f", mode.Freq);
	}

	TRACE("\n");
	TRACE("Binning %d", mode.Binning);
	TRACE("\n");
	TRACE("zoom %d", mode.Zoom);
	TRACE("\n");
	TRACE("Frequency  %.2f", mode.Freq);
	TRACE("\n");
	TRACE("PGA %d", mode.PGA);
	TRACE("\n");

	//TRACE("Current Detector-Mode: [ PGA:%d,AnaBin:%d,ExpWin:%d,AcqType:%d,Subset:%s ]\n", mode.PGA, mode.Binning, mode.Freq, mode.subset);
	if (grabindex == 1)
	{
		result = gs_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode4", 10000);
	}
	if (grabindex == 2)
	{
		result = gs_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode4", 10000);
	}

	if (Err_OK != result)
	{
		TRACE("\t\t SetCalisubset [No ]\n", gs_pDetInstance->GetErrorInfo(result).c_str());
		// break;
	}
	else
	{
		TRACE("\t\t[Yes]\n");
	}

	ret = gs_pDetInstance->SyncInvoke(Cmd_SetCorrectOption, Enm_TriggerMode_Inner | Enm_CorrectOp_SW_PostOffset | Enm_CorrectOp_SW_Gain | Enm_CorrectOp_SW_Defect, 10000);

	if (Err_OK != ret)
	{
		TRACE("\t\t  Set Correct option [No ]\n", gs_pDetInstance->GetErrorInfo(result).c_str());
	}
	else
	{
		TRACE("\t\t[Yes]\n");
	}
	
}



//----------------------------------------------------
void iRay::SwitchApplicationMode_HWGainDefectCorrection()  // Calibration 
{
	int ret = gs_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode4", 5000);

	if (Err_OK != ret)
		return;
	int file_index = 1;//defined by downloadCorrectionFile2Device
	ret = gs_pDetInstance->SyncInvoke(Cmd_SelectCaliFile, Enm_File_Gain, file_index, 3000);
	if (Err_OK != ret)
		return;
	ret = gs_pDetInstance->SyncInvoke(Cmd_SelectCaliFile, Enm_File_Defect, file_index, 3000);
	if (Err_OK != ret)
		return;
}
void iRay::SwitchApplicationMode_SWGainDefectCorrection()  // ** Add for Mode Selection 
{
	int ret = gs_pDetInstance->SyncInvoke(Cmd_SetCaliSubset, "Mode4", 5000);

	if (Err_OK != ret)
		return;
}
