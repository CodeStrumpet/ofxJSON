#pragma once
#include "ofParameterGroup.h"
#include "ofxJSONElement.h"
class ofJSONParameterGroup :
	public ofParameterGroup
{
public:
	ofJSONParameterGroup(void);
	~ofJSONParameterGroup(void);

	ofxJSONElement jsonElement();
};

