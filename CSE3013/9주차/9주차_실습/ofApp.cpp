#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second

    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255, 255, 255);
    ofSetLineWidth(4);

    draw_flag = 0;
    load_flag = 0;
    dot_diameter = 20.0f;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(127, 23, 31);  // Set the drawing color to brown
    int i;
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);


    ofSetLineWidth(5);
    if (draw_flag) {

        for (i = 0; i < num_of_line; i++) //�ٱ׸���
            ofDrawLine(lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        for (i = 0; i < num_of_dot; i++) {
            if (i == water_point) { //���� �������� ���̶��
                ofSetColor(255,0,0); //����
                ofDrawCircle(dots[i][0], dots[i][1],10);
            }
            else
            {
                ofSetColor(0, 0, 0); //�׿ܿ� ����
                ofDrawCircle(dots[i][0], dots[i][1], 10);
            }
        }
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */
        


         // 2nd week portion.
        ofSetLineWidth(2);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    if (key == 'q') {
        // Reset flags
        draw_flag = 0;

        // Free the dynamically allocated memory exits.
        for (int i = 0; i < num_of_line; i++) //lines�迭�����
            delete lines[i];
        for (int i = 0; i < num_of_dot; i++) //dots�迭 �����
            delete dots[i];
        delete lines;
        delete dots;
        delete dotschecker; //�������� �迭�� �����
        cout << "Dynamically allocated memory has been freed." << endl;

        _Exit(0);
    }
    if (key == 'd') {
        if (!load_flag) return;
        draw_flag = 1;
        draw();

        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
    }
    if (key == 's') {
        // 2nd week portion.
    }
    if (key == 'e') {
        // 2nd week portion.
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    int i;
    if (key == 'l') {
        // Open the Open File Dialog
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

        // Check whether the user opened a file
        if (openFileResult.bSuccess) {
            ofLogVerbose("User selected a file");

            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        } 
        else//���� ���ٸ� error
        {
            ofLogVerbose("error");
        }
    }

    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
    if (key == OF_KEY_RIGHT) {
        int k = dotschecker[water_point];  //���� ��ġ ����
        if (k == num_of_dot - 1) {  //�������̶��. �� ����ũ�ٸ�
            for (i = 0; i < num_of_dot; i++) {
                if (k - 1 == dotschecker[i])  //�ٷ� �ؼ���(�ι�°�� ū��) ã��
                    break;
            }
        }
        else
        {
            for (i = 0; i < num_of_dot; i++) {
                if (k + 1 == dotschecker[i]) //�ٷ� ���� ���� ã��
                    break;
            }
        }
        water_point = i;  //i��° ���Ҹ� ã�� ���̴�, water�� ���� ���� ���� i
        cout << "Selcted Dot Coordinate is (" << dots[water_point][0] << "," << dots[water_point][1] << ")" << endl;
    }
    if (key == OF_KEY_LEFT) { //right�� �ݴ�
        int k = dotschecker[water_point]; 
        if (k == 0) {
            for (i = 0; i < num_of_dot; i++) {
                if (1 == dotschecker[i])
                    break;
            }
        }
        else
        {
            for (i = 0; i < num_of_dot; i++) {
                if (k -1 == dotschecker[i])
                    break;
            }
        }
        water_point = i;
        cout << "Selcted Dot Coordinate is (" << dots[water_point][0] << "," << dots[water_point][1] << ")" << endl;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    //Path to the comma delimited file
    //string fileName = "input.txt";

    string fileName = openFileResult.getName();
    ofFile file(fileName);

    if (!file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;

    ofBuffer buffer(file);

    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
    int i,j, t=0;

    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */


    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;

 
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");

        if (words.size() == 1) {
            if (input_type == 0) { // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                lines = new int* [num_of_line];   //�� ������ŭ �迭����(2������ ��)
                for (i = 0; i < num_of_line; i++)
                    lines[i] = new int[4]; //�� �ึ�� 4���� ����
            }
            else { // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;
                dots = new int* [num_of_dot];   //�� ������ŭ �迭����(��)
                for (i = 0; i < num_of_dot; i++)
                    dots[i] = new int[2];   //�� �ึ�� 2���� ����
            }
        }
        else if (words.size() >= 2) {
            int x1, y1, x2, y2;
            if (input_type == 0) { // Input for actual information of lines
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());
                if (x1<0 || x2<0 || x1>ofGetWidth() || x2>ofGetWidth() || y1<=0 || y2<0 || y1>=ofGetHeight() || y2>ofGetHeight())  //���Ǹ�����
                    return;
                lines[t][0] = x1; lines[t][1] = y1; lines[t][2] = x2; lines[t][3] = y2;   //���ʴ�� x1,y1,x2,y2����
                t++;
            }
            else { // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                if (x1<0 || x1>ofGetWidth() || y1<0 || y1>ofGetHeight())  //���Ǹ�����
                    return;
                dots[t][0] = x1;  //���ʴ�� x1,y1����
                dots[t][1] = y1;
                t++;

            }
            if (num_of_line == t) {  //�ٴ� ���������� ���� size�� 2�ΰ��� ������ ����. �� inputtyhpe ����
                input_type = -1;
                t = 0;
            }

        } 
        // End of else if.
    } // End of for-loop (Read file line by line).
    dotschecker = new int[num_of_dot];   //�� ��ġ���� ��� �迭 ����
    for (i = 0; i < num_of_dot; i++) {
        dotschecker[i] = 0;//������ 0
        for (j = 0; j < i; j++) {  //ó������ i����
            if (dots[j][0] > dots[i][0]) {  //j��°���� ũ�� j��° ����++
                dotschecker[j]++;
            }
            else if (dots[j][0] == dots[i][0] && dots[j][1] > dots[i][1])    //x��ǥ�� ���ٸ�, y��ǥ ���ؼ�, j��°���� ũ��(�� �Ʒ��� �ִٸ�)j���� ++
                dotschecker[j]++;
            else
            {
                dotschecker[i]++;  //�׿ܿ� i��°���� ũ�ܰ��̴� i++
            }
        }
    }
    //initializeWaterLines();
}

void ofApp::initializeWaterLines() {
    ;
}