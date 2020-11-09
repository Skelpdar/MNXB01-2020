/*
 * MNXB-01 Temperature Data Project
 * 2020
 * Author: Erik Wallin
 */

//Calculates the average temperature in file fname, between startdate and enddate
Double_t avgTemp(const char* fname, const char* startdate, const char* enddate){
	
	std::cout << "Reading " << fname << std::endl;
	auto t = readFile(fname);

        //Data loading from TTree
        Double_t temperature = 0;
        t->SetBranchAddress("Temperature",&temperature);
        Long64_t N = t->GetEntries();

        Double_t y[N+100];

        for(Long64_t i=0; i<N; i++){
                t->GetEntry(i);
                y[i] = temperature;
        }

        Int_t time = 0;
        t->SetBranchAddress("Time",&time);
        N = t->GetEntries();

        Double_t x[N+100];

        for(Long64_t i=0; i<N; i++){
                t->GetEntry(i);
                x[i] = (Double_t)time;
        }

	//Determining start and end times
	TDatime date1(startdate);
	
	TDatime date2(enddate);

	Int_t starttime = date1.Convert();
	Int_t endtime = date2.Convert();

	Int_t startindex;
	Int_t endindex;
	for(Int_t i=0; i<N; i++){
		if(x[i] >= starttime){
			startindex = i;
			break;
		}	
	}
	for(Int_t i=startindex; i < N; i++){
		if(x[i] > endtime){
			endindex = i-1;
			break;
		}
	}

	Double_t sum = 0;

	for(Int_t i =startindex; i <= endindex; i++){
		sum += y[i];
	}

	std::cout << "Average temp: " << sum/(endindex-startindex+1) << std::endl;

	return sum/(endindex-startindex+1);
	
}

//Find the n smallest values in an array
//Time complexity of big O(size)
Int_t* findMin(Int_t* minindices, Double_t* array, const Int_t size, Int_t n){
	//Array that eventually holds the indices of the n smallest values
	//Int_t minindices[n];

	//Array holding wether indices have already been identified as minima or not
	bool covered[size];
	for(Int_t i = 0; i < size; i++){
		covered[i] = false;
	}

	for(Int_t i = 0; i < n; i++){
		//Find initial value to compare
		Double_t min;
		Int_t minindex;
		for(Int_t k = 0; k < size; k++){
			if(!covered[k]){
				min = array[k];
				minindex = k;
				break;
			}
		}

		//Search for minimum
		for(Int_t k = 0; k < size; k++){
			if(!covered[k] && array[k] < min){
				minindex = k;
				min = array[k];
			}
		}
		
		covered[minindex] = true;
		minindices[i] = minindex;
	}


	return minindices;
}
 
//Calculates first order Voronoi diagrams (naively, but seems to be quick enough)
//Returns the temperature of the nearest weather station
Double_t Voronoi(const Int_t n, Double_t x, Double_t y, Double_t* X, Double_t* Z){
	//Calculate distances to measuring stations
	Double_t distances[n];
	for(Int_t i=0; i < n; i++){
		distances[i] = (Double_t)sqrt(pow((double)(X[2*i]-x),2)+pow((double)(X[2*i+1]-y),2));
	}

	Int_t minIndices[1];
	findMin(minIndices,distances,n,1);	

	return Z[minIndices[0]];
}

//Second order Voronoi
//Weighs the temperature depending on the distances to the two nearest stations
Double_t twoVoronoi(Int_t n, Double_t x, Double_t y, Double_t* X, Double_t* Z){
	//Calculate distances to measuring stations
	Double_t distances[n];
	for(Int_t i=0; i < n; i++){
		distances[i] = (Double_t)sqrt(pow((double)(X[2*i]-x),2)+pow((double)(X[2*i+1]-y),2));
	}

	Int_t minIndices[2];
	findMin(minIndices,distances,n,2);

	Double_t distancesum = distances[minIndices[0]]+distances[minIndices[1]];

	return Z[minIndices[0]]*(1-distances[minIndices[0]]/distancesum) + Z[minIndices[2]]*(1-distances[minIndices[2]]/distancesum);
}

//Third order Voronoi
//Makes a linear fit of the temperature of the three nearest stations
Double_t threeVoronoi(Int_t n, Double_t x, Double_t y, Double_t* X, Double_t* Z){
	//Calculate distances to measuring stations
	Double_t distances[n];
	for(Int_t i=0; i < n; i++){
		distances[i] = (Double_t)sqrt(pow((double)(X[2*i]-x),2)+pow((double)(X[2*i+1]-y),2));
	}

	Int_t minIndices[3];
	findMin(minIndices,distances,n,3);

	Double_t s = distances[minIndices[0]]+distances[minIndices[1]] + distances[minIndices[2]];

	double pos[2*3] = {X[minIndices[0]*2],X[minIndices[0]*2+1],X[minIndices[1]*2],X[minIndices[1]*2+1],X[minIndices[2]*2],X[minIndices[2]*2+1]};

	double temp[3] = {Z[minIndices[0]],Z[minIndices[1]],Z[minIndices[2]]};

	TLinearFitter* lf = new TLinearFitter(2);
	lf->SetFormula("hyp2");
	lf->AssignData(6,2,pos,temp);
	lf->Eval();
	TVectorD params;
	lf->GetParameters(params);
	return params[0]+x*params[1]+y*params[2];
}

