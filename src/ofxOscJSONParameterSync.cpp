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
	ofxJSONElement element = syncGroupJSONElement();

	ofxOscMessage message = ofxOscMessage();
	message.setAddress("completeSettings");
	message.addStringArg(element.getRawString());

	jsonSender.sendMessage(message);
}


ofxJSONElement ofxOscJSONParameterSync::syncGroupJSONElement() {

	ofxJSONElement children = ofxJSONElement::Value(Json::arrayValue);
	for (int i = 0; i < jsonSyncGroup->size(); i++) {
		
		ofxJSONElement subElement = ofxJSONElement::Value(Json::objectValue);
		subElement["name"] = jsonSyncGroup->getName(i);

		string type = jsonSyncGroup->getType(i);
		if (type == "class ofParameterGroup") {

			subElement["type"] = "ofParameterGroup";
			subElement["escapedName"] = jsonSyncGroup->getGroup(i).getEscapedName();
			ofParameterGroup group = jsonSyncGroup->getGroup(i);

			ofJSONParameterGroup jsonGroup = ofJSONParameterGroup();
			for (int j = 0; j < group.size(); j++) {
				jsonGroup.add(group.get(j));
				jsonGroup.setName(group.getName());
			}
			subElement["children"] = jsonGroup.jsonElement();
		} else if (type == "class ofParameter<bool>") { // handle bool
			subElement["type"] = "bool";
			subElement["value"] = jsonSyncGroup->getBool(i).get();
			subElement["escapedName"] = jsonSyncGroup->getBool(i).getEscapedName();
		} else if (type == "class ofParameter<float>") { // handle float
			subElement["type"] = "float";
			subElement["value"] = jsonSyncGroup->getFloat(i).get();
			subElement["min"] = jsonSyncGroup->getFloat(i).getMin();
			subElement["max"] = jsonSyncGroup->getFloat(i).getMax();
			subElement["escapedName"] = jsonSyncGroup->getFloat(i).getEscapedName();
		} else if (type == "class ofParameter<int>") { // handle int
			subElement["type"] = "int";
			subElement["value"] = jsonSyncGroup->getInt(i).get();
			subElement["min"] = jsonSyncGroup->getInt(i).getMin();
			subElement["max"] = jsonSyncGroup->getInt(i).getMax();
			subElement["escapedName"] = jsonSyncGroup->getInt(i).getEscapedName();
		}  else if (type == "class ofParameter<class ofVec2f>") { // handle ofVec2f
			subElement["type"] = "ofVec2f";
			ofxJSONElement value = ofxJSONElement::Value(Json::objectValue);
			ofVec2f val = jsonSyncGroup->getVec2f(i);
			value["x"] = val.x;
			value["y"] = val.y;
			subElement["value"] = value;
			subElement["escapedName"] = jsonSyncGroup->getVec2f(i).getEscapedName();

			ofxJSONElement min = ofxJSONElement::Value(Json::objectValue);
			ofVec2f minVec = jsonSyncGroup->getVec2f(i).getMin();
			min["x"] = minVec.x;
			min["y"] = minVec.y;
			subElement["min"] = min;

			ofxJSONElement max = ofxJSONElement::Value(Json::objectValue);
			ofVec2f maxVec = jsonSyncGroup->getVec2f(i).getMax();
			max["x"] = maxVec.x;
			max["y"] = maxVec.y;
			subElement["max"] = max;

		}  else if (type == "class ofParameter<class ofColor_<unsigned char> >") { // handle ofColor
			subElement["type"] = "ofColor";
			ofColor color = jsonSyncGroup->getColor(i);
			ofxJSONElement value = ofxJSONElement::Value(Json::objectValue);
			value["r"] = color.r;
			value["g"] = color.g;
			value["b"] = color.b;
			value["a"] = color.a;
			subElement["value"] = value;
			subElement["escapedName"] = jsonSyncGroup->getColor(i).getEscapedName();

		} else {
			cout << "unrecognized type: " << jsonSyncGroup->getType(i) << endl;
			continue;
		}

		//subElement["escapedName"] = this->getEscapedName(i);
		
		children.append(subElement);
	}

	ofxJSONElement element = ofxJSONElement::Value(Json::objectValue);
	element["name"] = jsonSyncGroup->getName();
	element["type"] = "ofParameterGroup";
	element["children"] = children;
	return element;
}