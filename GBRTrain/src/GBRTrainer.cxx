#include "../interface/GBRTrainer.h"
#include "../interface/GBREvent.h"
#include "CondFormats/EgammaObjects/interface/GBRForest.h"
#include "TTree.h"
#include "TTreeFormula.h"
#include <assert.h>


//_______________________________________________________________________
GBRTrainer::GBRTrainer() : 
  fTree(0),
  fMinEvents(2000),
  fShrinkage(0.1),
  fNQuantiles(std::numeric_limits<unsigned short>::max()+1),
  fNBinsMax(128),
  fTransitionQuantile(0.7),
  sepgains(0),
  ws(0)
{

}

//_______________________________________________________________________
GBRTrainer::~GBRTrainer() 
{
  fTree = 0;

  //clear arrays
  if (sepgains) {
    delete[] sepgains;
    delete[] cutvals;
    delete[] nlefts;
    delete[] nrights;
    delete[] sumwlefts;
    delete[] sumwrights;
    delete[] bestbins;  
  }
  
  if (ws) {

    for (unsigned int ivar=0; ivar<fInputVars.size(); ++ivar) {
      delete[] ws[ivar];
      delete[] ns[ivar];
      delete[] tgts[ivar];
      delete[] tgt2s[ivar];
      
      delete[] sumws[ivar];
      delete[] sumns[ivar];
      delete[] sumtgts[ivar];
      delete[] sumtgt2s[ivar];
      delete[] varvals[ivar];   
      delete[] bsepgains[ivar];
      
      delete[] quants[ivar];
      delete[] bins[ivar];
    }
    
    delete[] ws;
    delete[] ns;
    delete[] tgts;
    delete[] tgt2s;
    
    delete[] sumws;
    delete[] sumns;
    delete[] sumtgts;
    delete[] sumtgt2s;
    delete[] varvals;  
    delete[] bsepgains;
    
    delete[] quants;
    delete[] bins;
  }
  
}

