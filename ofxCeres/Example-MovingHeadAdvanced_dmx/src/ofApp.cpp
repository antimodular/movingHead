#include "pch_ofApp.h"

#include "ofApp.h"
#include "ofxCeres.h"


//--------------------------------------------------------------
void ofApp::setup() {

	vector<string> movingHeadNames {
//        "6"
//        , "9_smallGobo_zoomOut"
//         "10_smallGobo_zoomIn"
//        , "99_smallGobo_zoomOut"
//        "smallGobo_10points_noFlip_zoomIn"
//        "1",
//        "2",
//        "3",
//        "4",
//        "5",
//        "6",
//        "7",
//        "8",
//        "9",
//        "10",
//        "11",
//        "12",
//        "13",
//        "14",
//        "15",
//        "16"
        
//        "1",
//        "2",
//        "3",
//        "4",
//        "5",
//        "6",
//        "7",
//        "8",
//        "9",
//        "10",
//        "11",
//        "12",
        "13"
//        "14",
//        "15",
//        "16"
	};

	// Initialise the moving heads
	for(auto movingHeadName : movingHeadNames) {
		this->movingHeads.emplace(movingHeadName, make_shared<MovingHead>());
	}
	this->selection = this->movingHeads.begin()->first;


	// Initialise the gui
	{
		this->gui.init();

		this->stripPanel = ofxCvGui::Panels::Groups::makeStrip();
		{
			this->stripPanel->setCellSizes({ -1, 400, 350 });
			this->gui.add(this->stripPanel);
		}

		// Add the panel for drawing 3D world
		{
			this->worldPanel = ofxCvGui::Panels::makeWorld();
			this->worldPanel->onDrawWorld += [this](ofCamera &) {
				this->drawWorld();
			};
			this->worldPanel->setGridEnabled(false);
//			this->worldPanel->getCamera().setCursorDrawEnabled(true);
			this->stripPanel->add(this->worldPanel);

		}

		// Add the widgets panel
		{
			this->widgetsPanel = ofxCvGui::Panels::makeWidgets();
			this->stripPanel->add(widgetsPanel);
		}

		// Add a blank panel in this slot for now (this will become the list panel)
		{
			this->listPanelHolder = make_shared<ofxCvGui::Panels::Groups::Grid>();
			this->listPanelHolder->add(this->movingHeads[selection]->getListPanel());

			this->stripPanel->add(listPanelHolder);
		}

		// Popualte the widgets
		this->repopulateWidgets();
	}

	// Load for json file
	this->load();

	// look at moving head #1 to start with
	{
		auto position4 = this->movingHeads.begin()->second->getTransform() * glm::vec4(0, 0, 0, 1);
		auto position = (glm::vec3) (position4 / position4.w);
		this->worldPanel->getCamera().lookAt(position);
	}
    
    dmx.connect(0,512); 
    
    dmx_focus = 127;
    dmx_coarseDimmer = 127; //30;
    /*
     //martin setup
     dmx.setLevel(1,228); //lamp on for martin
     dmx.update();
     shutterState = 38;
     dmx.setLevel(1,shutterState); //shutter open
     dmx.update();
     dmx.setLevel(2,0); //white
     */
    
    for(int i=0; i<=16; i++){
        int offset = (24*i); 
        dmx.setLevel(offset+22,255); //strobe stop
        
        dmx.setLevel(offset+23,255); //DIMMER coarse
        dmx.setLevel(offset+24,255); //DIMMER  fine
        
        dmx.setLevel(offset+10,64); //static gobo
        //    dmx.setLevel(offset+17,255); //zoom
        dmx.setLevel(offset+17,0); //zoom
        //    dmx.setLevel(offset+19,dmx_focus); //focus
        
        dmx.setLevel(offset+5,30); //pan tilt speed
         dmx.setLevel(offset+6,130); //lamp on
//        "LAMP_ON": "6, 130",
//        "LAMP_OFF": "6, 230",
    }
    
 
    for (int i = 0; i < 16; i++) {
        for (int n = 1; n <= 25; n++) {
            cout << (int)  dmx.getLevel(i*24 + n) << ", ";
        }
          cout<<endl;
    }
    cout<<endl;
    
    dmx.update();
    
    bFollow = false;
    targetY = 2.5;
}

//--------------------------------------------------------------
void ofApp::update() {
    
    if(bFollow == true){
        float tempX = ofMap(mouseX,0,ofGetWidth(),-4,5,true);
        //        float tempY = ofMap(mouseY,0,ofGetHeight(),-2,3,true);
        //        auto phase = ofGetElapsedTimef() * 2 * PI;
        //auto worldTarget =         glm::vec3(sin(phase),0.0f,cos(phase));
        worldTargetPoint = glm::vec3(tempX,targetY,0);
        for (const auto & movingHead : this->movingHeads) {
            movingHead.second->navigateToWorldTarget(worldTargetPoint);
        }
    }
    
	for (const auto & movingHead : this->movingHeads) {
		movingHead.second->update();
	}

    //MARK: this will be slow if no USB DMX device is detected
    this->renderDMX();
}

