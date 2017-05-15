//
//  screenWord.cpp
//  markovWordsChaos
//
//  Created by Jonathan B Gallagher on 5/5/17.
//  Copyright © 2017 Jonathan B Gallagher. All rights reserved.
//

#include <stdio.h>
#include "screenWord.h"

screenWord::screenWord(string w, ofVec2f p, ofColor c, uint64_t ttl)
{
    SetWord(w);
    SetPosition(p);
    SetColor(c);
    SetTTL(ttl);
    remove = false;
    animateOp = false;
    //fonts
    ofTrueTypeFont::setGlobalDpi(72);
    
    verdana14.load("verdana.ttf", 14, true, true);
    verdana14.setLineHeight(18.0f);
    verdana14.setLetterSpacing(1.037);
    
    verdana30.load("verdana.ttf", 30, true, true);
    verdana30.setLineHeight(34.0f);
    verdana30.setLetterSpacing(1.035);
    
    verdana14A.load("verdana.ttf", 14, false);
    verdana14A.setLineHeight(18.0f);
    verdana14A.setLetterSpacing(1.037);
    
    franklinBook14.load("frabk.ttf", 14);
    franklinBook14.setLineHeight(18.0f);
    franklinBook14.setLetterSpacing(1.037);
    
    franklinBook14A.load("frabk.ttf", 14, false);
    franklinBook14A.setLineHeight(18.0f);
    franklinBook14A.setLetterSpacing(1.037);
    
    franklinBook144A.load("frabk.ttf", 24, false);
    franklinBook144A.setLineHeight(28.0f);
    franklinBook144A.setLetterSpacing(1.037);
    
    startScale = 1.0;
    opacityInc = 255.0/(float)GetTTL();
    brightInc = 127.0/1000.0;
    scaleInc = 0.0;
}

void screenWord::SetColor(ofColor c)
{
    color = c;
}

ofColor screenWord::GetColor()
{
    return color;
}

void screenWord::SetPosition(ofVec2f p)
{
    pos = p;
}

ofVec2f screenWord::GetPosition()
{
    return pos;
}

void screenWord::SetWord(string w)
{
    word = w;
}

string screenWord::GetWord()
{
    return word;
}

void screenWord::SetTTL(uint64_t tms)
{
    ttl = tms;
}

uint64_t screenWord::GetTTL()
{
    return ttl;
}

void screenWord::SetStartTime(uint64_t ms)
{
    startTime = ms;
}

uint64_t screenWord::GetStartTime()
{
    return startTime;
}

void screenWord::SetCurrentTime(uint64_t ms)
{
    currentTime = ms;
}

uint64_t screenWord::GetCurrentTime()
{
    return currentTime;
}

void screenWord::AnimateOpacity()
{
    uint64_t diff;
    int newOpacity;
    diff = ofGetElapsedTimeMillis() - GetStartTime();
    
    newOpacity = 255 - (int)((float)diff*opacityInc);
    ofColor myColor = GetColor();
    myColor.a = newOpacity;
    SetColor(myColor);
    if(newOpacity <= 0) {
        remove = true;
        //animateOp = false;
    }
}

void screenWord::AnimateBrightness()
{
    uint64_t diff;
    int newBright;
    diff = ofGetElapsedTimeMillis() - GetStartTime();
    
    newBright = 255 - (int)((float)diff*brightInc);
    ofColor myColor = GetColor();
    myColor.setBrightness(newBright);
    SetColor(myColor);
    if(newBright <= 128) {
        animateBr = false;
        //animateOp = false;
    }
}


void screenWord::SetScale(float sc)
{
   // ofPushMatrix();
     //   ofScale(sc,sc,1.0);
   // ofPopMatrix();
    startScale = sc;
    scaleInc = sc/(float)ttl;

}

void screenWord::AnimateScale()
{
    uint64_t diff;
    float newScale;
    diff = ofGetElapsedTimeMillis() - GetStartTime();
    
    newScale = startScale - (int)((float)diff*scaleInc);
    ofPushMatrix();
        ofScale(newScale,newScale,1.0);
    ofPopMatrix();
}

void screenWord::DrawFont()
{
    uint64_t diff;
    float newScale;
    diff = ofGetElapsedTimeMillis() - GetStartTime();
    newScale = startScale - (int)((float)diff*scaleInc);
    if(newScale <= 1.0) {
        if(GetAnimateOp()) {
            AnimateOpacity();
        }
        if(GetAnimateBr())
            AnimateBrightness();
        ofSetColor(GetColor());
        if(!GetRemove())
            franklinBook144A.drawString(GetWord(), GetPosition().x, GetPosition().y);
    } else {
        ofPushMatrix();
        ofScale(newScale, newScale,1.0);
        AnimateOpacity();
        ofSetColor(GetColor());
        franklinBook144A.drawString(GetWord(), GetPosition().x, GetPosition().y);
        ofPopMatrix();
    }

}

ofRectangle screenWord::GetWordBounds()
{
    ofRectangle bounds;
    bounds = franklinBook144A.getStringBoundingBox(GetWord(), 0, 0);
    return bounds;
}