//
//  MarkovWordChain.hpp
//  example_aubioDemo
//
//  Created by Jonathan B Gallagher on 5/6/17.
//
//

#ifndef MarkovWordChain_hpp
#define MarkovWordChain_hpp

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <map>
#include <cstring>
#include <locale>
#include "chaosMap.h"

using namespace std;

class MarkovWordChain
{
public:
    MarkovWordChain(string fname, string seedWord, int numLevels,float parBegin, float parEnd, float x,int steps,int itr,int kit,float mpar,int mt);
    void CreateMarkovWordChain(string fname);
    void ComputeProbabilities();
    void LoadChaosMap(float parBegin, float parEnd, float x,int steps,int itr,int kit,float mpar,mapType mpType);
    void setMyMtype(int mt);
    void setUseChaosMap(bool uch);
    
    string GetFoundWord();
    string GetFirstWord();
    
    void GetNextProbChain();
    void GetNextWordInProbChain();
    
private:
    map<string,int> uniqueWordMap;
    
    typedef map<string,float> wordMap;
    typedef map<float,string> rWordMap;
    typedef map<string,wordMap> markovChain;
    typedef map<string,rWordMap> markovRChain;
    
    markovChain myMarkovChain;
    markovRChain myProbChain;
    markovChain myMarkovChainActive;
    int nlevels;
    markovChain::iterator itMrk;
    markovRChain::iterator itRMrk;
    markovChain::iterator itMrkActive;
    //chaos stuff
    ChaosMap *myMap;
    mapType  myMtype;
    float parBegin,parEnd,x,mpar;
    int steps,itr,kit,mtype;
    bool useChaosMap,firstRun;
    logMap::iterator parItr;
    vector<double>::iterator probItr;
    
    //found word
    string foundWord;
    string firstWord;
  
    
};
#endif /* MarkovWordChain_hpp */
