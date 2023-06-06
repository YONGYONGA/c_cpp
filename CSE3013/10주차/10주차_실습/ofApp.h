#pragma once

#include "ofMain.h"
#include "water_line.h"
class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	int draw_flag;
	int load_flag;
	int waterfall_start_flag;
	int selection_dot;
	int currentIndex;
	int num_of_line;
	int num_of_dot;
	/*		typedef struct Dot {
				int x1, y1;
			}Dot;*/
	Dot* dot;
	typedef struct Dott {
		int x, y;
	}Dott;
	Dott target_dot;
	/*		typedef struct Line {
				int x1, y1, x2, y2;
				double slope, x_coef, y_coef, constant;
			}LineSegment;*/
	LineSegment* lineseg;
	vector <WaterLine> wl;
	float dot_diameter;
	void resetWater();
	void processOpenFileSelection(ofFileDialogResult openFileResult);
};
