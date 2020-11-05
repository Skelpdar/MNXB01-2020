#include "TGraph.h"
#include "TTree.h"
#include "TString.h"
#include "TH2.h"
#include "dataLoader.C"
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <utility>
using namespace std;
/*1951-01-01 is the earliest entry*/
int NeverCalled (){
  std::vector<int> myvector;

  // set some values (from 1 to 10)
  for (int i=1; i<=10; i++) myvector.push_back(i);

  // erase the 6th element
  myvector.erase (myvector.begin()+5);

  // erase the first 3 elements:
  myvector.erase (myvector.begin(),myvector.begin()+3);

  std::cout << "myvector contains:";
  for (unsigned i=0; i<myvector.size(); ++i)
    std::cout << ' ' << myvector[i];
  std::cout << '\n';

  return 0;
}
// TDatime xAsDate(){	
// }
bool IsPrime(Long64_t n){//only works for 0-50,1950-2020 //I shouldn't have had to write this in Python
	if (n==2||n==3||n==5||n==7||n==11||n==13||n==17||n==19||n==23||n==29||n==31||n==37||n==41||n==43||n==47||n==1951||n==1973||n==1979||n==1987||n==1993||n==1997||n==1999||n==2003||n==2011||n==2017){
		return true;
	}
	else{
		return false;
	}
}
Long64_t FindHourByIndex(TDatime xd[], TDatime Interest, Long64_t N){ //I shouldn't have had to write this in Python
	for(Long64_t i=0; i<N; i++){
		if (xd[i]==Interest){//yyyy-mm-dd
			return i;
		}
	}
	cout << "date not in set";
	return -1;
}
Long64_t FindByIndex(vector<ULong64_t> set, ULong64_t InterestIndex, Long64_t N){ //I shouldn't have had to write this in Python
	for(ULong64_t i=0; i<set.size(); i++){
		if (set[i]==InterestIndex){
			return i;
		}
	}
	cout << "date not in set"<<endl;
	return -1;
}




bool checkHottest(Double_t x[],Double_t y[],vector<int> ApplicableHours){
// bool checkHottest(Double_t x[],Double_t y[]){
	Double_t MaxTemp=0;
	Long64_t HottestDayIndex;	// vector<int> ties;
	for (ULong64_t i=0; i<ApplicableHours.size(); i++){
		if (y[ApplicableHours[i]]>MaxTemp){
			MaxTemp=y[ApplicableHours[i]];
			HottestDayIndex=ApplicableHours[i];
		}		
	}
	TDatime time;
	time.Set(x[HottestDayIndex]);
	cout << "Hottest hour is: " << time.GetDate() <<endl;
	cout << "Hottest hour's temperature is: " << MaxTemp <<endl;
	return true;
}
vector<ULong64_t> SortByHottest(Double_t x[],Double_t y[],vector<int> ApplicableHours){
	// bool checkHottest(Double_t x[],Double_t y[]){
	vector<ULong64_t> RankingIndices;	//contains the index of the days ranked by hotness
	Double_t MaxTemp;
	Long64_t HottestDayIndex;	// vector<int> ties;
	TDatime SameDayEliminationVar;
	TDatime SameDayEliminationVar2;
	while (ApplicableHours.size() > 3 && RankingIndices.size() < 101){
		MaxTemp=0;
		for (ULong64_t i=0; i<ApplicableHours.size(); i++){//find the hottest currrently
			if (y[ApplicableHours[i]]>MaxTemp){
				MaxTemp=y[ApplicableHours[i]];
				HottestDayIndex=ApplicableHours[i];				
			}		
		}
		RankingIndices.push_back(HottestDayIndex);
		// cout << "The size before:" << RankingIndices.size() << endl;
		// cout << "Remaining hours:" << ApplicableHours.size() << endl;
		for (ULong64_t i=0; i<ApplicableHours.size(); i++){//remove all entries from that day
			SameDayEliminationVar.Set(x[HottestDayIndex]);
			SameDayEliminationVar2.Set(x[ApplicableHours[i]]);
			if (SameDayEliminationVar.GetDate() == SameDayEliminationVar2.GetDate()){
				ApplicableHours.erase(ApplicableHours.begin()+i);
				i--;
			}		
		}
		if (RankingIndices.size()%100==0){
			cout << "Building rank:" << RankingIndices.size() << endl;
		}	
	}		
	cout << "The size of the ranking:" << RankingIndices.size() << endl;
	// cout << "The size after:" << RankingIndices.size() << endl;
	return RankingIndices;
}
vector<int> allHalloweens(Double_t x[],Long64_t N){ //returns all hours that are new years
	vector<int> a;
	for(Long64_t i=0; i<N; i++){
		TDatime time;
		time.Set(x[i]);
		if (time.GetDay() == 31 && time.GetMonth() == 10){
			a.push_back(i);
			
		}
	}	
	return a;
}
vector<int> everything(Double_t x[],Long64_t N){ //returns all hours
	vector<int> a;
	for(Long64_t i=0; i<N; i++){
		TDatime time;
		time.Set(x[i]);		
		a.push_back(i);		
	}	
	return a;
}

