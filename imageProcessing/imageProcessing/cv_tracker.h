#pragma once

/////////////////////////////////////////////////
// jiunbae: https://github.com/jiunbae/MAT4015.git
// mean shift - object tracking.
// + dynamic search range
// + tracking object autosizing
// + model histogram update
//
// default@opencv
//@import 
//- .\include
//- .\lib\x86\vc12\bin
//@debug
//- opencv_world300d.lib
//- opencv_ts300d.lib
//@lib
//- .\lib\x86\vc12\lib

#ifndef __CV__TRACKER__
#define __CV__TRACKER__

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\video\video.hpp"

using namespace cv;

#define MY_HISTOGRAM_NAME "MY_histogram"
#define MY_TRACKER_NAME "MY_tracking"
#define CV_TRACKER_NAME "CV_tracking"

static const double SEARCH_MIN = 4, SEARCH_MAX = 32;
static const double EXTEND_LIMIT = (1 / 4.);
static const double RECT_EXTEND_MAX = 10, RECT_STEP = 2.5;
static const double RECT_SIZE_MIN = (3 / 4.), RECT_SIZE_MAX = (5 / 4.);


namespace cv {
	// color model enum
	enum COLOR_MODEL { CM_HSV, CM_RGB, CM_HUE, CM_GRAY };

	// tracker parameter
	struct TrackerParam
	{
		COLOR_MODEL color_model;
		double channel_ratio[3] = { 0.5, 0.25, 0.25 };
		// hists_bins mean how many bar in histogram
		int hist_bins, max_itrs, sampling;
		double search_range;

		TrackerParam()
		{
			hist_bins = 16;
			max_itrs = 32;
			search_range = 16;
			sampling = 8;
			color_model = CM_HSV;
		}
	};

	class Tracker {
	public:
		Tracker();
		~Tracker();

		void initilize(Mat, Rect, COLOR_MODEL);
		bool run(Mat);

	protected:
		TrackerParam param;
		MatND model3d;
		Mat model;
		Rect cvRect, myRect, rect;
		double * objectHists;

		// show histogram bar chart x8
		char HistRatio = 8;

		// if off, openCV method apply camshift;
		bool cvMeanshift = true;

		// set value to matrix pixel
		void matrixSet(Mat&, int, int, double[]);
		// get value of matrix pixel
		int matrixAt(const Mat&, int x, int y);

	private:
		// cv::histogram
		void cvHistogram(const Mat&, const Rect);
		// cv::backprojection
		void cvBackProject(const Mat&, Mat&);
		// cv::meanshift
		void cvMeanShift(Mat&, const Mat&, Rect);
		// cv::camshift
		void cvCamShift(Mat&, const Mat&, Rect);

		// histogram show
		// param: histogram
		// show histogram to bar graph (It's too samll to see, so multiply Ratio)
		void myShowHistogram(double *);

		// calc histogram
		// param: img, rect 
		// return histogram (double *) rect in img
		double * myHistogram(const Mat&, const Rect&);

		// get histogram value,
		// param: img, x, y, histogram
		double myHistogramValue(const Mat&, int x, int y, double*);

		// calc histogram similarity
		// param: img, x, y, hists to compare with
		// we get hists in img, rect (x - w/2 to x + w/2, y same)
		double mySimilarity(const Mat&, Rect, double*);
	};
}
#endif