#include "ofApp.h"
#include "ofEventUtils.h"

//--------------------------------------------------------------
void ofApp::setup(){
    if(arguments.size() != 20) {
        printf("Usage: ./markovWords <file> <seedWord> <nLevels> <parBegin> <parEnd> <x> <steps> <itr> <kit> <map> <mtype> <onsetThresh> <ttl> <cutoff> <px> <py> <pnr> <pnl>\n");
        std::exit(0);
    }
    
    fileName = arguments.at(1);
    seedWord = arguments.at(2);
    nlevels = atoi(arguments.at(3).c_str());
    parBegin = atof(arguments.at(4).c_str());
    parEnd = atof(arguments.at(5).c_str());
    x = atof(arguments.at(6).c_str());
    steps = atoi(arguments.at(7).c_str());
    itr = atoi(arguments.at(8).c_str());
    kit = atoi(arguments.at(9).c_str());
    mpar = atof(arguments.at(10).c_str());
    mtype = atoi(arguments.at(11).c_str());
    onsetThresh = atof(arguments.at(12).c_str());
    ttl = atoi(arguments.at(13).c_str());
    cutoff = atoi(arguments.at(14).c_str());
    pXStart = atoi(arguments.at(15).c_str());
    pYStart = atoi(arguments.at(16).c_str());
    pNumRand = atoi(arguments.at(17).c_str());
    pNumLinesMax = atoi(arguments.at(18).c_str());
    mode = atoi(arguments.at(19).c_str());
    myMarkovChain = new MarkovWordChain(fileName,seedWord,nlevels,parBegin,parEnd,x,steps,itr,kit,mpar,mtype);
    // set the size of the window
    ofSetWindowShape(1320,1085);

    activeScreenWordCount = 0;
    totalActiveScreenWords = 0;
    int nOutputs = 2;
    int nInputs = 2;
    //int sampleRate = 44100;
    //int bufferSize = 256;
    //int nBuffers = 4;

    // setup onset object
    onset.setup();
    //onset.setup("mkl", 2 * bufferSize, bufferSize, sampleRate);
    // listen to onset event
    ofAddListener(onset.gotOnset, this, &ofApp::onsetEvent);

    // setup pitch object
    pitch.setup();
    //pitch.setup("yinfft", 8 * bufferSize, bufferSize, sampleRate);

    // setup beat object
    beat.setup();
    //beat.setup("default", 2 * bufferSize, bufferSize, samplerate);
    // listen to beat event
    ofAddListener(beat.gotBeat, this, &ofApp::beatEvent);

    // setup mel bands object
    bands.setup();

    ofSoundStreamSetup(nOutputs, nInputs, this);
    //ofSoundStreamSetup(nOutputs, nInputs, sampleRate, bufferSize, nBuffers);
    //ofSoundStreamListDevices();

    
    franklinBook144A.load("frabk.ttf", 24, false);
    franklinBook144A.setLineHeight(28.0f);
    franklinBook144A.setLetterSpacing(1.037);

    pNumLines = 0;
    //pNumLinesMax = 5;
    pNumWords = 0;
    pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
    //pXStart = (int)ofGetWindowWidth()/2 - 35;
    //pYStart = (int)ofGetWindowHeight()*0.33;
    //pXStart = 250;
    //pYStart = 250;
    pXOffset = 0;
    pYOffset = 0;
    
    pNumLines2 = 0;
    pNumLinesMax2 = pNumLinesMax;
    pNumWords2 = 0;
    pNumWordsPerLine2 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
    pXStart2 = (ofGetWindowWidth()/3.0)*2.0 - pXStart;
    pYStart2 = pYStart;
    pXOffset2 = 0;
    pYOffset2 = 0;
    
    pNumLines3 = 0;
    pNumLinesMax3 = pNumLinesMax;
    pNumWords3 = 0;
    pNumWordsPerLine3 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
    pXStart3 = pXStart + (ofGetWindowWidth()/3.0);
    pYStart3 = pYStart + (ofGetWindowHeight()/2.0);
    pXOffset3 = 0;
    pYOffset3 = 0;


    firstRun = true;
    printText = false;
    printf("FINISHED SETUP!\n");

}

void ofApp::exit(){
    ofSoundStreamStop();
    ofSoundStreamClose();
}

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // compute onset detection
    onset.audioIn(input, bufferSize, nChannels);
    // compute pitch detection
    pitch.audioIn(input, bufferSize, nChannels);
    // compute beat location
    beat.audioIn(input, bufferSize, nChannels);
    // compute bands
    bands.audioIn(input, bufferSize, nChannels);
}