//_______________________________________________________________________
const GBRForest *GBRTrainer::TrainForest(int ntrees)
{
  
  const int nvars = fInputVars.size();
  
  
  //initialize TTreeFormulas to read variables from TTree
  std::vector<TTreeFormula*> inputforms;
  for (std::vector<std::string>::const_iterator it = fInputVars.begin(); 
       it != fInputVars.end(); ++it) {
    inputforms.push_back(new TTreeFormula(it->c_str(),it->c_str(),fTree));
  }
  
  TTreeFormula targetform(fTargetVar.c_str(),fTargetVar.c_str(),fTree);
  TTreeFormula cutform(fTrainingCut.c_str(),fTrainingCut.c_str(),fTree);
  
  
  Long64_t nev = 0;  
  
  //loop over tree to count training events with non-zero weight;
  for (Long64_t iev=0; iev<fTree->GetEntries(); ++iev) {
    fTree->LoadTree(iev);
    if (cutform.EvalInstance()!=0.) {
      ++nev;
    }
  }
  
  printf("nev = %i, nvar = %i\n",int(nev),nvars);

  //initialize arrays
  
  sepgains = new float[nvars];
  cutvals = new float[nvars];
  nlefts = new int[nvars];
  nrights = new int[nvars];
  sumwlefts = new float[nvars];
  sumwrights = new float[nvars];
  bestbins = new int[nvars];

  ws = new float*[nvars];
  ns = new int*[nvars];
  tgts = new float*[nvars];
  tgt2s = new float*[nvars];  
  sumws = new float*[nvars];
  sumns = new int*[nvars];
  sumtgts = new float*[nvars];
  sumtgt2s = new float*[nvars];
  varvals = new float*[nvars];    
  bsepgains = new float*[nvars];
  
  quants = new int*[nvars];
  bins = new int*[nvars];
  
  for (int ivar=0; ivar<nvars; ++ivar) {
    ws[ivar] = new float[fNBinsMax];
    ns[ivar] = new int[fNBinsMax];
    tgts[ivar] = new float[fNBinsMax];
    tgt2s[ivar] = new float[fNBinsMax];  
    sumws[ivar] = new float[fNBinsMax];
    sumns[ivar] = new int[fNBinsMax];
    sumtgts[ivar] = new float[fNBinsMax];
    sumtgt2s[ivar] = new float[fNBinsMax];
    varvals[ivar] = new float[fNBinsMax];  
    bsepgains[ivar] = new float[fNBinsMax];      
    
    quants[ivar] = new int[nev];
    bins[ivar] = new int[nev];
  }
    
  
  std::vector<GBREvent*> evts;
  evts.reserve(nev);
  
  double sumw = 0.;
  
  //loop over tree to fill arrays and event vector
  for (Long64_t iev=0; iev<fTree->GetEntries(); ++iev) {
    fTree->LoadTree(iev);
    
    float weight = cutform.EvalInstance();
    
    if (weight==0.) continue; //skip events with 0 weight
    
    sumw += weight;
    
    evts.push_back(new GBREvent(nvars));
    GBREvent *evt = evts.back();
    evt->SetWeight(weight);
    evt->SetTarget(targetform.EvalInstance());
    
    //printf("target = %5f\n",targetform.EvalInstance());
    
    for (int i=0; i<nvars; ++i) {
      evt->SetVar(i,inputforms[i]->EvalInstance());
    }

  }
  
  //map of input variable quantiles to values
  fQuantileMaps.resize(nvars, std::vector<float>(fNQuantiles));
  
  //parallelize building of quantiles for each input variable
  //(sorting of event pointer vector is cpu-intensive)
#pragma omp parallel for
  for (int ivar=0; ivar<nvars; ++ivar) {
    printf("sorting var %i\n",ivar);
        
    std::map<int,float,std::greater<float> > tmpmap;    
    std::vector<GBREvent*> evtsvarsort(evts.begin(),evts.end());
    
    std::sort(evtsvarsort.begin(),evtsvarsort.end(),GBRVarCMP(ivar));
    
    double sumwq = 0;
    for (unsigned int iev=0; iev<evtsvarsort.size(); ++iev) {
      sumwq += evtsvarsort[iev]->Weight();
      int quant = int((sumwq/sumw)*(fNQuantiles-1));
      float val = evtsvarsort[iev]->Var(ivar);
    
      //ensure that events with numerically identical values receive the same quantile
      if (iev>0 && val==evtsvarsort[iev-1]->Var(ivar)) quant = evtsvarsort[iev-1]->Quantile(ivar);
    
      evtsvarsort[iev]->SetQuantile(ivar,quant);
    
      tmpmap[quant] = val;
    
    }
    

    for (int i=0; i<fNQuantiles; ++i) {
      std::map<int,float,std::greater<float> >::const_iterator mit = tmpmap.lower_bound(i);
      
      float val;
      if (mit!=tmpmap.end()) val = mit->second;
      else val = -std::numeric_limits<float>::max();
      
      fQuantileMaps[ivar][i] = val;
      
      
    }
    
    
    
  }
    
  //sort events by target and compute median
  std::sort(evts.begin(),evts.end(),GBRTargetCMP());
  double medsumw = 0;
  float median = 0.;
  std::vector<GBREvent*>::const_iterator medit=evts.begin();
  while(medsumw<(0.5*sumw) && medit!=evts.end()) {
    medsumw += (*medit)->Weight();
    median = (*medit)->Target();
    
    ++medit;
  }
  
  //set initial response and recompute targets
  GBRForest *forest = new GBRForest;
  forest->SetInitialResponse(median);

  for (std::vector<GBREvent*>::iterator it=evts.begin(); it!=evts.end(); ++it) {
    (*it)->SetTarget((*it)->Target()-median);
  }  
  
  //sort by absolute value of the recomputed target and computed transformed target
  //according to huber loss function derivative (cutoff of outliers)
  std::sort(evts.begin(),evts.end(),GBRAbsTargetCMP());
  double transumw = 0.;
  float transition = 0.;
  std::vector<GBREvent*>::const_iterator transit=evts.begin();
  while(transumw<(fTransitionQuantile*sumw) && transit!=evts.end()) {
    transumw += (*transit)->Weight();
    transition = std::abs((*transit)->Target());  
    ++transit;
  } 
  
  for (std::vector<GBREvent*>::iterator it=evts.begin(); it!=evts.end(); ++it) {
    float tgt = (*it)->Target();
    if (std::abs(tgt)<transition) (*it)->SetTransTarget(tgt);
    else if (tgt<0.) (*it)->SetTransTarget(-transition);
    else (*it)->SetTransTarget(transition);
  }    

  
  printf("sumw = %5f, median = %5f, transition = %5f\n",sumw, median,transition);
  
  //loop over requested number of trees
  for (int itree=0; itree<ntrees; ++itree) {
    printf("tree %i\n",itree);

    //sort events by recomputed target, which is expected/required for correct computation
    //of median for each terminal mode
    std::sort(evts.begin(),evts.end(),GBRTargetCMP());
      
    forest->Trees().push_back(GBRTree());
    GBRTree &tree = forest->Trees().back();

    //train a single tree recursively from the root node
    TrainTree(evts,sumw,tree,nvars,transition);
    
    //recompute transition point and transformed target
    std::sort(evts.begin(),evts.end(),GBRAbsTargetCMP());
    transumw = 0.;
    transit=evts.begin();
    while(transumw<(fTransitionQuantile*sumw) && transit!=evts.end()) {
      transumw += (*transit)->Weight();
      transition = std::abs((*transit)->Target());  
      ++transit;
    } 
    
    for (std::vector<GBREvent*>::iterator it=evts.begin(); it!=evts.end(); ++it) {
      double tgt = (*it)->Target();
      if (std::abs(tgt)<transition) (*it)->SetTransTarget(tgt);
      else if (tgt<0.) (*it)->SetTransTarget(-transition);
      else (*it)->SetTransTarget(transition);
    }       
    
  }
  
  //return fully trained GBRForest
  return forest;
  
}

