#pragma once

// do wycieków pamiêci:
//#include "../include/vld.h"

#define _CRT_SECURE_NO_DEPRECATE
#define M_PI_32 4.712388980384688f// 270
#define M_PI 3.14159265358979323846f //180
#define M_PI_2 1.57079632679489661923f //90
#define M_PI_4 0.785398163397448309615f //45

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <list>

using namespace std;

class Marker
{
public: 		
	Marker(cv::Point center, float rotation, float size, int id):center(center), rotation(rotation), size(size), id(id), LastVisible(time(NULL)){};
	Marker(){};
	int id;
	cv::Point center;
	float rotation;
	float size;
	list<Marker> history;

	// dane pomocnicze
	time_t LastVisible;
	~Marker()
	{
		history.clear();
	}
};

class PolyMTLib
{
public:
	// parametry
	int cameraRotation;
	bool showDebugImages;
	unsigned int disposeUnseenMarkerAfter; // usuwaj niewodoczne markery po up³ywie zadanego czasu
	unsigned int maxMakerHistoryLength;	

	// markery
	vector<Marker> markers;
	HANDLE markersSync;

	PolyMTLib();
	~PolyMTLib();

	// Inicjuj klasê (inicjacja kamery)
	void Init(int deviceID = 0, int width = 320, int height = 240 , int FPS = 30);
	void Reinit(int deviceID = 0, int width = 320, int height = 240 , int FPS = 30);

	// Rozpocznij rozpoznawanie
	void Start();

	// Zakoñcz rozpoznawanie
	inline void Stop();

	// zwróæ obraz z kamery
	inline cv::Mat* GetRawImage();

	// zwróæ markery
	void GetMarkers(vector<Marker>& list);

	// zwróæ rozmiary kamery
	cv::Size GetScreenSize();

	// W³¹cz/wy³¹cz okna do debugowania
	void ToggleDebugWindows();

	cv::Mat* image_;
private:
	// VideoInput
	int width_;
	int height_;
	int fps_;
	cv::VideoCapture vi_;
	int deviceID_;	
	HANDLE mainLoopThreadEventFinishedHandle_;
	HANDLE debugWindowsSync_;
	
	// Czy klasa jest aktywna
	bool active_;

	// KOLORY:
	int nextMarkerID;// numer kolejno wykrytego markera	
	float computeRotation(cv::Point center, cv::Point top, float size);

	// kodowanie hamminga
	int checkParity(short digits);
	int PolyMTLib::HammingCheck(short digits);

	void mainLoopAr();

	// Obrót obrazka wejœciowego
	cv::Mat rotateImage();

	int getMarkerID(cv::Mat canonicalMarkerTresh, Marker * marker);
		
	static DWORD WINAPI startLoop(LPVOID param);
};