//--------------------------------------------------------------
void ofApp::draw() {

}

//--------------------------------------------------------------
void ofApp::drawWorld() {

	// Draw a floor grid
	if (this->drawGrid) {
		ofPushStyle();
		{
			ofSetColor(150);

			ofPushMatrix();
			{
				ofRotateDeg(180, 0, 1, 0);
				ofRotateDeg(-90, 0, 0, 1);
				ofDrawGridPlane(1.0f, 20, true);
			}
			ofPopMatrix();
		}
		ofPopStyle();
	}

	// Draw the moving heads
	for (auto & movingHead : this->movingHeads) {
		bool isSelected = movingHead.first == this->selection;
		if (isSelected) {
			movingHead.second->drawWorld(true);
		}
		else if(this->drawOtherFixtures) {
			movingHead.second->drawWorld(false);
		}
	}
}

//--------------------------------------------------------------
void ofApp::renderDMX() {
	// dmx values have addersses starting with 1, so we keep these addresses and throw away the first value
	vector<uint8_t> dmxValues(513, 0);

	for (auto & movingHead : this->movingHeads) {
		movingHead.second->renderDMX(dmxValues);
	}

	//--
	// HERE YOU NEED TO SEND DMX OUT
	//--
	//
	//

    for (int i = 0; i < 512; i++) {
//        cout << (int) dmxValues[i] << ", ";
//        if( i % 24 == 0)cout<<endl;
        dmx.setLevel(i+1, dmxValues[i]);
    }
//    cout<<endl;
    
//    dmx.setLevel(22,255); //strobe stop
//    dmx.setLevel(23,dmx_coarseDimmer); //DIMMER
//    
//    //    dmx.setLevel(10,64); //static gobo
//    dmx.setLevel(10,64); //static gobo
//    //    dmx.setLevel(17,255); //zoom
//    dmx.setLevel(17,0); //zoom
//    //    dmx.setLevel(19,dmx_focus); //focus
//    dmx.setLevel(5,30); //pan tilt speed
    
    //    for (int i = 0; i < 30; i++) {
    //        cout << (int)dmx.getLevel(i+1) << ", ";
    //    }
    //    cout << endl;
    //    dmx.setLevel(1,shutterState); //shutter open
    //    dmx.setLevel(2,0); //white
    
    
    dmx.update();
    
    for (int i = 0; i < 16; i++) {
        for (int n = 1; n <= 25; n++) {
            cout << (int)  dmx.getLevel(i*24 + n) << ", ";
        }
        cout<<endl;
    }
    cout<<endl;
    
    //
    //--
}

//--------------------------------------------------------------
void ofApp::repopulateWidgets() {
	this->widgetsPanel->clear();

	static shared_ptr<ofxCvGui::Widgets::MultipleChoice> selector;
	if (!selector) {
		selector = make_shared<ofxCvGui::Widgets::MultipleChoice>("Moving head");
		{
			for (auto & it : this->movingHeads) {
				selector->addOption(it.first);
			}
		}

		selector->onValueChange += [this](int) {
			this->selection = selector->getSelection();
			this->repopulateWidgets();

			// bring up the list in that panel slot
			this->listPanelHolder->clear();
			this->listPanelHolder->add(this->movingHeads[selection]->getListPanel());
		};
	}

	this->widgetsPanel->addFps();
	this->widgetsPanel->addButton("Save all", [this]() {
		this->save();
	});
	this->widgetsPanel->add(selector);

	this->widgetsPanel->addSpacer();

	this->movingHeads[this->selection]->populateWidgets(this->widgetsPanel);

	this->widgetsPanel->addSpacer();

	this->widgetsPanel->addToggle(this->drawGrid);
}

//--------------------------------------------------------------
void ofApp::load() {
	for (const auto & it : this->movingHeads) {
		ofFile file;
		file.open(it.first + ".json");
		if (file.exists()) {
			nlohmann::json json;
			file >> json;
			it.second->deserialize(json);
		}
	}
}

//--------------------------------------------------------------
void ofApp::save() {
	for (const auto & it : this->movingHeads) {
		nlohmann::json json;
		it.second->serialize(json);

		ofFile file;
		file.open(it.first + ".json", ofFile::Mode::WriteOnly);
		file << std::setw(4) << json;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == 'q') dmx_focus++;
    if(key == 'a') dmx_focus--;
    dmx_focus = ofClamp(dmx_focus, 0, 255);
    
    if(key == 'e') targetY+=0.01;
    if(key == 'd') targetY-= 0.01;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if(key == 'x') {
        if(shutterState == 38) shutterState = 0;
        else shutterState = 38;
    }
    if(key == 't') bFollow = !bFollow;
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
	if (this->worldPanel->isMouseOver()) {
//		this->movingHeads[this->selection]->setWorldCursorPosition(this->worldPanel->getCamera().getCursorWorld());
	}
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