vector<int> allPrimeDates(Double_t x[],Long64_t N){ //returns all hours that have prime day, month, and year
	vector<int> a;
	for(Long64_t i=0; i<N; i++){
		TDatime time;
		time.Set(x[i]);
		if (IsPrime(time.GetYear()) && IsPrime(time.GetMonth()) && IsPrime(time.GetDay()) ){
			a.push_back(i);
		}
	}	
	return a;
}
vector<int> descendingDates(Double_t x[],Long64_t N){ //unimplemented
	vector<int> a;
	for(Long64_t i=0; i<N; i++){
		TDatime time;
		time.Set(x[i]);
		string S = time.AsString();
		if (S[0]<S[1] ){//yyyy-mm-dd
			a.push_back(i);
		}
	}	
	return a;
}

Long64_t FindRankOfDay(vector<ULong64_t> ranking, ULong64_t InterestIndex, Long64_t N, TDatime xd[]){ 
	for(ULong64_t i=0; i<ranking.size(); i++){
		// cout << "comparing :"  <<xd[ranking[i]].GetDate() <<endl;
		// cout << "with: "<<xd[InterestIndex].GetDate()<<endl;
		if (xd[ranking[i]].GetDate()==xd[InterestIndex].GetDate()){
			return i;
		}
	}
	cout << "date not in set"<<endl;
	return -1;
}

// pair <vector<TDatime> ,vector<ULong64_t>> DaysWithHotness(TDatime xd[],Double_t y[],Long64_t N){
vector<TDatime> DaysWithHotness1(TDatime xd[],Double_t y[],Long64_t N){
	vector<TDatime> Days;	
	vector<Double_t> Temperatures;		
	Double_t MaxTemp = 0;
	for (Long64_t i=1;i<N;i++){
		if (xd[i].GetDate() == xd[i-1].GetDate()){
			if (y[i]>MaxTemp){
				MaxTemp = y[i];
			}
		}
		if (xd[i].GetDate() != xd[i-1].GetDate()){
			Days.push_back(xd[i-1]);
			Temperatures.push_back(MaxTemp);
			MaxTemp=0;
		}		
	}
	return Days;	
}
vector<Double_t> DaysWithHotness2(TDatime xd[],Double_t y[],Long64_t N){
	vector<TDatime> Days;	
	vector<Double_t> Temperatures;		
	Double_t MaxTemp = 0;
	for (Long64_t i=1;i<N;i++){
		if (xd[i].GetDate() == xd[i-1].GetDate()){
			if (y[i]>MaxTemp){
				MaxTemp = y[i];
			}
		}
		if (xd[i].GetDate() != xd[i-1].GetDate()){
			Days.push_back(xd[i-1]);
			Temperatures.push_back(MaxTemp);
			MaxTemp=0;
		}		
	}
	return Temperatures;	
}	
// /*
vector<TDatime> SortDaysByHottest(vector<TDatime> xDays, vector<Double_t> yDays){
	vector<TDatime> Ranking;	
	Double_t MaxTemp;
	Long64_t HottestDayIndex;	
	while (xDays.size() > 201){//because Erik did N+100 and I didn't have the time to question him
		MaxTemp=0;
		for (ULong64_t i=0; i<xDays.size(); i++){//find the hottest currrently
			if (yDays[i]>MaxTemp){
				MaxTemp=yDays[i];
				HottestDayIndex=i;				
			}		
		}
		Ranking.push_back(xDays[HottestDayIndex]);
		
		xDays.erase(xDays.begin()+HottestDayIndex);
		yDays.erase(yDays.begin()+HottestDayIndex);
		if (Ranking.size()%100==0){
			// cout << "Remaining stuff:" << xDays.size() << endl;
		}	
	}		
	// cout << "The size of the  (faster)  ranking:" << Ranking.size() << endl;
	cout << "The size of the ranking:" << Ranking.size() << endl;
	return Ranking;
}

