
Double_t avgTemp(const char* fname, const char* startdate, const char* enddate){
	//auto t = readFile("data/rawdata_smhi-opendata_1_52240_20200905_163726.csv");
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

	return sum/(endindex-startindex+1);
	
}

void maps(){
	/*
	const Int_t n = 5;
	Double_t x[n], y[n], z[n]; 

	//Double_t X[n*2], Z[n];

	for (Int_t i=0;i<n;i++) {
		x[i] = 10+i;
		//X[i*2] = 10+i;
		y[i] = 55+i;
		//X[i*2+1] = 55+i;
		z[i] = i;
		//Z[i] = 5;
		printf(" i %i %f %f \n",i,x[i],y[i]);
	}

	Double_t X[n*n*2], Z[n*n];

	Double_t a[n*n],b[n*n],c[n*n];

	for (Int_t i=0;i<n;i++) {
		for (Int_t k=0;k<n;k++) {
			X[2*(i+n*k)] = i;
			a[(i+n*k)] = i;
			X[2*(i+n*k)+1] = k;
			b[i+n*k] = k;
			Z[i+n*k] = i;
			c[i+n*k] = i;
		}
	}
	*/
	/*
	for (Int_t i=0; i < 2*n*n; i++){
		std::cout <<X[i] << std::endl;;
	}
	*/

	/*
	TGraph *gr = new TGraph(n,x,y);
	gr->SetLineWidth(29);
	gr->SetMarkerSize(1.3);
	gr->Draw("P*");
	*/
	
	Double_t X[2*4] = {18,58.5,14,56,14,59,20,64};
	Double_t Z[4] = {5,10,4,-2};

	//TGraph2D* gr = new TGraph2D(n*n,a,b,c);
	//gr->Draw("P");
	
	TLinearFitter* lf = new TLinearFitter(2);
	lf->SetFormula("hyp2");
	lf->AssignData(4,2,X,Z);
	lf->Eval();
	TVectorD params;
	lf->GetParameters(params);
	//Function on the form z = a0+a1*x+a2*y
	lf->PrintResults(3);

	TCanvas* canv = new TCanvas("image", "map", 40, 40, 314, 695);

	const char* maskfname = "mapmask.png";

        //TImage* maskimg = TImage::Open(maskfname);

        //if (!img) return;

        TASImage maskimage(maskfname);
        UInt_t* maskargb   = maskimage.GetArgbArray();

        //TH2D* hmask = new TH2D("h","mapmask",xPixels,10.459,24.258,yPixels,55.104,69.084);

	const char* fname = "map.png";
	
	TImage* img = TImage::Open(fname);

	if (!img) return;

	TASImage image(fname);
	UInt_t yPixels = image.GetHeight();
	UInt_t xPixels = image.GetWidth();
	UInt_t *argb   = image.GetArgbArray();

	TH2D* h = new TH2D("h","map",xPixels,10.459,24.258,yPixels,55.104,69.084);

	for (int row=0; row<xPixels; ++row) {
		for (int col=0; col<yPixels; ++col) {
			int index = col*xPixels+row;
			float grey = float(argb[index]&0xff)/256;
			if(float(maskargb[index]&0xff)/256 > 0.5){
				Double_t t = params[0]+params[1]*(10.459+(24.258-10.459)*row/(Double_t)xPixels) + params[2]*(55.104+(69.085-55.104)*col/(Double_t)yPixels);
				h->SetBinContent(row+1,yPixels-col,t);
			}
		}
	}

	gStyle->SetPalette(52, 0);
	h->Draw("colz");
/*
	const Int_t n = 5;
	Double_t x[n], y[n], z[n]; 

	//Double_t X[n*2], Z[n];

	for (Int_t i=0;i<n;i++) {
		x[i] = 10+i;
		//X[i*2] = 10+i;
		y[i] = 55+i;
		//X[i*2+1] = 55+i;
		z[i] = i;
		//Z[i] = 5;
		printf(" i %i %f %f \n",i,x[i],y[i]);
	}

	Double_t X[n*n*2], Z[n*n];

	Double_t a[n*n],b[n*n],c[n*n];

	for (Int_t i=0;i<n;i++) {
		for (Int_t k=0;k<n;k++) {
			X[2*(i+n*k)] = i;
			a[(i+n*k)] = i;
			X[2*(i+n*k)+1] = k;
			b[i+n*k] = k;
			Z[i+n*k] = i;
			c[i+n*k] = i;
		}
	}
*/
	/*
	for (Int_t i=0; i < 2*n*n; i++){
		std::cout <<X[i] << std::endl;;
	}
	*/

	/*
	TGraph *gr = new TGraph(n,x,y);
	gr->SetLineWidth(29);
	gr->SetMarkerSize(1.3);
	gr->Draw("P*");
	*/
/*	
	TGraph2D* gr = new TGraph2D(n*n,a,b,c);
	//gr->Draw("P");
	
	TLinearFitter* lf = new TLinearFitter(2);
	lf->SetFormula("hyp2");
	lf->AssignData(n*n,2,X,Z);
	lf->Eval();
	TVectorD params;
	lf->GetParameters(params);
	//Function on the form z = a0+a1*x+a2*y
	lf->PrintResults(3);
*/
	TLine* line = new TLine(15,60,25,65);

	line->Draw();

	gStyle->SetOptStat("000000000");

	//gPad->SetBBoxX2(100);

}
