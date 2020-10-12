#include<bits/stdc++.h>
using namespace std;

#define DatasetDirectory "data/TenFamiliesStructure.csv"
#define AtributesDirectory "data/TenFamiliesAttributes.csv"

int main(){
	std::ifstream structure(DatasetDirectory); 	
	std::ifstream atributes(AtributesDirectory);

	std::ofstream output("PC_FamilyIDs.txt");

	std::string line;
	std::map<int, int> idToFamily;
	std::map<int, char> idToSexo;
	std::set<int> familySet;

	// Part 1: Reads data from Structure dataset and maps person ID to family ID.

	getline(structure, line); // Read header.
	while(getline(structure, line)){

		std::stringstream input(line);
		int id, familyID, lixo;
		char c;
		char sexo;

		// Reads ID and maps to family ID.
		input >> id >> c >> id >> c >> familyID >> c >> lixo >> c >> lixo >> c >> sexo;
		
		idToFamily[id] = familyID;
		idToSexo[id] = sexo;
	}

	// Part 2: Reads data from Atributes dataset and writes family ID to given person ID.

	output << "sexo\n";
	getline(atributes, line); // Read header.
	while(getline(atributes, line)){
		std::stringstream input(line);
		int id;

		input >> id;
		output << idToSexo[id] << "\n";
	}

	output.close();
}