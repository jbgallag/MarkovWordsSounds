#pragma once

#include "ofMain.h"
#include "ofxAubio.h"
#include "ofxGui.h"
#include "MarkovWordChain.hpp"
#include "screenWord.h"

class ofApp : public ofBaseApp{

    public:
        void setup();
        void exit();

        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);

        void audioIn(float * input, int bufferSize, int nChannels);
        void audioOut();

        void onsetEvent(float & time);
        void beatEvent(float & time);
    
        void DrawRandomText(float ons, float lpitch);
        void SetScreenWord(float ons);
        void SetScreenWordTicker(float ons);
        void SetScreenWordCascade(float lpitch);
        void SetScreenWordFlash();
        void SetScreenWordParagraphMulti(float ons);
        void SetScreenWordParagraph(float ons);
        void DrawScreenWords();
    
        void MakeColorMap();
        vector<string> arguments;
    
    ofTrueTypeFont  franklinBook144A;
       
    private:
    
    string seedWord,fileName;
    int nlevels;
    int cascadeOffset,lastPosition;
    float parBegin,parEnd,x,mpar,onsetThresh;
    int steps,itr,kit,mtype;
    uint64_t ttl;
    int cutoff;
    int totalActiveScreenWords;
    int activeScreenWordCount;
    int pXOffset,pYOffset,pXStart,pYStart,pNumLines,pNumWords,pNumWordsPerLine,pNumRand;
    int pNumLinesMax;
    int pXOffset2,pYOffset2,pXStart2,pYStart2,pNumLines2,pNumWords2,pNumWordsPerLine2;
    int pNumLinesMax2;
    int pXOffset3,pYOffset3,pXStart3,pYStart3,pNumLines3,pNumWords3,pNumWordsPerLine3;
    int pNumLinesMax3;
    
    int mode;
    bool firstRun;
    bool printText;
    MarkovWordChain *myMarkovChain;
    vector<screenWord *>ScreenWords;
    typedef vector<screenWord *>::iterator scwItr;
    map<int,ofColor> myColorMap;
    vector<scwItr>EraseScreenWords;
        ofxAubioOnset onset;
        ofxAubioPitch pitch;
        ofxAubioBeat beat;
        ofxAubioMelBands bands;

        ofxPanel pitchGui;
        ofxFloatSlider midiPitch;
        ofxFloatSlider pitchConfidence;

        ofxPanel beatGui;
        bool gotBeat;
        ofxFloatSlider bpm;

        ofxPanel onsetGui;
        bool gotOnset;
        ofxFloatSlider onsetThreshold;
        ofxFloatSlider onsetNovelty;
        ofxFloatSlider onsetThresholdedNovelty;

        ofxPanel bandsGui;
        ofPolyline bandPlot;
    
};