void audioOut(){
}

//--------------------------------------------------------------
void ofApp::update(){
    //onset.setThreshold(onsetThreshold);
}

void ofApp::DrawRandomText(float ons, float lpitch)
{
    myMarkovChain->GetNextProbChain();
    myMarkovChain->GetNextWordInProbChain();
    if(mode == 0)
        SetScreenWord(lpitch);
    if(mode == 1)
        SetScreenWordTicker(lpitch);
    if(mode == 2)
        SetScreenWordParagraph(lpitch);
    if(mode == 3)
        SetScreenWordParagraphMulti(lpitch);
    
}

void ofApp::SetScreenWord(float ons)
{
    ofVec2f pos;
    ofColor clr;
    pos.x = floor(((double) rand()/RAND_MAX)*(ofGetWindowWidth()));
    pos.y = floor(((double) rand()/RAND_MAX)*(ofGetWindowHeight()));
    int idx = (int)floor(ons);
    if(idx > cutoff)
        idx = cutoff;
    clr.setHsb(idx/(cutoff/255),128,255,255);
    screenWord *scWord = new screenWord(myMarkovChain->GetFoundWord(),pos,clr,ttl);
    scWord->SetStartTime(ofGetElapsedTimeMillis());
    scWord->SetAnimateOp(true);
    scWord->SetAnimateBr(true);
   // printf("ONSET: %f\n",log(ons));
   // if(scWord->GetScale() == 1.0)
    //    scWord->SetScale(log(ons));
    ScreenWords.push_back(scWord);
}

void ofApp::SetScreenWordTicker(float ons)
{
    ofVec2f pos;
    ofColor clr;
    pos.x = ofGetWindowWidth();
    pos.y = ofGetWindowHeight()/2;
    clr = ofColor(255,255,255,255);
    
    screenWord *scWord;
    if(firstRun) {
        scWord = new screenWord(seedWord,pos,clr,ttl);
        firstRun = false;
    } else {
        scWord = new screenWord(myMarkovChain->GetFoundWord(),pos,clr,ttl);
    }
    scWord->SetAnimateOp(true);
    scWord->SetStartTime(ofGetElapsedTimeMillis());
    //scWord->SetScale(log(ons)*0.5);
    ofRectangle bounds = scWord->GetWordBounds();
    int newOffset = (int)(bounds.getMaxX()-bounds.getMinX());
    pos.x = ofGetWindowWidth() - (newOffset+12);
    pos.y = ofGetWindowHeight()/2;
    scWord->SetPosition(pos);
    if(ScreenWords.size() != 0) {
        for(vector<screenWord *>::iterator itV = ScreenWords.begin(); itV != ScreenWords.end(); itV++) {
            screenWord *scWord = *itV;
            ofVec2f newPos;
            newPos.x = scWord->GetPosition().x - (newOffset+12);
            newPos.y = scWord->GetPosition().y;
            scWord->SetPosition(newPos);
        }
    }
    ScreenWords.push_back(scWord);
    

}

