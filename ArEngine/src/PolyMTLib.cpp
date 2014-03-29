#include "PolyMTLib.h"

#define DEFAULT_SHOW_DEBUG_IMAGES false
#define DEFAULT_CAMERA_ROTATION 0
#define DEFAULT_DISPOSE_UNSEEN_AFTER 2
#define DEFAULT_MAX_MARKER_HISTORY_LENGTH 5

/// jakiej wielkosci minimalnie musi byc kontur, aby by³ brany pod uwagê
#define MINIMAL_CONTOUR_SIZE 60
#define CANONICAL_MARKER_SIZE 50

PolyMTLib::PolyMTLib()
{	
	markersSync = CreateMutex(NULL, false, L"Markers mutex");

	showDebugImages = DEFAULT_SHOW_DEBUG_IMAGES;
	cameraRotation = DEFAULT_CAMERA_ROTATION;	
	disposeUnseenMarkerAfter = DEFAULT_DISPOSE_UNSEEN_AFTER;
	maxMakerHistoryLength = DEFAULT_MAX_MARKER_HISTORY_LENGTH;
}

void PolyMTLib::Reinit(int deviceID, int width, int height, int FPS)
{
	Stop();
	vi_.release();
	delete image_;	

	Init(deviceID,width,height,FPS);

	Start();
}

void PolyMTLib::Init(int deviceID, int width, int height, int FPS)
{
	// inicjacja zmiennych
	deviceID_= deviceID;
	active_ = false;	
	fps_ = 1000/FPS; // do pêtli czasu rzeczywistego
	
	vi_.open(deviceID);        
	vi_.set(CV_CAP_PROP_FPS, FPS);
	vi_.set(CV_CAP_PROP_FRAME_WIDTH, width);	
	vi_.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	

	width_ = static_cast<int>(vi_.get(CV_CAP_PROP_FRAME_WIDTH));
	height_ = static_cast<int>(vi_.get(CV_CAP_PROP_FRAME_HEIGHT));
	image_ = new cv::Mat(height_, width_, CV_8UC3);	

	nextMarkerID = 0;
}

float PolyMTLib::computeRotation(cv::Point top, cv::Point center, float size)
{
	/// wykorzysuje iloczyn skalarny 2 wektorów do obliczania k¹ta, zwraca odchylenie od osi OY (zakres <-PI,PI>)
	cv::Point2d tmp = top - center;
	tmp *= 1.0/size;	
	float ret = static_cast<float>(acos(tmp.y/sqrt(tmp.x*tmp.x + tmp.y*tmp.y)));

	//ret =  ret<0.0?(ret + 2.0*M_PI):ret;

	return ret;
}

cv::Mat PolyMTLib::rotateImage()
{	
	cv::Point2f src_center(width_/2.0f, height_/2.0f);
	cv::Mat rot_mat = getRotationMatrix2D(src_center, cameraRotation, 1.0);
	cv::Mat dst;

	warpAffine(*image_, dst, rot_mat, image_->size());
	return dst;
}

void PolyMTLib::Start()
{
	// Zapobieranie kilkutrotnemu rozpoczêciu rozpoznawania
	if(active_)
		return;

	active_ = true;	

	DWORD ThreadID;

	CreateThread(NULL, 0, PolyMTLib::startLoop, (LPVOID)this, 0, &ThreadID);

	mainLoopThreadEventFinishedHandle_ = CreateEvent(NULL, false, false, L"mainLoop");
}

DWORD WINAPI PolyMTLib::startLoop(LPVOID param)
{
	((PolyMTLib*)param)->mainLoopAr();

	return 0;
}

int PolyMTLib::checkParity(short digits)
{
	int output = 1;
	for (int i=0; i<9; i++)
		output ^= (digits >> i) & 1;
	return output;
} 

int PolyMTLib::HammingCheck(short digits)
{
	return checkParity(digits & 341) && checkParity(digits & 102) && checkParity(digits & 120) && checkParity(digits & 384); 
}

