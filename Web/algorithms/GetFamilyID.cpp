#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#define DatasetDirectory "/home/geflx/github/data-visualization-challenge/Datasets/TenFamiliesStructure.csv"
#define AtributesDirectory "/home/geflx/github/data-visualization-challenge/Datasets/TenFamiliesAttributes.csv"

#define OutputDirectory "/home/geflx/github/data-visualization-challenge/Web/Back-end/Family_JSON/"

int main(){
	std::ifstream structure(DatasetDirectory); 	
	std::ifstream atributes(AtributesDirectory);

	std::ofstream output(std::string(OutputDirectory) + "PC_FamilyIDs.txt");

	std::string line;
	std::map<int, int> idToFamily;
	std::set<int> familySet;

	// Part 1: Reads data from Structure dataset and maps person ID to family ID.

	getline(structure, line); // Read header.
	while(getline(structure, line)){
		std::stringstream input(line);
		int id, familyID;
		char c;

		// Reads ID and maps to family ID.
		input >> id >> c >> id >> c >> familyID;
		idToFamily[id] = familyID;

		// Insert family ID in set of IDs.
		familySet.insert(familyID);
	}

	// Part 2: Reads data from Atributes dataset and writes family ID to given person ID.

	output << "family\n";
	getline(atributes, line); // Read header.
	while(getline(atributes, line)){
		std::stringstream input(line);
		int id;

		input >> id;
		output << idToFamily[id] << "\n";
	}

	output.close();

	std::cout << "To use in HTML file (families): \n";
	for(int i : familySet)
		std::cout << i << "\n";
	
}