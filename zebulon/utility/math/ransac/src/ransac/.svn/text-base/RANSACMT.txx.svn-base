
template<class T, class S>
RANSACMT<T,S>::RANSACMT()
{
  this->numberOfThreads = itk::MultiThreader::GetGlobalDefaultNumberOfThreads();
}
/*****************************************************************************/

template<class T, class S>
void  RANSACMT<T,S>::setNumberOfThreads(unsigned int numberOfThreads)
{  
  if(numberOfThreads==0 || 
     numberOfThreads>itk::MultiThreader::GetGlobalDefaultNumberOfThreads())
     std::exception();     
  this->numberOfThreads = numberOfThreads;
}

/*****************************************************************************/

template<class T, class S>
double RANSACMT<T,S>::compute(std::vector<S> &parameters, 
								  						ParameterEstimator<T,S> *paramEstimator , 
									  					std::vector<T> &data,  		                     
										  				double desiredProbabilityForNoOutliers)
{
                        //STEP1: setup
  parameters.clear();

  this->data = data;
  this->paramEstimator = paramEstimator;
  unsigned int numForEstimate = this->paramEstimator->numForEstimate();  
  unsigned int numDataObjects = this->data.size();

		      //there are less data objects than the minimum required for an exact fit, or
	        //desiredProbabilityForNoOutliers is not in (0.0,1.0)
	if(numDataObjects < numForEstimate || 
     desiredProbabilityForNoOutliers>=1.0 || desiredProbabilityForNoOutliers<=0.0) 
		return 0;

  this->bestVotes = new bool[numDataObjects];
                 //initalize with 0 so that the first computation which gives 
                //any type of fit will be set to best
  this->numVotesForBest = 0;

  SubSetIndexComparator subSetIndexComparator(numForEstimate);
  this->chosenSubSets = 
    new std::set<int *, SubSetIndexComparator >(subSetIndexComparator);
          //initialize with the number of all possible subsets
  this->allTries = choose(numDataObjects,numForEstimate);
  this->numTries = this->allTries;
  this->numerator = log(1.0-desiredProbabilityForNoOutliers);
  

  srand((unsigned)time(NULL)); //seed random number generator

                  //STEP2: create the threads that generate hypotheses and test

  itk::MultiThreader::Pointer threader = itk::MultiThreader::New();   
  threader->SetNumberOfThreads(this->numberOfThreads);
  threader->SetSingleMethod( RANSACMT<T,S>::RANSACThreadCallback, this);
            //runs all threads and blocks till they finish
  threader->SingleMethodExecute();

         //STEP3: least squares estimate using largest consensus set and cleanup

  std::vector<T *> leastSquaresEstimateData;
	if(this->numVotesForBest > 0) {
		for(unsigned int j=0; j<numDataObjects; j++) {
			if(this->bestVotes[j])
				leastSquaresEstimateData.push_back(&(data[j]));
		}
		paramEstimator->leastSquaresEstimate(leastSquaresEstimateData,parameters);
	}
                      //cleanup
	typename std::set<int *, SubSetIndexComparator >::iterator it = this->chosenSubSets->begin();
	typename std::set<int *, SubSetIndexComparator >::iterator chosenSubSetsEnd = this->chosenSubSets->end();
	while(it!=chosenSubSetsEnd) {
		delete [] (*it);
		it++;
	}
	this->chosenSubSets->clear();
  delete this->chosenSubSets;
	delete [] this->bestVotes;

	return (double)this->numVotesForBest/(double)numDataObjects;
}

/*****************************************************************************/

