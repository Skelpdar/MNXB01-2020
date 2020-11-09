#include "TTree.h"
#include "TString.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

std::vector<std::string> splitLine(std::string line, std::string delimeter = " "){
	std::vector<std::string> elements;

	//std::string delimeter = " ";

	auto start = 0;
	auto end = line.find(delimeter);
	while( end != std::string::npos){
		elements.push_back(line.substr(start, end-start));

		start = end + delimeter.length();
		end = line.find(delimeter, start);
	}

	elements.push_back(line.substr(start, end));

	return elements;
}

TTree* readFile(const TString filename){

	TTree* t = new TTree("T", "Weather data tree");

	//
	Double_t temperature;
	Int_t date;
	Char_t quality;

	t->Branch("Temperature", &temperature, 99);
	t->Branch("Time", &date, 99);
	t->Branch("Quality", &quality, 99);

	std::ifstream datafile(filename);

	std::string line;
	while(std::getline(datafile, line)){
		//Split line between spaces
		std::vector<std::string> elements = splitLine(line);

		//Formats the date string
		std::string d = elements.at(0);
		d.append(" ");
		d.append(elements.at(1));
		
		std::vector<std::string> dateparts = splitLine(elements.at(0),":");
		std::string year = dateparts.at(0);
		if(std::stoi(year) >= 1995){
			//Explicit conversion to a char array, as it couldn't figure out a viable way by itself
			TDatime formattedDate(d.c_str());

			//We save this as an 4-byte Int_t instead of time_t, as root wouldn't plot the latter
			//without explicit conversion
			date = formattedDate.Convert();

			temperature = (Double_t)stod(elements.at(2));

			quality = (Char_t)elements.at(3)[0];

			t->Fill();
		}
	}

	t->AutoSave();

	return t;
}