void ofApp::SetScreenWordParagraph(float ons)
{
    int newOffset;
    ofVec2f pos;
    ofColor clr;
        if(pNumLines < pNumLinesMax) {
            if(pYOffset == 0) {
                pos.x = pXStart;
                pos.y = pYStart;
                pXOffset = pXStart;
                pYOffset = pYStart;
                pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
            } else {
                pos.x = pXOffset;
                pos.y = pYOffset;
            }
            int idx = (int)floor(ons);
            if(idx > cutoff)
                idx = cutoff;
            //clr.setHsb(idx/(cutoff/127),200,255,255);

            clr = ofColor(255,255,255,255);
            screenWord *scWord;
            if(firstRun) {
                scWord = new screenWord(seedWord,pos,clr,ttl);
                firstRun = false;
            } else {
                scWord = new screenWord(myMarkovChain->GetFoundWord(),pos,clr,ttl);
            }

            scWord->SetAnimateOp(true);
            scWord->SetAnimateBr(true);
            scWord->SetStartTime(ofGetElapsedTimeMillis());
            ofRectangle bounds = scWord->GetWordBounds();
            newOffset = (int)(bounds.getMaxX()-bounds.getMinX());
            ScreenWords.push_back(scWord);
            pNumWords++;
            if(pNumWords > pNumWordsPerLine) {
                pYOffset = pYOffset + scWord->franklinBook144A.getSize();
                pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
                pXOffset = pXStart;
                pNumWords = 0;
                pNumLines++;
            } else {
                pXOffset = pXOffset + (newOffset) + (int)(scWord->franklinBook144A.getLetterSpacing())*7.5;
            }
        } else {
            pNumLines = 0;
            pNumWords = 0;
            pYOffset = 0;
            pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
        }

}
void ofApp::SetScreenWordParagraphMulti(float ons)
{
    int newOffset,newOffset2,newOffset3;
    ofVec2f pos,pos2,pos3;
    ofColor clr,clr2,clr3;
    if(ons > 50.0 && ons < 1000.0) {
     if(pNumLines < pNumLinesMax) {
        if(pYOffset == 0) {
            pos.x = pXStart;
            pos.y = pYStart;
            pXOffset = pXStart;
            pYOffset = pYStart;
            pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
        } else {
            pos.x = pXOffset;
            pos.y = pYOffset;
        }
         int idx = (int)floor(ons);
         
         clr.setHsb(253,idx/(1000.0/255),128,255);

        screenWord *scWord;
         if(firstRun) {
             scWord = new screenWord(seedWord,pos,clr,ttl);
             firstRun = false;
         } else {
             scWord = new screenWord(myMarkovChain->GetFoundWord(),pos,clr,ttl);
         }

        scWord->SetAnimateOp(true);
        scWord->SetStartTime(ofGetElapsedTimeMillis());
        ofRectangle bounds = scWord->GetWordBounds();
        newOffset = (int)(bounds.getMaxX()-bounds.getMinX());
        ScreenWords.push_back(scWord);
        pNumWords++;
        if(pNumWords > pNumWordsPerLine) {
            pYOffset = pYOffset + scWord->franklinBook144A.getSize();
            pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
            pXOffset = pXStart;
            pNumWords = 0;
            pNumLines++;
        } else {
            pXOffset = pXOffset + (newOffset) + (int)(scWord->franklinBook144A.getLetterSpacing())*7.5;
        }
     } else {
        pNumLines = 0;
        pNumWords = 0;
        pYOffset = 0;
        pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
     }
    }//end freq < 1000
    if(ons > 1050.0 && ons < 1750.0) {
     if(pNumLines2 < pNumLinesMax2) {
        if(pYOffset2 == 0) {
            pos2.x = pXStart2;
            pos2.y = pYStart2;
            pXOffset2 = pXStart2;
            pYOffset2 = pYStart2;
            pNumWordsPerLine2 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
        } else {
            pos2.x = pXOffset2;
            pos2.y = pYOffset2;
        }
         int idx = (int)floor(ons);
         
         clr2.setHsb(137,idx/(1000.0/127),128,255);

        screenWord *scWord2 = new screenWord(myMarkovChain->GetFoundWord(),pos2,clr2,ttl);
        scWord2->SetAnimateOp(true);
        scWord2->SetStartTime(ofGetElapsedTimeMillis());
        ofRectangle bounds2 = scWord2->GetWordBounds();
        newOffset2 = (int)(bounds2.getMaxX()-bounds2.getMinX());
        ScreenWords.push_back(scWord2);
        pNumWords2++;
        if(pNumWords2 > pNumWordsPerLine2) {
            pYOffset2 = pYOffset2 + scWord2->franklinBook144A.getSize();
            pNumWordsPerLine2 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
            pXOffset2 = pXStart2;
            pNumWords2 = 0;
            pNumLines2++;
        } else {
            pXOffset2 = pXOffset2 + (newOffset2) + (int)(scWord2->franklinBook144A.getLetterSpacing())*7.5;
        }
     } else {
        pNumLines2 = 0;
        pNumWords2 = 0;
        pYOffset2 = 0;
        pNumWordsPerLine2 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
     }
    } //end if 1000 < freq < 2000
    if(ons > 1800.0) {
     if(pNumLines3 < pNumLinesMax3) {
        if(pYOffset3 == 0) {
            pos3.x = pXStart3;
            pos3.y = pYStart3;
            pXOffset3 = pXStart3;
            pYOffset3 = pYStart3;
            pNumWordsPerLine3 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
        } else {
            pos3.x = pXOffset3;
            pos3.y = pYOffset3;
        }
         int idx = (int)floor(ons);
         
         clr3.setHsb(37,idx/(1000.0/127),128,255);

        screenWord *scWord3 = new screenWord(myMarkovChain->GetFoundWord(),pos3,clr3,ttl);
        scWord3->SetAnimateOp(true);
        scWord3->SetStartTime(ofGetElapsedTimeMillis());
        ofRectangle bounds3 = scWord3->GetWordBounds();
        newOffset3 = (int)(bounds3.getMaxX()-bounds3.getMinX());
        ScreenWords.push_back(scWord3);
        pNumWords3++;
        if(pNumWords3 > pNumWordsPerLine3) {
            pYOffset3 = pYOffset3 + scWord3->franklinBook144A.getSize();
            pNumWordsPerLine3 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
            pXOffset3 = pXStart3;
            pNumWords3 = 0;
            pNumLines3++;
        } else {
            pXOffset3 = pXOffset3 + (newOffset3) + (int)(scWord3->franklinBook144A.getLetterSpacing())*7.5;
        }
     } else {
        pNumLines3 = 0;
        pNumWords3 = 0;
        pYOffset3 = 0;
        pNumWordsPerLine3 = (int)floor(((double)rand())/RAND_MAX * pNumRand) + pNumRand;
     }
    }//end if frew > 2000

    
    
}