int PolyMTLib::getMarkerID(cv::Mat markerMat, Marker* markerOut)
{
	int blockSize = markerMat.rows/7;
	int blockSizeRoot = blockSize*blockSize;//kwadrat wielkoœci (dodane podczas optymalizacji)
	cv::Mat square; // pomocnicza macierz

	// wykrywanie obwódki markera
	for(int x=0; x<7;x++)
		for(int y=0;y<7;y+=(x==0 || x== 6)?1:6)
		{
			square=markerMat(cv::Rect(x*blockSize,y*blockSize,blockSize,blockSize));
			if(countNonZero(square) > (blockSizeRoot) * 0.75)
				return -1;
		}	

	// marker ma pe³n¹ obwódkê, jedziemy dalej

	markerOut->rotation = -1; // ustawione w celu sprawdzenia, czy zostanie nadany jakikolwiek kierunek markerowy

	// wykrywanie bloków wykrywaj¹cych rotacjê
	for(int x=1; x<6;x++)
		for(int y=1;y<6;y+=(x==1 || x== 5)?1:4)
	{
		
		square=markerMat(cv::Rect(x*blockSize,y*blockSize,blockSize,blockSize));
		if(countNonZero(square) < (blockSizeRoot) >> 1)
		{
			if(markerOut->rotation >= 0) // wykryto wiêcej ni¿ 1 bit okreœlaj¹cy rotacjê
			{
				return -2;
			}

			if(x == 3 && y == 1) // góra
			{				
				markerOut->rotation = 0;
			}
			else if(x == 5 && y == 3) // prawo
			{				
				markerOut->rotation = M_PI_2;
			}
			else if(x == 3 && y == 5) // dó³
			{				
				markerOut->rotation = M_PI;
			}
			else if(x == 1 && y == 3) // lewo
			{				
				markerOut->rotation = M_PI_32;
			}
			else
			{
				// wykryto b³êdny blok okreœlaj¹cy rotacjê
				return -3;
			}
		}
		}	

	// nie wykryto ¿adnego bloku okreœlaj¹cego rotacjê
	if(markerOut->rotation<0)
		return -4;	

	short id = 0; // binarnie  000000000 :)
	short idIterator = 256;//binarnie 100000000

	if(markerOut->rotation == 0) //góra
	{	
		for(int y=2;y<5;y++)
			for(int x=2;x<5;x++)			
			{
				square=markerMat(cv::Rect(x*blockSize,y*blockSize,blockSize,blockSize));
				if(countNonZero(square) < (blockSize*blockSize) >> 1)
					id = id | idIterator;
				idIterator>>=1; // przesuñ jedynke w prawo
			}
	}
	else if(markerOut->rotation == M_PI_2) //prawo
	{
		for(int x=4;x>=2;x--)
			for(int y=2;y<5;y++)
			{
				square=markerMat(cv::Rect(x*blockSize,y*blockSize,blockSize,blockSize));
				if(countNonZero(square) < (blockSize*blockSize) >> 1)
					id = id | idIterator;
				idIterator>>=1; // przesuñ jedynke w prawo
			}
	}
	else if(markerOut->rotation == M_PI) //dó³
	{
		for(int y=4;y>=2;y--)
			for(int x=4;x>=2;x--)
			{
				square=markerMat(cv::Rect(x*blockSize,y*blockSize,blockSize,blockSize));
				if(countNonZero(square) < (blockSize*blockSize) >> 1)
					id = id | idIterator;
				idIterator>>=1; // przesuñ jedynke w prawo
			}
	}
	else if(markerOut->rotation == M_PI_32) //lewo
	{
		for(int x=2;x<5;x++)
			for(int y=4;y>=2;y--)
			{
				square=markerMat(cv::Rect(x*blockSize,y*blockSize,blockSize,blockSize));
				if(countNonZero(square) < (blockSize*blockSize) >> 1)
					id = id | idIterator;
				idIterator>>=1; // przesuñ jedynke w prawo
			}
	}	

	if(!HammingCheck(id))
		return -5;

	markerOut->id = (id>>4 & 16) + (id>>3 & 14) + (id>>2 & 1);

	if(markerOut->id == 0)
		return -6;
	/// nie uzywamy markera o ID = 0, bo jest to ca³y bialy marker

	return 0;
}