//_______________________________________________________________________
void GBRTrainer::TrainTree(const std::vector<GBREvent*> &evts, double sumwtotal, GBRTree &tree, int nvars, double transition) {
  
  //index of current intermediate node
  int thisidx = tree.CutIndices().size();    
  
  //number of events input to node
  int nev = evts.size();
  
  //index of best cut variable
  int bestvar = 0;

  //trivial open-mp based multithreading of loop over input variables
  //The loop is thread safe since each iteration writes into its own
  //elements of the 2-d arrays
#pragma omp parallel for
  for (int ivar=0; ivar<nvars; ++ivar) {

    //fill temporary array of quantiles (to allow auto-vectorization of later loops)
    for (int iev = 0; iev<nev; ++iev) {
      quants[ivar][iev] = evts[iev]->Quantile(ivar);
    }
    
    int minquant = std::numeric_limits<int>::max();
    int maxquant = 0;
    
    //find max and min quantiles in the input events
    //(this loop should be vectorized by gcc with reasonable optimization options)
    for (int iev = 0; iev<nev; ++iev) {
      if (quants[ivar][iev]<minquant) minquant = quants[ivar][iev];
      if (quants[ivar][iev]>maxquant) maxquant = quants[ivar][iev];
    }    
    
    //calculate offset and scaling (powers of 2) to reduce the total number of quantiles
    //to the fNBinsMax for the search for the best split value
    int offset = minquant;
    unsigned int bincount = maxquant-minquant+1;
    unsigned int pscale = 0;
    while (bincount>fNBinsMax) {
      ++pscale;
      bincount >>= 1;
    }    
//    int scale = 1<<pscale;
    
    //final number of bins (guaranteed to be <= fNBinsMax) for best split search
    const unsigned int nbins = ((maxquant-offset)>>pscale) + 1;
    

    //zero arrays where necessary and compute map between bin numbers
    //and variable cut values
    //This loop should auto-vectorize in appropriate compiler/settings
    for (unsigned int ibin=0; ibin<nbins; ++ibin) {
      ws[ivar][ibin] = 0.;
      ns[ivar][ibin] = 0;
      tgts[ivar][ibin] = 0.;
      tgt2s[ivar][ibin] = 0.;
      
      int quant = ((1+ibin)<<pscale) + offset - 1;
      
      varvals[ivar][ibin] = fQuantileMaps[ivar][quant];

    }
    
    //compute reduced bin value for each event using bit-shift operations
    //This loop should auto-vectorize in appropriate compiler/settings
    for (int iev=0;iev<nev;++iev) {
      bins[ivar][iev] = (quants[ivar][iev]-offset)>>pscale;
    }

     
    //compute summed quantities differential in each bin
    //(filling 'histograms')
    //This loop is one of the most expensive in the algorithm for large training samples
    //This loop can unfortunately not be vectorized because the memory addressed 
    //are computed within the loop iteration
    //JOSH: Is this already fundamentally making vectorization impossible because the addresses to be incremented are
    //scattered, or is it just that the compiler can't resolve the dependencies?  If the latter, can we force gcc to vectorize
    //this loop)
    
    for (int iev=0;iev<nev;++iev) {
      int ibin = bins[ivar][iev];
      
      ws[ivar][ibin] += evts[iev]->Weight();
      ++ns[ivar][ibin];
      tgts[ivar][ibin] += evts[iev]->WeightedTransTarget();
      tgt2s[ivar][ibin] += evts[iev]->WeightedTransTarget2();

    } 
 
    //convert differential arrays to cumulative arrays by summing over
    //each element
    //loop cannot be vectorized because this is an iterative calculation
    sumws[ivar][0] = ws[ivar][0];
    sumns[ivar][0] = ns[ivar][0];
    sumtgts[ivar][0] = tgts[ivar][0];
    sumtgt2s[ivar][0] = tgt2s[ivar][0];    
    
    for (unsigned int ibin=1; ibin<nbins; ++ibin) {      
      sumws[ivar][ibin] = sumws[ivar][ibin-1] + ws[ivar][ibin];
      sumns[ivar][ibin] = sumns[ivar][ibin-1] + ns[ivar][ibin];
      sumtgts[ivar][ibin] = sumtgts[ivar][ibin-1] + tgts[ivar][ibin];
      sumtgt2s[ivar][ibin] = sumtgt2s[ivar][ibin-1] + tgt2s[ivar][ibin];  
    }
    
    //int n = sumns[ivar][nbins-1];
    float sumw = sumws[ivar][nbins-1];
    float sumtgt = sumtgts[ivar][nbins-1];
    float sumtgt2 = sumtgt2s[ivar][nbins-1];      
    
    //weighted variance of target in full dataset
    float fullvariance = sumtgt2 - sumtgt*sumtgt/sumw;
    
   // printf("fullrms = %5f, sumtgt2 = %5f, sumtgt = %5f, sumw = %5f\n",fullrms,sumtgt2,sumtgt,sumw);
    
    //printf("short loop\n");
    float maxsepgain = -std::numeric_limits<float>::max();
    float cutval = 0.;
    int nleft= 0;
    int nright = 0;
    float sumwleft=0.;
    float sumwright=0.;
    int bestbin=0;
    
    //loop over all bins and compute improvement in weighted variance of target for each split
    //This loop is relatively expensive and should auto-vectorize in the appropriate compiler/settings
    for (unsigned int ibin=0; ibin<nbins; ++ibin) {
      float leftvariance = sumtgt2s[ivar][ibin] - sumtgts[ivar][ibin]*sumtgts[ivar][ibin]/sumws[ivar][ibin];
      float rightsumw = sumw - sumws[ivar][ibin];
      float righttgtsum = sumtgt - sumtgts[ivar][ibin];
      float righttgt2sum = sumtgt2 - sumtgt2s[ivar][ibin];
      float rightvariance = righttgt2sum - righttgtsum*righttgtsum/rightsumw;
      
      //weighted improvement in variance from this split
      bsepgains[ivar][ibin] = fullvariance - rightvariance - leftvariance;
    }
    
    //loop over computed variance improvements and select best split, respecting also minimum number of events per node
    //This loop cannot auto-vectorize, at least in gcc 4.6x due to the mixed type conditional, but it's relatively fast
    //in any case
    for (unsigned int ibin=0; ibin<nbins; ++ibin) {   
      if (sumns[ivar][ibin]>=fMinEvents && (nev-sumns[ivar][ibin])>=fMinEvents && bsepgains[ivar][ibin]>maxsepgain) {
	maxsepgain = bsepgains[ivar][ibin];
        bestbin = ibin;
      }
    }
     
    cutval = varvals[ivar][bestbin];
    nleft = sumns[ivar][bestbin];
    nright = nev - nleft;
    sumwleft = sumws[ivar][bestbin];
    sumwright = sumw - sumwleft;        
    
    sepgains[ivar] = maxsepgain;
    cutvals[ivar] = cutval;
    nlefts[ivar] = nleft;
    nrights[ivar] = nright;
    sumwlefts[ivar] = sumwleft;
    sumwrights[ivar] = sumwright;
    bestbins[ivar] = bestbin;
        
  }
  

  
  float globalsepgain = -std::numeric_limits<float>::max();
  for (int ivar=0; ivar<nvars; ++ivar) {
    if (sepgains[ivar]>globalsepgain) {
      globalsepgain = sepgains[ivar];
      bestvar = ivar;
    }
  }    
  
  //if no appropriate split found, make this node terminal
  if (globalsepgain<=0.) {
    //no valid split found, making this node a leaf node
    //printf("globalsepgain = %5f, no valid split\n",globalsepgain);
    BuildLeaf(evts,sumwtotal,tree,transition);
    return;
  }
  
  //fill vectors of event pointers for left and right nodes below this one
  std::vector<GBREvent*> leftevts;
  std::vector<GBREvent*> rightevts;
  
  leftevts.reserve(nev);
  rightevts.reserve(nev);
  
  int nleft = 0;
  int nright = 0;
  double sumwleft = 0.;
  double sumwright = 0.;
  
  for (std::vector<GBREvent*>::const_iterator it = evts.begin(); it!=evts.end(); ++it) {
    if ((*it)->Var(bestvar)>cutvals[bestvar]) {
      ++nright;
      sumwright += (*it)->Weight();
      rightevts.push_back(*it);
    }
    else {
      ++nleft;
      sumwleft += (*it)->Weight();
      leftevts.push_back(*it);
    }    
  }
 
 // printf("thisidx = %i, bestvar = %i, cutval = %5f, n = %i, nleft = %i, nright = %i\n",thisidx,bestvar,cutvals[bestvar],nev,nlefts[bestvar],nrights[bestvar]);
  
  assert(nlefts[bestvar]==nleft);
  assert(nrights[bestvar]==nright);
  
  //fill intermediate node
  tree.CutIndices().push_back(bestvar);
  tree.CutVals().push_back(cutvals[bestvar]);
  tree.LeftIndices().push_back(0);   
  tree.RightIndices().push_back(0);  
  
  //check if left node is terminal
  bool termleft = nleft<=(2*fMinEvents);
  if (termleft) tree.LeftIndices()[thisidx] = -tree.Responses().size();
  else tree.LeftIndices()[thisidx] = tree.CutIndices().size();
  
  //printf("this idx = %i, termleft = %i, nleft = %i, fMinEvents = %i\n",thisidx,  termleft,nleft,fMinEvents);  
  
  //build left node as appropriate
  if (termleft) {  
    BuildLeaf(leftevts,sumwleft,tree,transition);
  }
  else {  
    TrainTree(leftevts,sumwleft,tree,nvars,transition);  
  }
  
  //check if right node is terminal
  bool termright = nright<=(2*fMinEvents);
  if (termright) tree.RightIndices()[thisidx] = -tree.Responses().size();
  else tree.RightIndices()[thisidx] = tree.CutIndices().size();
    
  //printf("this idx = %i, termright = %i, nright = %i, fMinEvents = %i\n",thisidx,  termright,nright,fMinEvents);    
  
  //build right node as appropriate
  if (termright) {  
    BuildLeaf(rightevts,sumwright,tree,transition);
  }
  else {  
    TrainTree(rightevts,sumwright,tree,nvars,transition);  
  }
  
}

  
  