void ofApp::SetScreenWordCascade(float lpitch)
{
    int newOffset;
    ofVec2f pos;
    ofColor clr;
    if(pYOffset == 0) {
        pos.x = pXStart;
        pos.y = pYStart;
        pXOffset = pXStart;
        pYOffset = pYStart;
        pNumWordsPerLine = (int)floor(((double)rand())/RAND_MAX * 3) + 3;
    } else {
        pos.x = pXOffset;
        pos.y = pYOffset;
    }
    clr = ofColor(55,55,255,255);
    screenWord *scWord;
    if(firstRun) {
        scWord = new screenWord(seedWord,pos,clr,ttl);
        firstRun = false;
    } else {
        scWord = new screenWord(myMarkovChain->GetFoundWord(),pos,clr,ttl);
    }

    ofRectangle bounds = scWord->GetWordBounds();
    newOffset = (int)(bounds.getMaxX()-bounds.getMinX());
    
    pXOffset = pXOffset + (newOffset) + (int)(scWord->franklinBook144A.getLetterSpacing())*7.5;
    if(pXOffset >=  ofGetWindowWidth()) {
        pXOffset = 0;
        pYOffset = pYOffset + scWord->franklinBook144A.getSize();
        if(pYOffset >= ofGetWindowHeight() - scWord->franklinBook144A.getSize()) {
            pYOffset = pYOffset - scWord->franklinBook144A.getSize();
            if(ScreenWords.size() != 0) {
                for(vector<screenWord *>::iterator itV = ScreenWords.begin(); itV != ScreenWords.end(); itV++) {
                    screenWord *scWord = *itV;
                    ofVec2f newPos;
                    //newPos.x = scWord->GetPosition().x - (newOffset+12);
                    newPos.y = scWord->GetPosition().y - scWord->franklinBook144A.getSize();
                    scWord->SetPosition(newPos);
                }
            }

        }
    }
    
    ScreenWords.push_back(scWord);
}

void ofApp::DrawScreenWords()
{
    float nextX,nextY;
    for(vector<screenWord *>::iterator itV = ScreenWords.begin(); itV != ScreenWords.end(); itV++) {
        screenWord *scWord = *itV;
        //ofSetColor(scWord->GetColor());
        //scWord->AnimateOpacity();
        //scWord->AnimateScale();
        //franklinBook144A.drawString(scWord->GetWord(), scWord->GetPosition().x, scWord->GetPosition().y);
        scWord->DrawFont();
        if(scWord->GetRemove())
            EraseScreenWords.push_back(itV);
    }
    for(vector<scwItr>::iterator itV = EraseScreenWords.begin(); itV != EraseScreenWords.end(); itV++) {
        ScreenWords.erase(*itV);
    }
    EraseScreenWords.clear();
}
    
//--------------------------------------------------------------
void ofApp::draw(){
    // update beat info
    ofSetBackgroundColor(ofColor::black);
    if (gotBeat) {
        //ofSetColor(ofColor::green);
        //ofRect(90,150,50,50);
        gotBeat = false;
    }

    // update onset info
    if (gotOnset) {
        //ofSetColor(ofColor::red);
        //ofRect(250 + 90,150,50,50);
        if(printText)
            DrawRandomText(onset.novelty,pitch.latestPitch);
        gotOnset = false;
        
    }
    onsetNovelty = onset.novelty;
 
    if (pitch.latestPitch) midiPitch = pitch.latestPitch;
    bpm = beat.bpm;

    // draw
     DrawScreenWords();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 'g')
        printText = true;
    if(key == 's')
        printText = false;
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

//----
void ofApp::onsetEvent(float & time) {
    //ofLog() << "got onset at " << time << " s";
    if(onset.novelty > onsetThresh) {
        gotOnset = true;
    }
}

//----
void ofApp::beatEvent(float & time) {
    //ofLog() << "got beat at " << time << " s";
    gotBeat = true;
}