Long64_t FindRankOfDay2(vector<TDatime> xDays, TDatime HourOfInterest){ 
	for(ULong64_t i=0; i<xDays.size(); i++){
		if (xDays[i].GetDate()==HourOfInterest.GetDate()){
			return i;
		}
	}
	cout << "date not in faster set"<<endl;
	return -1;
}

	
// */
void funFact(){
	/*start mostly Erik's code*/
	auto t = readFile("data/rawdata_smhi-opendata_1_52240_20200905_163726.csv"); //Data loading from TTree
	Double_t temperature = 0;
	t->SetBranchAddress("Temperature",&temperature);
	Long64_t N = t->GetEntries();
	Double_t y[N+100];
	for(Long64_t i=0; i<N; i++){
		t->GetEntry(i);
		y[i] = temperature;
	}
	// cout << "Y length: " << N <<endl;	
	Int_t time = 0;
	t->SetBranchAddress("Time",&time);
	N = t->GetEntries();
	Double_t x[N+100];	
	for(Long64_t i=0; i<N; i++){
		t->GetEntry(i);
		x[i] = (Double_t)time;
	}
	// cout << "X length: " << N <<endl;	
	// cout<<"Number of hours in the sample: "<<t->GetEntries()<<endl;
	/*end mostly Erik's code*/
	
	// int RankingSize=1000+1;
	
	TDatime xd[N+100]; //x, but as date
	for(Long64_t i=0; i<N; i++){
		xd[i].Set(x[i]);
	}
	cout << "Please insert a date and time of interest of format YYYY-MM-DD" << endl;
	TString input;
	cin >> input;
	
	if (input == " "){
		input = "2012-08-03";
	}	
	// input = "2011-11-11";
	// input = "2018-08-03";
	// input = "2018-08-03";

	TString input2 = input + " 23:00:00";
	
	
	TDatime HourOfInterest;
	HourOfInterest = TDatime(input2);
	Long64_t HourOfInterestIndex = FindHourByIndex(xd, HourOfInterest, N);
	
	// cout << HourOfInterest.AsString() << endl;
	// cout << "The index of the requested day is: "<< HourOfInterestIndex << endl;
	cout << "You have inputted: "<< input << endl;

	// vector<int> b = everything(x,N);
	

	// for (Long64_t i=0; i<b.size(); i++){
		// TDatime time;
		// time.Set(x[b[i]]);	
		// cout << "Halloween is: " << time.GetDate() <<endl;
	// }
	

	
	// vector<ULong64_t> c=SortByHottest(x,y,everything(x,N));
	
	TH2* h2 = new TH2F("h2", input, 2, -1.6, 1.6, 20, 2000, 2020);
	
	
	// Long64_t rank = FindRankOfDay(c, HourOfInterestIndex, N,xd);
	// cout << "Out of all days, this day was #"<< rank+1 << " in terms of hotness" << endl;
	// for(Long64_t i=0; i<rank; i++){
		// h2->Fill(1,xd[c[i]].GetYear());
	// }
	// for(Long64_t i=rank+1; i<101; i++){
		// h2->Fill(-1,xd[c[i]].GetYear());
	// }
	
	vector<TDatime> xDays =   DaysWithHotness1(xd,y,N);
	vector<Double_t> yDays = DaysWithHotness2(xd,y,N);
	
	vector<TDatime> FasterRank = SortDaysByHottest(xDays,yDays);
	Long64_t rank2 = FindRankOfDay2(xDays,HourOfInterest);
	
	cout << "Out of all days, this day was #"<< rank2+1 << " in terms of hotness" << endl;
	
	for(Long64_t i=0; i<rank2; i++){
		h2->Fill(1,FasterRank[i].GetYear());
	}
	for(ULong64_t i=rank2+1; i<FasterRank.size(); i++){
		h2->Fill(-1,FasterRank[i].GetYear());
	}
	
	
	TAxis* a = h2->GetXaxis();
    a->SetNdivisions(-502);
    a->ChangeLabel(1,-1,-1,-1,-1,-1,"                                                          colder");
    a->ChangeLabel(-1,-1,-1,-1,-1,-1,"hotter                                                         ");

	h2->Draw("box");
	
	
	

	
}

/*	// exploration loop
	for(Long64_t i=0; i<10; i++){
		cout << "The time: " << x[i] <<endl;
		TDatime time;
		time.Set(x[i]);		
		// cout << "The time 2: " << time.AsString() <<endl;
		cout << "The time 2: " << time.GetDay() <<endl;
		cout << "The time 3: " << time.GetDate() <<endl;
		cout << "The temperature: " << y[i] <<endl;
	}
*/	
/*	// printer
for (vector<int>::size_type i = 0; i < c.size(); i++) {
	cout << c.at(i) << endl;
}
*/	


