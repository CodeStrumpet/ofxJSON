#pragma once
#include "ofxOscParameterSync.h"
#include "ofxOscSender.h"
#include "ofxJSONElement.h"

class ofxOscJSONParameterSync :
	public ofxOscParameterSync
{
public:
	ofxOscJSONParameterSync(void);
	~ofxOscJSONParameterSync(void);

	void setup(ofParameterGroup & group, int localPort, string host, int remotePort);

	void sendFullParameterSet();
	ofxJSONElement parameterGroupJSONElement(ofParameterGroup *group);

	ofxOscSender jsonSender;
	ofParameterGroup * jsonSyncGroup; 
};