//_______________________________________________________________________
void GBRTrainer::BuildLeaf(const std::vector<GBREvent*> &evts, double sumw, GBRTree &tree, double transition) {

  //printf("building leaf\n");
  
  //int thisidx = -tree.Responses().size();
  //printf("thisidx = %i\n",thisidx);
  
 
  float medsumw = 0;
  float median = 0.;  
  for (std::vector<GBREvent*>::const_iterator it = evts.begin(); it!=evts.end(); ++it) {    
    if (medsumw<(0.5*sumw)) {
      median = (*it)->Target();
      medsumw += (*it)->Weight();
    }
    else break;
  }
  
  float shift = 0.;
  const float invsumw = 1.0/sumw;
  for (std::vector<GBREvent*>::const_iterator it = evts.begin(); it!=evts.end(); ++it) {
    float weight = (*it)->Weight();
    float diff = (*it)->Target() - median;
    
    if (std::abs(diff) > transition) {
      if (diff<0.) diff = -transition;
      else diff = transition;
    }
    
    shift += weight*invsumw*diff; 
  
  }
  
  float response = fShrinkage*(median+shift);
  
  tree.Responses().push_back(response);
  
  for (std::vector<GBREvent*>::const_iterator it = evts.begin(); it!=evts.end(); ++it) {
    (*it)->SetTarget((*it)->Target()-response);
  }
  
  //printf("thisidx = %i, n = %i, response = %5f\n", thisidx, int(evts.size()) ,response);
  
}


