/*
 *  VUMeter.h
 *  juce_application
 *
 *  Created by Jordan Hochenbaum on 5/16/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _VUMETER_H_
#define _VUMETER_H_

class VUMeter   :   public Component, public ChangeBroadcaster {
private:
    GradientBrush* brsGreen;
    float value;
	
public:
	
	VUMeter() : Component( "Slider Field Component" ){
		value = .5f;
		brsGreen =  new GradientBrush(Colour(0,200,0),0,0,Colour(200,200,0),0,100,false);
		
	}
	
	
	~VUMeter(){
		delete brsGreen;
		deleteAllChildren ();
	}
	
	
	void paint (Graphics& g){
		//background
		g.setColour(Colour(0,0,0));
		g.fillRect(0,0,getWidth(),getHeight());
		
		
		g.setBrush(brsGreen);
		g.fillRect(0,int(getHeight() * (1-getValue())),getWidth(),getHeight());
		
		//bevel outline for the entire draw area
		g.drawBevel(0, 0, getWidth(), getHeight(), 2, Colour(100,100,100), Colours::white, 0);
	}
	
	float getValue(){
		return value;
	}
	
	void setValue(float v){
		value = jmin(jmax(v , .0f) , 1.0f);
	}
	
};




#endif