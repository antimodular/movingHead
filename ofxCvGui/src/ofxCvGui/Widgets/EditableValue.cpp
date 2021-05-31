#include "pch_ofxCvGui.h"

namespace ofxCvGui {
    namespace Widgets {
        //specialisation for ofParameter<string>
        template<> EditableValue<string>::EditableValue(ofParameter<string> & parameter) :
        LiveValue<string>(parameter.getName(), [&parameter]() { return parameter;}) {
            this->setEditable(true);
            this->onEditValue += [&parameter, this](string & userValueString) {
                parameter.set(userValueString);
                this->onValueChange.notifyListeners(userValueString);
            };
        }
    }
}
