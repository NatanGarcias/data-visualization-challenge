#include <vector>
#include <fstream>

struct Person{
	int ID, familyID, motherID, fatherID, 
		gen, age, birthYear, deathYear;
	bool deceased, suicide;
	char sex;
};

struct Family{
	std::vector<Person> vec;
	int tableID;

	Family(int tID) : tableID(tID) {};
	InsertPerson(int in_ID, int in_familyID, int in_motherID, int in_fatherID, int in_gen,
				 int in_age, int in_birthYear, int in_deathYear, bool in_deceased, bool in_suicide,
				 char in_sex)
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

int main(){

}