template<class T, class S>
ITK_THREAD_RETURN_TYPE RANSACMT<T,S>::RANSACThreadCallback( void *arg )
{
  typedef itk::MultiThreader::ThreadInfoStruct ThreadInfoType;
  ThreadInfoType *infoStruct = static_cast<ThreadInfoType*>(arg);
                           //dynamic_cast doesn't work with void *
  RANSACMT<T,S> *caller = reinterpret_cast< RANSACMT<T,S> * >(infoStruct->UserData);

  if(caller!=NULL)
  {
    int i, j, k, l, maxIndex, numVotesForCur;
    int *curSubSetIndexes;

    int numDataObjects = caller->data.size();
    unsigned int numForEstimate = caller->paramEstimator->numForEstimate();
    std::vector<T *> exactEstimateData;
    std::vector<S> exactEstimateParameters;
  	double denominator;
	  
               //true if data[i] agrees with the current model, otherwise false
	  bool *curVotes = new bool[numDataObjects];
         //true if data[i] is NOT chosen for computing the exact fit, otherwise false
	  bool *notChosen = new bool[numDataObjects]; 

	  for(i=0; i<caller->numTries; i++) {
				  //randomly select data for exact model fit ('numForEstimate' objects).
      std::fill(notChosen,notChosen+numDataObjects, true);      
		  curSubSetIndexes = new int[numForEstimate];
		  exactEstimateData.clear();
		  maxIndex = numDataObjects-1; 
		  for(l=0; l<numForEstimate; l++) {
		                             //selectedIndex is in [0,maxIndex]
			  int selectedIndex = (int)(((float)rand()/(float)RAND_MAX)*maxIndex + 0.5);
			  for(j=-1,k=0; k<numDataObjects && j<selectedIndex; k++) {
				  if(notChosen[k])
					  j++;
			  }
			  k--;
			  exactEstimateData.push_back(&(caller->data[k]));
			  notChosen[k] = false;
			  maxIndex--;
		  }
                 //get the indexes of the chosen objects so we can check that this sub-set hasn't been
		             //chosen already
		  for(l=0, j=0; j<numDataObjects; j++) {
			  if(!notChosen[j]) {
				  curSubSetIndexes[l] = j+1;
				  l++;
			  }
		  }

      caller->hypothesisMutex.Lock();

                 //check that the sub-set just chosen is unique
		  std::pair< typename std::set<int *, SubSetIndexComparator >::iterator, bool> res = caller->chosenSubSets->insert(curSubSetIndexes);
		 
      caller->hypothesisMutex.Unlock();

		  if(res.second == true) { //first time we chose this sub set        
				                 //use the selected data for an exact model parameter fit
			  caller->paramEstimator->estimate(exactEstimateData,exactEstimateParameters);
			                  //selected data is a singular configuration (e.g. three colinear points for 
			                  //a circle fit)
			  if(exactEstimateParameters.size() == 0)
				  continue;
					     //see how many agree on this estimate
			  numVotesForCur = 0;        
        std::fill(curVotes,curVotes+numDataObjects, false);
                                 //continue checking data until there is no chance of getting a larger consensus set 
                                //or all the data has been checked              
			  for(j=0; j<numDataObjects && caller->numVotesForBest-numVotesForCur<numDataObjects-j+1; j++) {
				  if(caller->paramEstimator->agree(exactEstimateParameters, caller->data[j])) {
					  curVotes[j] = true;
					  numVotesForCur++;
				  }
			  }                            //found a larger consensus set?
        caller->resultsMutex.Lock();
			  if(numVotesForCur > caller->numVotesForBest) {                    
				  caller->numVotesForBest = numVotesForCur;
          std::copy(curVotes, curVotes+numDataObjects, caller->bestVotes);
                      //all data objects are inliers, terminate the search
			    if(caller->numVotesForBest == numDataObjects)
            i= caller->numTries;                
          else {  //update the estimate of outliers and the number of iterations we need				           		  				
				    denominator = log(1.0- pow((double)numVotesForCur/(double)numDataObjects, (double)(numForEstimate)));
				    caller->numTries = (int)(caller->numerator/denominator + 0.5);
					            //there are cases when the probablistic number of tries is greater than all possible sub-sets
				    caller->numTries = caller->numTries<caller->allTries ? caller->numTries : caller->allTries;
          }
        }
        caller->resultsMutex.Unlock();
		  }
		  else {  //this sub set already appeared, release memory
			  delete [] curSubSetIndexes;			
		  }
	  }
	  delete [] curVotes;
	  delete [] notChosen;
  }
  return ITK_THREAD_RETURN_VALUE;
}

/*****************************************************************************/

template<class T, class S>
unsigned int RANSACMT<T,S>::choose(unsigned int n, unsigned int m)
{
	double denominatorEnd, numeratorStart, numerator,denominator, i, result;
              //perform smallest number of multiplications
	if((n-m) > m) {
		numeratorStart = n-m+1;
		denominatorEnd = m;
	}
	else {
		numeratorStart = m+1;
		denominatorEnd = n-m;
	}
	
	for(i=numeratorStart, numerator=1; i<=n; i++)
		numerator*=i;
	for(i=1, denominator=1; i<=denominatorEnd; i++)
		denominator*=i;	
	result = numerator/denominator;
	
	         //check for overflow both in computation and in result	         
	if(denominator>std::numeric_limits<double>::max() || 
	   numerator>std::numeric_limits<double>::max() || 
	   static_cast<double>(std::numeric_limits<unsigned int>::max())<result )	
	  return std::numeric_limits<unsigned int>::max();
	else 
	  return static_cast<unsigned int>(result);   
}
