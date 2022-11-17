#include "opencv.h"


int getContours2(Mat& imgDil, Mat& img) {
	int temp=0;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		string objectType;
		
		if (area > 100)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);

			int objCor = (int)conPoly[i].size();

			if (objCor !=0) temp = 1;
			if (objCor == 0) temp = 0;
		}
	}
	return temp;
}

void getContours(Mat &imgDil, Mat &img, int& temp){
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		string objectType;

		if (area > 600)
		{
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
		//	cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);

			int objCor = (int)conPoly[i].size();
			if (objCor == 10) { objectType = "F"; }
			else if (objCor == 6) { objectType = "L"; }
			else if (objCor == 8) {
				if (temp == 1) { objectType = "E";  }
				else if (temp == 0) { objectType = 'T'; }
				temp = 0;
			}
			else if (objCor == 12) { objectType = 'C'; }
			////else if (objCor == 4)
			//{
			//	float aspRatio = (float)boundRect[i].width / (float)boundRect[i].height;
			//	cout << aspRatio << endl;
			//	if (aspRatio > 0.95 && aspRatio < 1.05) { objectType = "Square"; }
			//	else { objectType = "Rect"; }
			//}
			//else if (objCor > 4) { objectType = "Circle"; }

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2);
			putText(img, objectType, { boundRect[i].x,boundRect[i].y - 5 }, FONT_HERSHEY_PLAIN, 1, Scalar(0, 69, 255), 2);
			circle(img, (boundRect[i].tl() + boundRect[i].br()) / 2, 0, Scalar(0, 255, 255), 8);
			//change to world coordinates	
			if (objectType=="T" || objectType == "E" || objectType == "L") {
				double imgx, imgy;
				Point2i down = boundRect[i].tl();
				Point2i up = boundRect[i].br();
				imgx = (down.y+up.y)/2.;
				imgy = (down.x+up.x)/2.;
				int ymin = -1, ysubmin = -1;
				int xmin = -1, xsubmin = -1;
				double xtrue=0, ytrue=0;
				int t=0;
				int Minimum_distance_y = 100;
				int Minimum_distance_x = 100;
				int jtemp = 0;
				int x[37][35] = { 43,41,39,37,34,31,28,25,22,20,17,15,13,11,9,8,7,6,5,5,5,5,5,5,6,8,9,10,13,15,16,18,20,22,25,
							  53,50,47,45,41,38,35,33,29,27,25,22,20,18,16,15,14,13,12,12,12,12,12,12,13,14,16,18,19,21,23,25,27,29,32,
							  59,58,55,52,49,46,43,40,37,35,32,30,27,26,24,23,21,20,19,19,19,19,19,19,20,22,24,25,27,29,31,33,36,38,40,
							  68,66,63,60,57,54,52,48,46,43,40,38,35,33,32,30,29,28,27,27,26,26,27,27,28,30,32,33,35,37,39,41,43,46,48,
							  76,75,71,68,65,62,59,57,54,51,48,46,44,42,40,39,38,37,36,35,35,35,36,36,37,39,40,42,43,46,48,50,52,54,56,
							  85,83,80,77,74,71,69,65,63,60,57,55,53,51,49,48,47,46,45,44,44,44,45,45,46,47,49,50,52,54,56,58,60,63,65,
							  94,91,89,86,83,81,78,75,72,70,67,65,63,61,59,58,56,55,54,54,53,54,54,55,56,57,58,60,61,63,65,68,70,73,74,
							  104,101,98,96,92,90,87,85,83,80,77,75,73,71,69,68,66,65,64,64,64,64,64,65,66,67,68,70,71,73,75,77,79,82,84,
							  112,111,109,106,103,101,98,96,93,91,88,86,84,82,80,79,77,76,75,74,74,74,75,75,76,77,79,80,82,83,85,87,89,92,93,
							  0,0,118,116,113,111,108,106,103,101,99,97,95,93,91,90,88,87,86,86,85,85,86,86,87,88,89,91,93,94,96,98,100,102,103,
							  0,0,128,127,125,122,120,117,115,113,111,108,106,105,103,101,100,99,98,98,97,97,97,98,98,99,101,102,104,105,107,108,110,113,114,
							  0,0,140,138,136,133,131,129,127,125,123,121,119,117,115,114,112,111,110,110,109,109,109,110,110,112,113,114,115,117,118,120,122,123,125,
							  0,0,151,149,148,145,143,142,140,138,136,134,133,131,129,128,126,125,125,124,123,123,122,123,124,125,126,127,128,129,130,131,133,134,136,
							  0,0,162,161,160,158,156,155,153,151,150,148,147,145,144,143,141,141,140,139,138,138,138,138,139,139,140,141,142,143,144,145,146,147,148,
							  0,0,174,173,172,170,169,168,167,165,164,162,161,160,159,158,157,156,155,154,153,153,153,153,153,154,155,155,155,156,157,158,159,160,161,
							  0,0,187,186,185,184,182,182,181,179,178,177,176,175,174,173,172,171,170,169,169,168,168,168,168,169,169,169,170,170,171,171,172,172,173,
							  0,0,200,200,198,197,196,196,195,193,192,192,191,190,189,188,188,187,186,185,184,185,184,184,184,184,184,184,184,184,184,184,185,185,185,
							  0,0,212,211,212,212,210,210,209,208,208,207,207,206,205,204,204,203,202,202,201,201,200,200,199,199,199,199,199,199,199,198,198,198,198,
							  0,0,225,224,225,224,224,224,223,223,223,222,222,221,221,221,220,219,219,218,217,217,216,216,215,215,214,214,213,213,212,212,212,211,211,
							  0,0,240,238,238,238,238,238,238,238,237,237,237,237,236,236,236,235,235,234,234,234,233,232,231,230,229,229,228,227,226,226,225,224,224,
							  0,0,251,251,251,252,251,251,252,252,252,253,253,253,253,253,252,252,252,251,250,250,249,248,247,246,245,244,243,241,241,239,239,238,237,
							  0,0,264,265,265,265,265,266,266,267,267,268,268,268,268,268,268,268,267,267,266,266,264,263,262,261,260,258,257,256,254,253,252,250,248,
							  0,276,277,278,278,278,280,279,281,281,282,283,283,284,284,284,284,284,283,283,282,281,280,279,277,276,274,273,271,270,268,266,264,263,261,
							  0,287,287,289,290,292,292,293,295,296,296,297,298,299,299,299,299,299,299,298,297,297,295,294,292,291,289,287,285,283,281,279,278,276,273,
							  0,296,298,299,301,303,304,305,307,308,310,311,312,313,314,314,314,314,314,313,312,311,309,308,307,305,303,301,300,297,295,293,290,288,286,
							  0,307,309,312,313,314,316,318,320,321,323,324,326,327,328,328,328,328,328,328,326,325,324,322,320,319,316,314,312,310,307,305,303,300,297,
							  0,0,321,323,325,326,328,330,332,334,335,337,339,340,341,341,342,342,342,341,340,339,337,336,334,332,330,327,325,322,319,317,314,312,310,
							  0,332,332,335,336,338,340,342,344,345,347,349,351,352,353,353,354,354,354,354,353,351,350,348,347,344,342,339,336,334,331,329,326,323,320,
							  0,342,344,345,348,349,351,353,355,357,359,360,362,363,364,365,366,366,366,365,365,364,362,360,358,356,353,351,348,345,343,340,337,334,331,
							  0,350,352,355,357,359,361,364,366,368,370,371,373,374,375,376,377,377,377,377,376,375,373,371,369,367,365,362,360,356,354,351,348,345,342,
							  356,360,362,364,367,369,371,374,376,378,381,382,383,385,386,387,388,388,388,387,387,386,384,382,380,378,375,373,370,367,364,361,358,355,351,
							  366,369,371,374,376,378,381,384,386,388,390,392,393,395,396,397,398,398,398,397,397,396,395,393,391,388,385,383,380,377,374,371,368,364,361,
							  376,377,380,382,385,388,391,393,395,398,399,401,403,404,406,406,407,407,407,407,406,406,404,402,400,398,395,393,390,387,384,380,377,373,370,
							  384,387,389,392,395,397,400,402,405,406,408,410,412,413,414,415,416,416,416,416,416,414,412,411,409,407,404,401,399,395,392,389,385,383,379,
							  392,394,396,399,402,405,407,410,412,414,416,418,420,421,423,423,424,424,424,424,424,422,420,419,416,414,412,409,407,404,400,398,395,391,387,
							  399,402,406,409,411,414,415,418,420,422,425,427,428,429,430,431,432,432,433,432,431,430,428,427,425,422,421,417,414,412,408,405,402,398,395,
							  407,409,413,415,419,421,424,426,427,430,432,434,435,437,438,439,439,439,439,439,439,437,435,434,431,429,427,425,421,418,416,412,409,407,404
				};
				int y[37][35] = { 150,157,164,171,179,187,196,206,215,225,235,245,256,267,278,290,301,313,325,336,348,359,371,383,394,405,416,426,437,447,456,466,474,483,491,
								  147,154,162,168,177,185,194,202,212,222,232,243,254,265,277,288,301,312,325,337,349,361,373,385,395,407,418,429,439,449,459,468,477,486,494,
								  144,150,158,166,174,181,191,200,210,220,230,241,253,264,276,288,300,313,325,337,350,362,374,386,398,409,421,431,441,452,461,471,480,489,497,
								  141,148,156,162,170,179,188,197,207,218,229,240,251,263,275,287,299,312,325,338,350,363,375,388,399,411,423,433,444,455,464,474,483,492,500,
								  138,144,152,160,168,177,186,195,205,216,227,238,250,262,274,287,299,312,325,338,351,364,377,389,401,413,425,436,447,458,468,477,486,495,503,
								  135,143,151,157,165,174,184,193,203,214,224,236,248,260,273,286,299,312,325,339,352,365,378,391,403,415,427,439,450,460,470,480,489,498,506,
								  133,141,148,155,164,172,181,191,201,212,223,235,247,259,272,285,298,321,326,340,353,366,380,393,406,418,430,442,453,463,474,483,493,502,510,
								  130,139,147,154,163,171,179,189,199,210,221,233,245,258,271,284,298,312,326,340,354,367,381,394,407,420,432,444,455,466,476,486,496,505,513,
								  128,136,143,152,159,167,177,187,197,208,220,231,244,257,270,284,298,312,327,341,355,369,383,396,410,422,435,447,458,469,480,490,498,507,515,
								  0,0,142,149,157,166,176,185,195,207,218,230,243,256,270,283,297,312,327,342,356,370,384,398,412,425,438,450,461,472,483,492,502,511,518,
								  0,0,141,147,155,164,173,183,194,205,216,229,241,255,269,283,297,312,327,342,357,373,386,400,414,427,440,453,464,476,486,496,505,514,522,
								  0,0,138,144,153,162,171,181,191,202,214,227,240,254,267,282,297,312,327,343,358,373,388,403,417,430,443,456,468,479,489,499,508,517,525,
								  0,0,135,144,152,161,170,179,190,202,214,226,240,253,267,282,297,312,328,344,359,375,390,404,419,432,445,458,470,482,492,502,511,520,528,
								  0,0,136,143,151,160,169,178,189,201,213,225,239,253,267,282,297,313,329,344,360,376,391,406,420,434,447,459,471,482,493,503,513,521,529,
								  0,0,134,143,150,159,168,178,188,199,212,225,238,252,267,282,297,313,329,345,360,376,392,407,421,435,448,461,473,484,495,504,514,523,531,
								  0,0,135,142,150,157,167,177,187,199,211,224,238,252,267,282,298,313,330,346,361,378,393,408,422,437,450,463,474,486,497,507,516,524,533,
								  0,0,135,142,150,157,167,176,187,198,211,224,238,252,267,283,298,314,330,346,362,379,394,409,424,438,451,464,476,487,498,508,517,526,534,
								  0,0,136,145,151,157,167,177,187,199,211,223,237,252,267,283,298,314,331,347,363,379,395,410,425,439,452,465,477,489,499,509,518,527,535,
								  0,0,135,142,150,157,166,176,187,199,211,223,237,253,268,283,299,315,331,348,364,380,396,411,425,440,453,466,478,489,500,510,519,528,536,
								  0,0,134,143,150,158,167,176,188,199,211,224,238,253,268,283,299,315,332,348,365,381,396,411,426,440,453,466,478,490,501,510,519,528,536,
								  0,0,135,142,150,158,167,177,188,200,212,225,239,253,269,284,300,316,333,349,365,381,397,412,426,440,454,467,479,490,501,511,520,528,538,
								  0,0,132,142,151,158,168,177,188,201,213,226,240,254,269,285,301,317,333,349,365,382,397,412,426,441,454,466,478,490,501,510,520,528,537,
								  0,126,136,143,152,160,169,179,190,202,214,227,241,255,270,286,301,317,333,350,366,382,397,412,426,440,454,466,478,490,500,510,519,528,536,
								  0,128,136,145,153,161,171,180,192,203,215,229,242,257,271,287,302,318,334,350,366,381,397,412,426,440,453,466,477,489,499,510,519,527,536,
								  0,128,137,146,155,163,171,182,193,204,216,229,243,258,272,287,303,319,335,351,366,382,397,412,426,440,453,465,477,488,499,509,518,527,535,
								  0,129,140,148,156,164,174,184,195,207,218,231,245,259,274,289,304,320,335,351,367,382,397,411,426,439,452,464,476,487,498,507,517,525,535,
								  0,0,141,149,158,167,176,186,198,209,221,234,247,261,275,290,305,321,336,351,367,382,396,411,425,439,451,464,474,485,496,506,515,524,533,
								  0,137,145,153,162,170,179,189,200,212,224,236,250,263,277,291,306,321,336,351,366,381,396,410,424,437,449,462,473,484,494,504,513,522,531,
								  0,138,147,155,165,173,181,192,202,214,226,238,251,265,279,293,307,322,337,351,366,381,395,409,422,436,448,460,471,482,492,502,512,521,529,
								  0,141,150,159,167,175,184,194,204,216,227,240,253,266,280,294,308,323,337,351,366,380,394,408,421,434,446,458,469,480,491,501,510,519,527,
								  135,144,153,160,169,178,187,196,207,218,230,242,255,268,282,295,309,323,337,351,365,380,393,407,420,433,445,456,468,478,489,498,508,516,525,
								  138,147,155,164,171,181,191,200,210,221,233,245,257,270,283,296,310,324,338,351,365,379,392,406,419,431,443,454,466,477,486,496,505,513,522,
								  141,150,158,166,175,184,193,202,213,223,235,247,259,272,285,298,311,325,338,351,365,379,392,405,417,430,441,453,463,474,484,494,503,511,520,
								  145,153,161,169,178,186,195,206,216,226,238,249,261,274,286,299,312,325,339,352,365,378,391,404,416,428,440,451,462,473,483,491,501,509,517,
								  149,156,164,172,181,190,198,208,218,229,240,251,263,275,288,300,313,326,339,352,365,378,391,403,415,426,438,450,460,470,480,488,498,505,515,
								  150,159,169,175,184,193,201,211,221,232,242,254,265,277,289,301,314,326,339,352,365,377,390,401,413,424,436,446,457,467,477,486,494,504,513,
								  153,163,171,179,188,196,205,215,224,234,245,256,267,280,291,303,315,327,339,352,365,376,389,401,412,424,434,445,456,465,476,485,493,501,509
				};
				int xtemp[37] = {0};
				int ytemp[37] = {0};
				int k = 0;
				int m = 0;
				int tmpi = 0;
				int tmpj = 0;
				coord aa[50];
				
				std::cout << "imgx:" << imgx << "\n";
				std::cout << "imgy:" << imgy << "\n";

				for (int i = 0; i < 37; i++) {
					for (int j = 0; j < 35; j++) {
						if (fabs(imgx - x[i][j]) <= 2) {
							xtemp[k] = i;
							k++;
						}
					}
				}
				std::cout << "k(x):" << k << "\n";
				//std::cout << x[tmpi][tmpj] << "\n";
				

				/*if (imgx - x[xmin][jtemp] > 0) {
					xsubmin = xmin+1;
				}
				else {
					xsubmin = xmin;
					xmin = xmin-1;
				}*/
				
				
				
				//std::cout << "xsubmin:" << xsubmin << "\n";

				for (int i = 0; i < k; i++) {
					for (int j = 0; j < 35; j++) {
						if (fabs(imgy - y[xtemp[i]][j]) <= 10) {
							aa[m].x = xtemp[i];
							aa[m].y = j;
							m++;
						}
					}
				}
				//std::cout << y[tmpi][tmpj] << "\n";
				std::cout << "m(y):" << m << "\n";
				for (int i = 0; i < k; i++) {
					std::cout << xtemp[i] << " ";
				}
				std::cout << "\n";
				for (int i = 0; i < m; i++) {
					std::cout << "(" << aa[i].x << "," << aa[i].y << ")" << "\n";
				}
				std::cout << "\n";
				double dis = 0.0;
				double mindis = 10000.0;
				for (int i = 0; i < m; i++) {
					dis = sqrt(pow(imgx - aa[i].x, 2.0) + pow(imgy - aa[i].y, 2.0));
					if (dis < mindis) {
						mindis = dis;
						xmin = aa[i].x-1;
						ymin = aa[i].y;
					}
					
				}
				
				if (imgy - y[xmin][ymin] >= 0)
					ysubmin = ymin + 1;
				else if (imgy - y[xmin][ymin] < 0) {
					ysubmin = ymin;
					ymin = ymin - 1;
				}

				
				//std::cout << "ysubmin:" << ysubmin << "\n";
				if (imgx - x[xmin][ymin] >= 0)
					xsubmin = xmin + 1;
				else if (imgx - x[xmin][ymin] < 0) {
					xsubmin = xmin;
					xmin = xmin - 1;
				}
				std::cout << "xmin:" << xmin << "\n";
				std::cout << "ymin:" << ymin << "\n";
				std::cout << "xsubmin:" << xsubmin << "\n";
				std::cout << "ysubmin:" << ysubmin << "\n";
				/*printf("%lf,%lf\n,%lf,%lf\n,%lf,%lf\n,%lf,%lf\n", (xmin * 1.5 - 27)* (-1), (ymin * 1.5 - 51)* (-1), (xmin * 1.5 - 27)* (-1), (ysubmin * 1.5 - 51)* (-1), (xsubmin * 1.5 - 27)* (-1), (ymin * 1.5 - 51)* (-1),(xsubmin*1.5-27)*(-1), (ysubmin * 1.5 - 51)* (-1));*/
					if (xmin != xsubmin) 
						//xtrue = (xmin * 1.5 - 27 + /*((1.5 * abs(imgx - x[xmin][ymin])) / (abs(x[xmin][ymin] - x[xsubmin][ymin])))*/) * (-1);
						xtrue = (1.5 * (((imgx - x[xmin][ymin]) * (xsubmin - xmin)) / (x[xsubmin][ymin] - x[xmin][ymin]) + xmin) - 27.) * (-1)*0.75;
					else
						xtrue = (-1) * (xmin * 1.5 - 27.);
					if (ymin != ysubmin) 
						//ytrue = (ymin * 1.5 /*+((1.5 * abs(imgy - y[xmin][ymin])) / (abs(y[xmin][ymin] - y[xmin][ysubmin])))*/ - 51) * (-1) ;
						ytrue = (1.5*(34.-(((imgy - y[xmin][ymin]) * (ysubmin - ymin)) / (y[xmin][ysubmin] - y[xmin][ymin]) + ymin) ))*0.6 +10;
					else 
						ytrue = (-1) * (ymin * 1.5 - 51)+1;
			 //cout << imgx << " " << imgy<< " "  << endl;
				//cout << xtrue << " " << ytrue << " " << objectType << endl;
				if (objectType == "T") {
					square_coord[0][0] = xtrue;
					square_coord[0][1] = ytrue;
				}
				else if (objectType == "E") {
					square_coord[1][0] = xtrue;
					square_coord[1][1] = ytrue;
				}
				else if (objectType == "L") {
					square_coord[2][0] = xtrue;
					square_coord[2][1] = ytrue;
				} 
			}
		}
	}
	imshow("Image", img);
}

