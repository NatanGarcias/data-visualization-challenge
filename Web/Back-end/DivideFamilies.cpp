#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#define DatasetDirectory "/home/geflx/github/data-visualization-challenge/Datasets/TenFamiliesStructure.csv"
#define AtributesDirectory "/home/geflx/github/data-visualization-challenge/Datasets/TenFamiliesAttributes2.csv"

#define OutputDirectory "/home/geflx/github/data-visualization-challenge/Web/Back-end/Family_JSON/"
#define MAX_GENERATIONS 1000

struct Person{
	int ID, familyID, motherID, fatherID, 
		gen, age, birthYear, deathYear;
	char sex, deceased, suicide;
	Person(){
		ID = -1, familyID = -1, motherID = -1, deathYear = -1;
		fatherID = -1, gen = -1, age = -1, birthYear = -1;
		sex = 'e', deceased = 'e', suicide = 'e';
	}
};

struct Atributes{
	bool depression, eating, alcohol, somaticDisorder,
		 obesity, psychosis, bipolar, interpersonalTrauma,
		 personalityDisorder, anxiety, asthma, emotional,
		 COPD, impulseControl, cardiovascular, autoimmune;

	std::string deathCause;

	Atributes(){
		depression = false;
		eating = false;
		alcohol = false;
		somaticDisorder = false;
		obesity = false;
		psychosis = false;
		bipolar = false;
		interpersonalTrauma = false;
		personalityDisorder = false;
		anxiety = false;
		asthma = false;
		emotional = false;
		COPD = false;
		impulseControl = false;
		cardiovascular = false;
		autoimmune = false;
	}
};

// A family is made by a vector of people.
struct Family{
	std::vector<Person> vec;

	void InsertPerson(int in_ID, int in_familyID, int in_motherID, int in_fatherID, char in_sex, char in_deceased,
			char in_suicide, int in_birthYear, int in_deathYear, int in_gen, int in_age)
	{
		Person tmp;
		tmp.ID = in_ID;
		tmp.familyID = in_familyID;
		tmp.motherID = in_motherID;
		tmp.fatherID = in_fatherID;
		tmp.gen = in_gen;
		tmp.age = in_age;
		tmp.birthYear = in_birthYear;
		tmp.deathYear = in_deathYear;

		vec.push_back(tmp);
	}
};

void readDataset(std::vector<std::string> &fields, std::vector<Family> &dataset, std::map<int, int> &mapFamily, std::set<int> &c_mapFamily){

	// Opening file.
	std::ifstream input(DatasetDirectory);
	if (!input){
		std::cerr << "Could not open the file! Aborting.\n";
		exit(0);
	}

	// Main variables.
	int count_mapFamily = 0;
	std::string line, token;

	// Reading fields (first line).
	getline(input, line);
	std::stringstream s_stream(line);
	while(std::getline(s_stream, token, ','))
		if(!token.empty())
			fields.push_back(token);

	// Reading persons from dataset and mapping each family to a vector of dataset.
	while(getline(input, line)){
		std::stringstream s_stream_two(line);

		Person tmp;
		int c_field = 0;
		while(std::getline(s_stream_two, token, ',')){
			if(!token.empty()){
				if(c_field == 0 )
					tmp.ID = std::stoi(token);
				else if(c_field == 2 )
					tmp.familyID = std::stoi(token);
				else if(c_field == 3 )
					tmp.motherID = std::stoi(token)==0? -1: std::stoi(token);
				else if(c_field == 4 )
					tmp.fatherID = std::stoi(token)==0? -1: std::stoi(token);
				else if(c_field == 5 )
					tmp.sex = token[0];
				else if(c_field == 6 )
					tmp.deceased = token[0];
				else if(c_field == 7 )
					tmp.suicide = token[0];
				else if(c_field == 8 )
					tmp.birthYear = std::stoi(token);
				else if(c_field == 9 )
					tmp.deathYear = std::stoi(token);					
				else if(c_field == 10 )
					tmp.gen = std::stoi(token);
				else if(c_field == 11 )
					tmp.age = std::stoi(token);				
			}
			++c_field;
		}
		// Maps family to a specific position in dataset vector.
		if(c_mapFamily.find(tmp.familyID) == c_mapFamily.end()){
			mapFamily[tmp.familyID] = c_mapFamily.size();
			c_mapFamily.insert(tmp.familyID);
		}

		// Insert person in the (family) mapped vector.
		dataset[mapFamily[tmp.familyID]].vec.push_back(tmp);
	}
	return;
}