void PolyMTLib::mainLoopAr()
{	
	vector<Marker> tmpMarkers;
	cv::Mat frame = cv::Mat(height_,width_, CV_8UC1);
	cv::Mat tresh = cv::Mat(height_,width_, CV_8UC1);
	vector<cv::Point> approxCurve;

	cv::Scalar redColor = cv::Scalar(0,0,255);
	cv::Scalar greenColor = cv::Scalar(0,255,0);

	cv::Point2f warpMat[4];
	warpMat[0] = cv::Point(0,0);
	warpMat[1] = cv::Point(CANONICAL_MARKER_SIZE-1,0);
	warpMat[2] = cv::Point(CANONICAL_MARKER_SIZE-1,CANONICAL_MARKER_SIZE-1);
	warpMat[3] = cv::Point(0,CANONICAL_MARKER_SIZE-1);

	cv::Point2f warpMatsource[4];
	cv::Mat canonicalMarker = cv::Mat(CANONICAL_MARKER_SIZE,CANONICAL_MARKER_SIZE, CV_8UC1);

	vector<vector<cv::Point>> contours;		
	cv::Mat kernel = (cv::Mat_<char>(3,3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);

	while(active_)
	{      		
		vi_ >> *image_;

		if(cameraRotation)
		{
			*image_ = rotateImage();
		}

		tmpMarkers.clear();		

		// przetwarzaj jedynie zielony kolor			
		cv::cvtColor(*image_,frame,CV_BGR2GRAY);			

		// proguj
		cv::adaptiveThreshold(frame, tresh, 255, cv::ADAPTIVE_THRESH_MEAN_C,cv::THRESH_BINARY_INV,7, 5);		

		cv::erode(tresh, tresh, cv::Mat(2,2,CV_8UC1));
		cv::dilate(tresh, tresh, cv::Mat(2,2,CV_8UC1));

		//cv::imshow("tresh", tresh);

		//char buffer[255];

		// znajdŸ kontury
		cv::findContours(tresh, contours, CV_RETR_EXTERNAL , CV_CHAIN_APPROX_NONE);		
		int markersnr = 0;
		int size = contours.size();
		for(int i=0; i<size; i++)
			{
				if(contours[i].size() > MINIMAL_CONTOUR_SIZE)
				{
					approxPolyDP(cv::Mat(contours[i]), approxCurve , float(contours[i].size())*0.05 , true);
					if(approxCurve.size()==4 && cv::isContourConvex(cv::Mat(approxCurve)))
					{
						// sort
						int dx1 = approxCurve[1].x - approxCurve[0].x;
						int dy1 =  approxCurve[1].y - approxCurve[0].y;
						int dx2 = approxCurve[2].x - approxCurve[0].x;
						int dy2 = approxCurve[2].y - approxCurve[0].y;
						int o = (dx1*dy2)-(dy1*dx2);

						if (o  < 0)
						{
							swap(approxCurve[1],approxCurve[3]);
						}

						

						for(int j=0;j<4;j++)
							warpMatsource[j] = approxCurve[j];
						cv::Mat M=getPerspectiveTransform(warpMatsource,warpMat);
						
						cv::warpPerspective(frame, canonicalMarker,  M, cv::Size(CANONICAL_MARKER_SIZE,CANONICAL_MARKER_SIZE));
						threshold(canonicalMarker, canonicalMarker,125, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);

						//cv::imshow("canon", canonicalMarker);

						// je¿eli b³¹d przy wykrywaniu markera, przejdŸ dalej
						Marker newMarker;

						int error = getMarkerID(canonicalMarker, &newMarker);
						// je¿eli b³¹d przy wyznaczaniu znacznika, wyjdŸ

						newMarker.size = sqrt(pow((float)(approxCurve[3].x - approxCurve[0].x) ,2) + pow((float)(approxCurve[3].y - approxCurve[0].y) ,2));
						newMarker.rotation += computeRotation(approxCurve[3], approxCurve[0], newMarker.size);

						cv::Point center = cv::Point(0,0);
						for(unsigned int j=0;j<4; j++)
						{			
							center.x += approxCurve[j].x;
							center.y += approxCurve[j].y;
						}
						center.x /= 4;
						center.y /= 4;

						newMarker.center = center;
						newMarker.LastVisible = time(NULL);						

						// kolorowanie markerów
						cv::line(*image_,approxCurve[0],approxCurve[1],error<0?redColor:greenColor, 2);
						cv::line(*image_,approxCurve[1],approxCurve[2],error<0?redColor:greenColor, 2);
						cv::line(*image_,approxCurve[2],approxCurve[3],error<0?redColor:greenColor, 2);
						cv::line(*image_,approxCurve[0],approxCurve[3],error<0?redColor:greenColor, 2);

						if(error  < 0)
						{
							// wyswietlenie numerów z b³êdem
							//sprintf(buffer,"%d",error);
							//cv::putText(*image_, buffer, center, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255,255,0));

							continue;
						}

						tmpMarkers.push_back(newMarker);
					}
				}
			}				

		WaitForSingleObject(markersSync, INFINITE);				
		
		// przypisywanie znalezionych markerów do juz istniej¹cych
		for(unsigned int i=0;i<tmpMarkers.size();i++)
		{
			bool found = false;
			for(unsigned int j=0;j<markers.size();j++)
			{
				/// znaleziono taki sam marker, podmieñ wspó³rzêdne
				if(markers[j].id == tmpMarkers[i].id)
				{
					markers[j].history.push_back(tmpMarkers[i]);

					if(markers[j].history.size() > maxMakerHistoryLength)
					{
						markers[j].history.pop_front();
					}

					cv::Point avgCenter = cv::Point(0,0);
					
					float avgSize = 0.0;
					/*float avgRotation;*/
					
					float sum1 = 0.0, sum2 = 0.0, k1 = 0.0, k2 = 0.0;					
					int count1 = 0, count2 = 0;										

					// uœrednienie
					for(list<Marker>::iterator it = markers[j].history.begin(); it != markers[j].history.end(); ++it)
					{						
						avgCenter += it->center;											
						avgSize += it->size;
						
						if(it->rotation < M_PI)
						{
							sum1 += it->rotation;
							count1++;
						}
						else
						{
							sum2 += it->rotation;
							count2++;
						}
					}

					if(count1>0) k1 = sum1/count1;
					if(count2>0) k2 = sum2/count2;

					if(count1>0 && count2>0)
					{
						if(k2-k1 >= M_PI)
						{
							markers[j].rotation  = ((sum1+sum2)-count2*2.0f*M_PI)/markers[j].history.size();
						}
						else
						{
							markers[j].rotation  = (sum1+sum2)/markers[j].history.size();
						}					
					}
					else if(count1>0)
					{
						markers[j].rotation = k1;
					}
					else
					{
						markers[j].rotation = k2;
					}

					markers[j].center = avgCenter * (1.0f / markers[j].history.size());					
					markers[j].size = avgSize * (1.0f / markers[j].history.size());												
					markers[j].LastVisible = tmpMarkers[i].LastVisible;
					found = true;
					break;
				}
			}

			/// nowy marker!
			if(!found)
			{
				markers.push_back(tmpMarkers[i]);
			}
		}

		// usuwanie markerów, który nie by³y dawno widoczne na scenie
		for(int i=markers.size()-1; i>=0; i--)
		{
			if(time(NULL) - markers[i].LastVisible > disposeUnseenMarkerAfter)
			{
				markers[i].history.clear();
				markers.erase(markers.begin()+i);
			}
		}

		ReleaseMutex(markersSync);	

		WaitForSingleObject(debugWindowsSync_, INFINITE);
		if(showDebugImages)
		{
			cv::imshow("video", *image_);
		}
		ReleaseMutex(debugWindowsSync_);

		if(cvWaitKey(fps_)==27) break;
	}

	SetEvent(mainLoopThreadEventFinishedHandle_);
}

cv::Size PolyMTLib::GetScreenSize()
{
	return cv::Size(width_, height_);
}

void PolyMTLib::ToggleDebugWindows()
{
	WaitForSingleObject(debugWindowsSync_, INFINITE);
	showDebugImages = showDebugImages?false:true;	

	if(!showDebugImages)
	{				
		cv::destroyWindow("video");
	}

	ReleaseMutex(debugWindowsSync_);
};
void PolyMTLib::Stop()
{
	if (active_)
	{
		active_ = false;

		// poczekaj, a¿ w¹tek siê skoñczy
		WaitForSingleObject(mainLoopThreadEventFinishedHandle_, INFINITE);
	}
}

cv::Mat* PolyMTLib::GetRawImage()
{
	return image_;
}

void PolyMTLib::GetMarkers(vector<Marker>& list)
{
	WaitForSingleObject(markersSync, INFINITE);
	list = vector<Marker>(markers);
	ReleaseMutex(markersSync);
}

PolyMTLib::~PolyMTLib(void)
{	
	Stop();

	vi_.release();

	delete image_;	
}
