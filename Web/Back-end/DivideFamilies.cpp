#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

#define DatasetDirectory "/home/geflx/github/data-visualization-challenge/Datasets/TenFamiliesStructure.csv"
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

void printFamilyFiles(const std::vector<Family> &dataset){
	for(int i = 0; i < dataset.size(); i++){

		// Create output file.
		std::string outStr = "Family_" + std::to_string(i+1) + ".txt";
		std::ofstream outFile(outStr);

		// Map marriage edges, manage people outside family/first generation and map suicides.
		std::map<int, int> marriage;
		std::set<int> outsiders;
		std::set<int> suicided;
		std::set<int> printed;

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
			if(dataset[i].vec[j].gen == 1 && dataset[i].vec[j].sex == 'M'){
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
				outFile << "\"sex\":\"M\"}, ";
				printed.insert(dataset[i].vec[j].ID);
				printed.insert(spouse);
			}			
		}

		// Print other generations.
		for(int k = 2; k <= MAX_GENERATIONS; k++){
			for(int j = 0; j < dataset[i].vec.size(); j++){
				if(dataset[i].vec[j].gen == k){

					// If I'm not the outsider.
					if(outsiders.find(dataset[i].vec[j].ID) == outsiders.end()){
						outFile << "{\"child\":\"" << dataset[i].vec[j].ID << "\",";
						
						// The D3's parent field will have a non-outsider parent.
						bool father_is_outsider = outsiders.find(dataset[i].vec[j].fatherID) == outsiders.end() ? false : true;
						bool mother_is_outsider = outsiders.find(dataset[i].vec[j].motherID) == outsiders.end() ? false : true;

						if(!father_is_outsider && !mother_is_outsider){

							// Special case: IDs 23883, 3388 ... (Only one parent is specified, then print the known one).
							int knownParent = -1;
							if(dataset[i].vec[j].fatherID != -1)
								knownParent = dataset[i].vec[j].fatherID;
							else
								knownParent = dataset[i].vec[j].motherID;

							outFile << " \"parent\":\"" << knownParent << "\",";

						}else if(!father_is_outsider || !mother_is_outsider){
							if(!father_is_outsider){
								outFile << " \"parent\":\""<< dataset[i].vec[j].fatherID << "\",";
							}else{ // !mother_is_outsider
								outFile << " \"parent\":\""<< dataset[i].vec[j].motherID << "\",";
							}
						}else{
							std::cout << "Unusual error. Aborting.\n";
							std::cout << "\n\n";
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
						}

						// Printing suicide info.
						outFile << "\"suicide\":\"";
						if(suicided.find(dataset[i].vec[j].ID) != suicided.end())
							outFile << "yes";
						else
							outFile << "no";
						outFile << "\"},";

						printed.insert(dataset[i].vec[j].ID);
					}
				}
			}
		}

		std::cout << "POSITIONS: " << dataset[i].vec.size() << " PRINTED: " << printed.size() << "\n";
		outFile.close();
	}
}

int main(){

	std::vector<Family> dataset (10); // 10 families.
	std::map<int, int> mapFamily;
	std::set<int> c_mapFamily;
	std::vector<std::string> fields;

	readDataset(fields, dataset, mapFamily, c_mapFamily);
	
	printDatasetRaw(dataset);
	printFamilyFiles(dataset);
}