void printDatasetRaw(const std::vector<Family> &dataset){

	// Print dataset raw content.
	for(int i = 0; i < dataset.size(); i++)
		for (int j = 0; j < dataset[i].vec.size(); j++)
			std::cout << dataset[i].vec[j].ID << "," << 
						 dataset[i].vec[j].familyID << "," << 
						 dataset[i].vec[j].motherID << "," << 
						 dataset[i].vec[j].fatherID << "," << 
						 dataset[i].vec[j].sex << "," << 
						 dataset[i].vec[j].deceased << "," << 
						 dataset[i].vec[j].suicide << "," << 
						 dataset[i].vec[j].birthYear << "," << 
						 dataset[i].vec[j].deathYear << "," << 
						 dataset[i].vec[j].gen << "," << 
						 dataset[i].vec[j].age << "\n";		
	return;
}

void readAtributes(std::map<int, Atributes> mapAtributes){

	std::ifstream input(AtributesDirectory);

	std::string line, token;

	// Reading first line.
	getline(input, line);

	// Reading remaining lines
	std::stringstream s_stream(line);
	while(getline(input, line)){
		std::stringstream s_stream_two(line);

		Person tmp;
		int c_field = 0;

		int personID;

		while(std::getline(s_stream_two, token, ',')){

			if(!token.empty()){
				if(c_field == 1){
					personID = std::stoi(token);
				}else if(c_field == 7){ // Death cause
					mapAtributes[personID].deathCause = token;
				}else if(c_field == 11){
					mapAtributes[personID].alcohol = token == "1"? true : false;
				}else if(c_field == 14){
					mapAtributes[personID].psychosis = token == "1"? true : false;
				}else if(c_field == 17){
					mapAtributes[personID].anxiety = token == "1"? true : false;
				}else if(c_field == 20){
					mapAtributes[personID].somaticDisorder = token == "1"? true : false;
				}else if(c_field == 23){
					mapAtributes[personID].eating = token == "1"? true : false;
				}else if(c_field == 26){
					mapAtributes[personID].bipolar = token == "1"? true : false;
				}else if(c_field == 29){
					mapAtributes[personID].depression = token == "1"? true : false;
				}else if(c_field == 32){
					mapAtributes[personID].interpersonalTrauma = token == "1"? true : false;
				}else if(c_field == 35){
					mapAtributes[personID].anxiety = token == "1"? true : mapAtributes[personID].anxiety;
				}else if(c_field == 38){
					mapAtributes[personID].emotional = token == "1"? true : false;
				}else if(c_field == 41){
					mapAtributes[personID].personalityDisorder = token == "1"? true : false;
				}else if(c_field == 44){
					mapAtributes[personID].impulseControl = token == "1"? true : false;
				}else if(c_field == 47){
					mapAtributes[personID].obesity = token == "1"? true : false;
				}else if(c_field == 50){
					mapAtributes[personID].cardiovascular = token == "1"? true : false;
				}else if(c_field == 53){
					mapAtributes[personID].COPD = token == "1"? true : false;
				}else if(c_field == 56){
					mapAtributes[personID].asthma = token == "1"? true : false;
				}else if(c_field == 59){
					mapAtributes[personID].autoimmune = token == "1"? true : false;
				}
			}
			c_field++;
		}
	}
	return;
}

