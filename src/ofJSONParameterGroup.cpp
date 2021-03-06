#include "ofJSONParameterGroup.h"


ofJSONParameterGroup::ofJSONParameterGroup(void)
{
}


ofJSONParameterGroup::~ofJSONParameterGroup(void)
{
}


ofxJSONElement ofJSONParameterGroup::jsonElement() {

	ofxJSONElement children = ofxJSONElement::Value(Json::arrayValue);
	for (int i = 0; i < this->size(); i++) {
		
		ofxJSONElement subElement = ofxJSONElement::Value(Json::objectValue);
		subElement["name"] = this->getName(i);

		string type = this->getType(i);
		if (type == "class ofParameterGroup") {

			subElement["type"] = "ofParameterGroup";
			subElement["escapedName"] = this->getGroup(i).getEscapedName();
			ofParameterGroup group = this->getGroup(i);

			ofJSONParameterGroup jsonGroup = ofJSONParameterGroup();
			for (int j = 0; j < group.size(); j++) {
				jsonGroup.add(group.get(j));
				jsonGroup.setName(group.getName());
			}
			subElement["children"] = jsonGroup.jsonElement();
		} else if (type == "class ofParameter<bool>") { // handle bool
			subElement["type"] = "bool";
			subElement["value"] = this->getBool(i).get();
			subElement["escapedName"] = this->getBool(i).getEscapedName();
		} else if (type == "class ofParameter<float>") { // handle float
			subElement["type"] = "float";
			subElement["value"] = this->getFloat(i).get();
			subElement["min"] = this->getFloat(i).getMin();
			subElement["max"] = this->getFloat(i).getMax();
			subElement["escapedName"] = this->getFloat(i).getEscapedName();
		} else if (type == "class ofParameter<int>") { // handle int
			subElement["type"] = "int";
			subElement["value"] = this->getInt(i).get();
			subElement["min"] = this->getInt(i).getMin();
			subElement["max"] = this->getInt(i).getMax();
			subElement["escapedName"] = this->getInt(i).getEscapedName();
		}  else if (type == "class ofParameter<class ofVec2f>") { // handle ofVec2f
			subElement["type"] = "ofVec2f";
			ofxJSONElement value = ofxJSONElement::Value(Json::objectValue);
			ofVec2f val = this->getVec2f(i);
			value["x"] = val.x;
			value["y"] = val.y;
			subElement["value"] = value;
			subElement["escapedName"] = this->getVec2f(i).getEscapedName();

			ofxJSONElement min = ofxJSONElement::Value(Json::objectValue);
			ofVec2f minVec = this->getVec2f(i).getMin();
			min["x"] = minVec.x;
			min["y"] = minVec.y;
			subElement["min"] = min;

			ofxJSONElement max = ofxJSONElement::Value(Json::objectValue);
			ofVec2f maxVec = this->getVec2f(i).getMax();
			max["x"] = maxVec.x;
			max["y"] = maxVec.y;
			subElement["max"] = max;

		}  else if (type == "class ofParameter<class ofColor_<unsigned char> >") { // handle ofColor
			subElement["type"] = "ofColor";
			ofColor color = this->getColor(i);
			ofxJSONElement value = ofxJSONElement::Value(Json::objectValue);
			value["r"] = color.r;
			value["g"] = color.g;
			value["b"] = color.b;
			value["a"] = color.a;
			subElement["value"] = value;
			subElement["escapedName"] = this->getColor(i).getEscapedName();

		} else {
			cout << "unrecognized type: " << this->getType(i) << endl;
			continue;
		}

		//subElement["escapedName"] = this->getEscapedName(i);
		
		children.append(subElement);
	}

	ofxJSONElement element = ofxJSONElement::Value(Json::objectValue);
	element["name"] = this->getName();
	element["type"] = "ofParameterGroup";
	element["children"] = children;
	return element;
}