int  open_camera() {
	VideoCapture cap(1);

	if (!cap.isOpened()) {
		cout << "Camera not found" << endl;
		return 0;
	}
	int temp;
	Mat img;
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;
	Mat imgGray2, imgBlur2, imgCanny2, imgDil2, imgErode2;
	int counter = 0;
	while (true) {
		bool success = cap.read(img);
		if (!success) {
			break;
		}
		Mat imgHSV, mask;
		
		int hmin = 14, smin =144, vmin = 225, hmax = 116, smax = 255, vmax = 255;
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		inRange(imgHSV, lower, upper, mask);
		//cvtColor(mask, imgGray, COLOR_BGR2GRAY);
		GaussianBlur(mask, imgBlur, Size(3, 3), 50, 255);
		Canny(imgBlur, imgCanny, 150, 200);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(imgCanny, imgDil, kernel);

		Mat imgHSV2, mask2;

		int hmin2 = 25, smin2 = 100, vmin2 = 190, hmax2 = 81, smax2 = 178, vmax2 = 255;
		Scalar lower2(hmin2, smin2, vmin2);
		Scalar upper2(hmax2, smax2, vmax2);
		cvtColor(img, imgHSV2, COLOR_BGR2HSV);
		inRange(imgHSV2, lower2, upper2, mask2);
		//cvtColor(mask, imgGray, COLOR_BGR2GRAY);
		GaussianBlur(mask2, imgBlur2, Size(3, 3), 50, 255);
		Canny(imgBlur2, imgCanny2, 150, 200);
		Mat kernel2 = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(imgCanny2, imgDil2, kernel2);
		temp = getContours2(imgDil2, img );
		getContours(imgDil, img, temp);
		
		counter++;
		waitKey(1);
		if (counter == 35) {
			for (int i=0;i<3;i++)
			{
				printf("%lf %lf\n", square_coord[i][0], square_coord[i][1]);
			}
			break;
		}
	}
	//string path = "Resources/shape.jpg";
	//Mat img = imread(path);
	Mat imgResize;
	resize(img, imgResize, Size(), 0.5, 0.5);
	//Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;
	// Preprocessing
	//cvtColor(img, imgGray, COLOR_BGR2GRAY);
	//GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	//Canny(imgBlur, imgCanny, 25, 75);
	//Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate(imgCanny, imgDil, kernel);

	//getContours(imgDil, img);

	//imshow("Image", img);
	//imshow("Image Gray", imgGray);
	//imshow("Image Blur", imgBlur);
	//imshow("Image Canny", imgCanny);
	//imshow("Image Dil", imgDil);
	waitKey(0);
	return 0;
}

float coords_array(int n)
{
    return square_coord[n][0],square_coord[n][1];
}