#pragma once
#include "ofxOscParameterSync.h"
#include "ofJSONParameterGroup.h"
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
	ofxJSONElement syncGroupJSONElement();

	ofxOscSender jsonSender;
	ofParameterGroup * jsonSyncGroup; 
};

