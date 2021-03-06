#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
    ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);
    ofSetDataPathRoot("../Resources/data/");
    ofRunApp(new testApp()); // start the app
}
