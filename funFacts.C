#include "TGraph.h"
#include <iostream>

void temperatureOverTime(){

	auto t = readFile("data/rawdata_smhi-opendata_1_52240_20200905_163726.csv");
	
	//This is a simple way to draw directly from a TTree
	//With limited options
	//t->Draw("Temperature:Time");
	//auto graph = (TGraph*)gPad->GetPrimitive("Graph");
	//graph->GetXaxis()->SetTimeFormat("%Y");

	//Data loading from TTree
	Double_t temperature = 0;
	t->SetBranchAddress("Temperature",&temperature);
	Long64_t N = t->GetEntries();

	Double_t y[N+100];

	for(Long64_t i=0; i<N; i++){
		t->GetEntry(i);
		y[i] = temperature;
	}
	
	cout<<"This is a test"<<endl;
	
	Int_t time = 0;
	t->SetBranchAddress("Time",&time);
	N = t->GetEntries();

	Double_t x[N+100];

	for(Long64_t i=0; i<N; i++){
		t->GetEntry(i);
		x[i] = (Double_t)time;
	}

	//Drawing
	TGraph* graph = new TGraph(N,x,y);
	//Make the x-axis a time axis
	graph->GetXaxis()->SetTimeDisplay(1);
	//Start time at EPOCH
	graph->GetXaxis()->SetTimeOffset(0);
	//Formatting, here we just show years
	graph->GetXaxis()->SetTimeFormat("%Y");
	//Scatter plot on new canvas
	graph->Draw("ap");

}