void printFamilyFiles(const std::vector<Family> &dataset, const std::map<int, Atributes> &mapAtributes){
	for(int i = 0; i < dataset.size(); i++){

		// Create output file.
		std::string outDir = OutputDirectory;
		std::string outStr = outDir + "Family_" + std::to_string(i+1) + ".txt";
		std::ofstream outFile(outStr);

		// Map marriage edges, manage people outside family/first generation and map suicides.
		std::map<int, int> marriage;
		std::set<int> outsiders;
		std::set<int> suicided;
		std::set<int> printed;

		std::set<int> printedFixed;
		std::set<int> printedAsSpouse;

		for(int j = 0; j < dataset[i].vec.size(); j++){
			if(dataset[i].vec[j].fatherID != -1 && dataset[i].vec[j].motherID != -1){
				marriage[dataset[i].vec[j].fatherID] = dataset[i].vec[j].motherID;
				marriage[dataset[i].vec[j].motherID] = dataset[i].vec[j].fatherID;
			}else{
				// Special case: men from generation 1 won't be outsiders.
				if(dataset[i].vec[j].gen == 1){
					if(dataset[i].vec[j].sex == 'F')
						outsiders.insert(dataset[i].vec[j].ID);
				}
				// Special case: Person 23883 indicates only one parent.
				else if(dataset[i].vec[j].fatherID == -1 && dataset[i].vec[j].motherID == -1){
					outsiders.insert(dataset[i].vec[j].ID);
				}
			}
			if(dataset[i].vec[j].suicide == 'Y')
				suicided.insert(dataset[i].vec[j].ID);
		}

		// Print outsiders/family from first generation. Men will be root in this generation.
		for(int j = 0; j < dataset[i].vec.size(); j++){
			if(dataset[i].vec[j].gen == 1 && dataset[i].vec[j].sex == 'F'){
				outFile << "{\"child\":\"" << dataset[i].vec[j].ID << "\", \"parent\":\"\", \"spouse\":";
				
				// Print spouse and if spouse suicided.
				int spouse = marriage[dataset[i].vec[j].ID];
				if(spouse != -1){
					outFile << "\"" << std::to_string(spouse) << "\", ";
					if(suicided.find(spouse) != suicided.end())
						outFile << "\"spouse_suicide\":\"yes\", ";
					else
						outFile << "\"spouse_suicide\":\"no\", ";
					printed.insert(spouse);
				}
				outFile << "\"sex\":\"F\"},\n";
				printed.insert(dataset[i].vec[j].ID);
				printed.insert(spouse);
				printedFixed.insert(dataset[i].vec[j].ID);
				printedAsSpouse.insert(spouse);
			}			
		}

		// Print other generations.
		for(int k = 2; k <= MAX_GENERATIONS; k++){
			for(int j = 0; j < dataset[i].vec.size(); j++){
				if(dataset[i].vec[j].gen == k){

					// If I'm not the outsider.
					if(outsiders.find(dataset[i].vec[j].ID) == outsiders.end()){
						outFile << "{\"child\":\"" << dataset[i].vec[j].ID << "\",";
						
						if(printedFixed.find(dataset[i].vec[j].fatherID) != printedFixed.end()){
								outFile << " \"parent\":\""<< dataset[i].vec[j].fatherID << "\",";
						}else if(printedFixed.find(dataset[i].vec[j].motherID) != printedFixed.end()){
								outFile << " \"parent\":\""<< dataset[i].vec[j].motherID << "\",";
						}else{
							std::cout << "Parents weren't printed in tree. Aborting ... ";
							exit(0);
						}
			
						// Print spouse information if married.
						if(marriage[dataset[i].vec[j].ID] != 0){
							int spouse = marriage[dataset[i].vec[j].ID];
							outFile << "\"spouse\": \"" << std::to_string(spouse) << "\", \"spouse_suicide\": \"";

							if(suicided.find(spouse) != suicided.end())
								outFile << "yes";
							else
								outFile << "no";
							outFile << "\",";
							printed.insert(spouse);
							printedAsSpouse.insert(spouse);
						}

						// Printing suicide info.
						outFile << "\"suicide\":\"";
						if(suicided.find(dataset[i].vec[j].ID) != suicided.end())
							outFile << "yes";
						else
							outFile << "no";
						outFile << "\"},\n";

						printed.insert(dataset[i].vec[j].ID);
						printedFixed.insert(dataset[i].vec[j].ID);
					}
				}
			}
		}

		std::cout << "POSITIONS: " << dataset[i].vec.size() << " PRINTED: " << printed.size() << "\n";
		for(int j = 0; j < dataset[i].vec.size(); j++){
			if(printed.find(dataset[i].vec[j].ID) == printed.end()){
				std::cout << "Not printed: " << dataset[i].vec[j].ID << ", father: " << dataset[i].vec[j].fatherID << " mother: " << dataset[i].vec[j].motherID << " gen: " << dataset[i].vec[j].gen << "\n";
			}
		}
		std::cout << "\n\n\n";
		outFile.close();
	}
}

int main(){

	std::vector<Family> dataset (10); // 10 families.
	std::map<int, int> mapFamily;
	std::set<int> c_mapFamily;
	std::vector<std::string> fields;

	readDataset(fields, dataset, mapFamily, c_mapFamily);
	
	std::map<int, Atributes> mapAtributes;

	readAtributes(mapAtributes);

	printFamilyFiles(dataset, mapAtributes);
}