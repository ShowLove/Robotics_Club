#include "LineParamEstimator.h"
#include "RANSAC.h"

/*
 * Example of using the Ransac class for robust parameter estimation.
 *
 * Author: Ziv Yaniv
 */
int  main(int argc, char* argv[])
{
	std::vector<double> lineParameters;
	LineParamEstimator lpEstimator(0.5); //for a point to be on the line it has to be closer than 0.5 units from the line
	std::vector<Point2D> pointData;
	std::vector<Point2D *> pointDataPtr;
	int numForEstimate = 2;
	int numSamples = 200000;
	int numOutliers = 800000;
	double desiredProbabilityForNoOutliers = 0.999;
	double maximalOutlierPercentage = 0.1 + (double)numOutliers/(double)(numSamples + numOutliers);
	double noiseSpreadRadius = 0.4;
	double outlierSpreadRadius = 10;
	int i;
	double newX, newY, dx, dy, norm;

    //1.Create data with outliers

    //randomly select a direction [dx,dy] and create a line passing through the origin
	//for each point sampled on the line add random noise, finally add outlying 
	//points in the direction of the line normal.

	srand((unsigned)time(NULL)); //seed random number generator

    //get random direction
	dx = rand();
	dy = rand();
	norm = sqrt(dx*dx + dy*dy); 
	dx/= norm;
	dy/= norm;
	dx *= (rand() > RAND_MAX/2 ? 1 : -1);
	dy *= (rand() > RAND_MAX/2 ? 1 : -1);


    //add 'numSamples' points
	for(i=0; i<numSamples; i++) {
		newX = i*dx + noiseSpreadRadius*(double)rand()/(double)RAND_MAX * (rand() > RAND_MAX/2 ? 1 : -1);
		newY = i*dy + noiseSpreadRadius*(double)rand()/(double)RAND_MAX * (rand() > RAND_MAX/2 ? 1 : -1);
		pointDataPtr.push_back(new Point2D(newX,newY));
		pointData.push_back(*(pointDataPtr[i]));
	}

    //'numOutliers' points
	double centerX = -dy*100;
	double centerY = dx*100;
	for(i=0; i<numOutliers; i++) {
		newX = centerX + outlierSpreadRadius * (double)rand()/(double)RAND_MAX * (rand() > RAND_MAX/2 ? 1 : -1);
		newY = centerY + outlierSpreadRadius * (double)rand()/(double)RAND_MAX * (rand() > RAND_MAX/2 ? 1 : -1);
		pointDataPtr.push_back(new Point2D(newX,newY));
		pointData.push_back(*(pointDataPtr[pointDataPtr.size()-1]));
	}
	
	double dotProd;

    //2. Compare least squares approach to Ransac

    std::cout<<"Total number of points used: "<<pointData.size()<<std::endl;
	std::cout<<"Number of outliers: "<<numOutliers<<std::endl;
    //The real line parameters
	std::cout<<"Real line parameters [nx,ny,ax,ay]\n\t [ "<<-dy<<", "<<dx<<", 0, 0 ]"<<std::endl;

    //A least squares estimate of the line parameters
	lpEstimator.leastSquaresEstimate(pointDataPtr,lineParameters);
	std::cout<<"Least squares line parameters: [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
	std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
	dotProd = lineParameters[0]*(-dy) + lineParameters[1]*dx;
	std::cout<<"\tDot product of real and computed line normals[+-1=correct]: "<<dotProd<<std::endl;
    dotProd = (-dy)*(lineParameters[2]) + dx*lineParameters[3];
	std::cout<<"\tCheck if computed point is on real line [0=correct]: "<<dotProd<<std::endl;

    clock_t startTime = clock();

    //A RANSAC estimate of the line parameters
    double usedData = RANSAC<Point2D,double>::compute(lineParameters,
                                                      &lpEstimator ,
                                                      pointData,
                                                      desiredProbabilityForNoOutliers);

    clock_t endTime = clock();
    std::cout<<"RANSAC running time: "<<(double)(endTime-startTime)/CLOCKS_PER_SEC<<std::endl;

    if(lineParameters.empty())
        std::cout<<"RANSAC failed to obtain an estimate.\n";
    else
    {
        std::cout<<"RANSAC line parameters [n_x,n_y,a_x,a_y]\n\t [ "<<lineParameters[0]<<", "<<lineParameters[1]<<", ";
        std::cout<<lineParameters[2]<<", "<<lineParameters[3]<<" ]"<<std::endl;
        dotProd = lineParameters[0]*(-dy) + lineParameters[1]*dx;
        std::cout<<"\tDot product of real and computed line normals[+-1=correct]: "<<dotProd<<std::endl;
        dotProd = (-dy)*(lineParameters[2]) + dx*lineParameters[3];
        std::cout<<"\tCheck if computed point is on real line [0=correct]: "<<dotProd<<std::endl;
        std::cout<<"\tPercentage of points which were used for final estimate: "<<usedData<<std::endl;
    }

    pointData.clear();
    for(i=0; i<pointDataPtr.size(); i++)
        delete pointDataPtr[i];

    return EXIT_SUCCESS;
}