/*
maps() draws a map of Sweden, and interpolates temperature from a sparse sample of
weather measurement stations.

The settings argument chooses how many of nearest neighbours should be chosen:
setting = 0 makes a linear fit over all measurement stations
setting = 1 interpolates temperatures from the nearest stations
setting = 2 makes a weighted average of the two nearest stations
setting = 3 makes a linear fit between the three nearest stations

The startdate and endate determines the period in which the average temperature should be calculated
They need to be in the SQL-like format, as in the default arguments

Example usage:
maps(3);
maps(2, "2015-01-01 00:00:00", "2015-01-02 00:00:00");
*/
void maps(Int_t setting = 3, const char* startdate = "2014-10-01 00:00:00", const char* enddate = "2014-11-01 00:00:00"){

	//Number of weather stations
	const Int_t N = 6;
	
	//Weather station coordinates	
	Double_t X[2*N] = {13.3374, 59.4446,
			15.6603, 60.619,
			12.9493, 57.7611,
			18.3428, 57.6614,
			12.8166, 55.3837,
			22.1193, 65.5434};

	//Find average Weather station temperatures between startdate and enddate
	Double_t Z[N] = {avgTemp("data/rawdata_smhi-openda_Karlstad.csv", startdate, enddate),
	avgTemp("data/rawdata_smhi-opendata_Falun.csv", startdate, enddate),
	avgTemp("data/rawdata_smhi-opendata_Boras.csv", startdate, enddate),
	avgTemp("data/rawdata_smhi-opendata_Visby.csv", startdate, enddate),
	avgTemp("data/rawdata_smhi-opendata_1_52240_20200905_163726.csv", startdate, enddate),
	avgTemp("data/rawdata_smhi-opendata_Lulea.csv", startdate, enddate)};


	//(Quickly) perform a linear fit over the whole map, for the case without Voronoi-cells	
	TLinearFitter* lf = new TLinearFitter(2);
	lf->SetFormula("hyp2");
	lf->AssignData(N*2,2,X,Z);
	lf->Eval();
	TVectorD params;
	lf->GetParameters(params);
	//Function on the form z = a0+a1*x+a2*y
	std::cout << "Global fit results:" << std::endl;
	lf->PrintResults(3);

	const char* fname = "map.png";
	TASImage image(fname);
	UInt_t yPixels = image.GetHeight();
	UInt_t xPixels = image.GetWidth();
	UInt_t *argb   = image.GetArgbArray();
	
	//Should have the same or larger dimensions than the map
	const char* maskfname = "mapmask.png";
        TASImage maskimage(maskfname);
        UInt_t* maskargb   = maskimage.GetArgbArray();

	TCanvas* canv = new TCanvas("map", "Temperature map", 40, 40, image.GetWidth(), image.GetHeight());

	//Map coordinates
	const Double_t longMin = 10.459;
	const Double_t longMax = 24.258;
	const Double_t latMin = 55.104;
	const Double_t latMax = 69.084;
	
	TH2D* h = new TH2D("h","map",xPixels,longMin,longMax,yPixels,latMin,latMax);

	//Iterate through pixels and set the temperature
	for (int row=0; row<xPixels; ++row) {
		for (int col=0; col<yPixels; ++col) {
			int index = col*xPixels+row;
			//An ARGB32 value is a 32 bit integer with four bytes for: alpha, red, green and blue
			// &0xff takes the blue values, whilst &0xff00 would take the green etc.
			float grey = float(argb[index]&0xff)/256;
			//Check whether the mask is white at that pixel, when brightness > 0.5 
			if(float(maskargb[index]&0xff)/256 > 0.5){
				Double_t t;
				//Find co-ords of the pixel
				Double_t x = longMin+(longMax-longMin)*(Double_t)row/(Double_t)xPixels;
				Double_t y = latMax-(latMax-latMin)*(Double_t)col/(Double_t)yPixels;

				//Calculate the temperature (depending on the setting)
				if(setting == 0){
					t = params[0]+params[1]*x + params[2]*y;
				}
				if(setting == 1){
					t = Voronoi(6, x, y, X,Z);
				}
				if(setting == 2){
					t = twoVoronoi(6, x, y,X,Z);
				}
				if(setting == 3){
					t = threeVoronoi(6, x, y,X,Z);
				}
				h->SetBinContent(row+1,yPixels-col,t);
			}
		}
	}

	//Monochrome palette
	gStyle->SetPalette(52,0);
	//Fewer contours in the histogram gives color-banding, while many gives a smooth color gradient
	h->SetContour(100);
	h->Draw("colz");

	//Draw the positions of the measuring stations
	Double_t x[N];
	Double_t y[N];
	for(int i = 0; i < N; i++){
		x[i] = X[2*i];
		y[i] = X[2*i+1];
	}

	TGraph *gr = new TGraph(6,x,y);
	gr->SetLineWidth(29);
	gr->SetMarkerSize(1.3);
	gr->Draw("P*");
	gr->SetMarkerColorAlpha(kRed, 1);	

	//Don't print fit statistics in the graphics
	gStyle->SetOptStat("000000000");

}
