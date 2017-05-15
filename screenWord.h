//
//  screenWord.h
//  markovWordsChaos
//
//  Created by Jonathan B Gallagher on 5/5/17.
//  Copyright © 2017 Jonathan B Gallagher. All rights reserved.
//

#ifndef screenWord_h
#define screenWord_h
#include "ofMain.h"

class screenWord
{
public:
    screenWord(string w, ofVec2f p, ofColor c, uint64_t ttl);
    void SetPosition(ofVec2f p);
    ofVec2f GetPosition();
    void SetColor(ofColor c);
    ofColor GetColor();
    void SetWord(string w);
    string GetWord();
    void SetTTL(uint64_t ms);
    uint64_t GetTTL();
    void SetStartTime(uint64_t ms);
    uint64_t GetStartTime();
    void SetCurrentTime(uint64_t ms);
    uint64_t GetCurrentTime();
    void AnimateOpacity();
    void AnimateBrightness();
    void AnimateScale();
    void SetScale(float sc);
    inline float GetScale() { return startScale;}
    inline bool GetRemove() { return remove; }
    inline void SetAnimateOp(bool op) { animateOp = op; }
    inline bool GetAnimateOp() { return animateOp;}
    inline void SetAnimateBr(bool br) { animateBr = br; }
    inline bool GetAnimateBr() { return animateBr;}

    void DrawFont();
    ofRectangle GetWordBounds();
    //fonts
    ofTrueTypeFont  franklinBook14;
    ofTrueTypeFont	verdana14;
    ofTrueTypeFont	verdana30;
    
    ofTrueTypeFont  franklinBook14A;
    ofTrueTypeFont  franklinBook144A;
    ofTrueTypeFont	verdana14A;

private:
    ofVec2f pos;
    ofColor color;
    float opacityInc;
    float brightInc;
    float scaleInc;
    float startScale;
    string word;
    uint64_t ttl;
    uint64_t startTime;
    uint64_t currentTime;
    bool remove,animateOp,animateBr;
};

#endif /* screenWord_h */
