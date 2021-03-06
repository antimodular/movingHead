#include "pch_ofxCvGui.h"

namespace ofxCvGui {
	namespace Widgets {
		//----------
		Spacer::Spacer() {
            this->setBounds(ofRectangle(0, 0, 100, 25));
//            this->setBounds(ofRectangle(0, 0, 100, 10)); //stephan
			this->onDraw += [] (DrawArguments & args) {
				auto center = args.localBounds.getCenter();

				const auto spread = glm::vec2(-10.0f, 2.0f);
				ofDrawRectRounded(ofRectangle(center - spread, center + spread), 2.0f);
			};
		}
	}
}
