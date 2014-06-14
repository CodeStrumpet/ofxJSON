#include "ofxOscJSONParameterSync.h"


ofxOscJSONParameterSync::ofxOscJSONParameterSync(void)
{
}


ofxOscJSONParameterSync::~ofxOscJSONParameterSync(void)
{
}

void ofxOscJSONParameterSync::setup(ofParameterGroup & group, int localPort, string host, int remotePort){
	jsonSender.setup(host,remotePort);

	jsonSyncGroup = &group;
	ofxOscParameterSync::setup(group, localPort, host, remotePort);
}

void ofxOscJSONParameterSync::sendFullParameterSet() {

	ofxJSONElement element = parameterGroupJSONElement(jsonSyncGroup);

	ofxOscMessage message = ofxOscMessage();
	message.setAddress("/" + jsonSyncGroup->getEscapedName());
	message.addStringArg(element.getRawString());

	jsonSender.sendMessage(message);
}


ofxJSONElement ofxOscJSONParameterSync::parameterGroupJSONElement(ofParameterGroup *group) {

	ofxJSONElement children = ofxJSONElement::Value(Json::arrayValue);
	for (int i = 0; i < group->size(); i++) {
		
		ofxJSONElement subElement = ofxJSONElement::Value(Json::objectValue);
		subElement["name"] = group->getName(i);

		string type = group->getType(i);
		if (type == "class ofParameterGroup") {

			// call this function recursively to compose the json for a nested parameterGroup
			subElement = parameterGroupJSONElement( &(group->getGroup(i)));
			
		} else if (type == "class ofParameter<bool>") { // handle bool
			subElement["type"] = "bool";
			subElement["value"] = group->getBool(i).get();
			subElement["escapedName"] = group->getBool(i).getEscapedName();
		} else if (type == "class ofParameter<float>") { // handle float
			subElement["type"] = "float";
			subElement["value"] = group->getFloat(i).get();
			subElement["min"] = group->getFloat(i).getMin();
			subElement["max"] = group->getFloat(i).getMax();
			subElement["escapedName"] = group->getFloat(i).getEscapedName();
		} else if (type == "class ofParameter<int>") { // handle int
			subElement["type"] = "int";
			subElement["value"] = group->getInt(i).get();
			subElement["min"] = group->getInt(i).getMin();
			subElement["max"] = group->getInt(i).getMax();
			subElement["escapedName"] = group->getInt(i).getEscapedName();
		}  else if (type == "class ofParameter<class ofVec2f>") { // handle ofVec2f
			subElement["type"] = "ofVec2f";
			ofxJSONElement value = ofxJSONElement::Value(Json::objectValue);
			ofVec2f val = group->getVec2f(i);
			value["x"] = val.x;
			value["y"] = val.y;
			subElement["value"] = value;
			subElement["escapedName"] = group->getVec2f(i).getEscapedName();

			ofxJSONElement min = ofxJSONElement::Value(Json::objectValue);
			ofVec2f minVec = group->getVec2f(i).getMin();
			min["x"] = minVec.x;
			min["y"] = minVec.y;
			subElement["min"] = min;

			ofxJSONElement max = ofxJSONElement::Value(Json::objectValue);
			ofVec2f maxVec = group->getVec2f(i).getMax();
			max["x"] = maxVec.x;
			max["y"] = maxVec.y;
			subElement["max"] = max;

		}  else if (type == "class ofParameter<class ofColor_<unsigned char> >") { // handle ofColor
			subElement["type"] = "ofColor";
			ofColor color = group->getColor(i);
			ofxJSONElement value = ofxJSONElement::Value(Json::objectValue);
			value["r"] = color.r;
			value["g"] = color.g;
			value["b"] = color.b;
			value["a"] = color.a;
			subElement["value"] = value;
			subElement["escapedName"] = group->getColor(i).getEscapedName();

		} else {
			cout << "unrecognized type: " << group->getType(i) << endl;
			continue;
		}
		
		children.append(subElement);
	}

	ofxJSONElement element = ofxJSONElement::Value(Json::objectValue);
	element["name"] = group->getName();
	element["escapedName"] = group->getEscapedName();
	element["type"] = "ofParameterGroup";
	element["children"] = children;
	return element;
}