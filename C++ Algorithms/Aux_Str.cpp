#include<bits/stdc++.h>
using namespace std;

template<typename T> ostream& operator<<(ostream &os, const vector<T> &v) { os << "{"; for (typename vector<T>::const_iterator vi = v.begin(); vi != v.end(); ++vi) { if (vi != v.begin()) os << ", "; os << *vi; } os << "}"; return os; }

#define fi first
#define se second

vector<vector<string>> data;

//TODO : LEITURA
void leitura(){

    string s;

    while(getline(cin,s)){

        string s2;
        vector<string> aux;

        for(int i=0;i<s.size();i++){
            if(s[i] != ','){
                s2.push_back(s[i]);
            }
            else{
                aux.push_back(s2);
                s2.clear();
            }
        }

        data.push_back(aux);
    }
}

struct Familia{
    int M,F,T;
    int sM, sF, sT;

    void init(){
        M = 0;
        F = 0;
        T = 0;
        sM = 0;
        sF = 0;
        sT = 0;
    }

    void printEst(){

        ofstream out("data/Estrutura.csv");

        cout << M << " " << F << " " << T << endl;
        cout << sM << " " << sF << " " << sT << endl << endl;

        cout << fixed  << setprecision(3) <<  100.0 * M/(T*1.0) << " " <<  100.0 * F/(T*1.0) << endl;
        cout << fixed  << setprecision(3) <<  100.0 * sM/(sT*1.0) << " " << 100.0 * sF/(sT*1.0) << endl;
        
        out << "Homens,Mulheres\n";
        out << fixed  << setprecision(3) <<  100.0 * M/(T*1.0) << ',' <<  100.0 * F/(T*1.0) << endl;
        out << fixed  << setprecision(3) <<  100.0 * sM/(sT*1.0) << ',' << 100.0 * sF/(sT*1.0) << endl;
        cout << " ---------------------" << endl << endl;

        out.close();
    }
};

Familia f[9];

void printEst(){
    ofstream out("data/Est.csv");
    out << "family,porcentagem\n";
    for(int i=0;i<9;i++) out << "family_" << i+1 << ',' << fixed << setprecision(3) << f[i].sT/(f[i].T*1.0) << endl;
    out.close();
}

void printM(){
    ofstream out("data/Est_M.csv");
    out << "family,porcentagem\n";
    for(int i=0;i<9;i++) out << "family_" << i+1 << ',' << fixed << setprecision(3) << f[i].M/(f[i].T*1.0) << endl;
    out.close();
}

void printF(){
    ofstream out("data/Est_F.csv");
    out << "family,porcentagem\n";
    for(int i=0;i<9;i++) out << "family_" << i+1 << ',' << fixed << setprecision(3) << f[i].F/(f[i].T*1.0) << endl;
    out.close();
}

void printMS(){
    ofstream out("data/Est_MS.csv");
    out << "family,porcentagem\n";
    for(int i=0;i<9;i++) out << "family_" << i+1 << ',' << fixed << setprecision(3) << f[i].sM/(f[i].sT*1.0) << endl;
    out.close();
}

void printFS(){
    ofstream out("data/Est_FS.csv");
    out << "family,porcentagem\n";
    for(int i=0;i<9;i++) out << "family_" << i+1 << ',' << fixed << setprecision(3) << f[i].sF/(f[i].sT*1.0) << endl;
    out.close();
}

void contStructure(){

    map<string,int> mapa;

    mapa["38"] = 0;
    mapa["149"] = 1;
    mapa["27251"] = 2;
    mapa["42623"] = 3;
    mapa["68939"] = 4;
    mapa["176860"] = 5;
    mapa["603481"] = 6;
    mapa["791533"] = 7;
    mapa["903988"] = 8;

    for(int i=0;i<9;i++) f[i].init();

    for(int i=0;i<data.size();i++){
        f[ mapa[ data[i][2]] ].T++;
        if(  data[i][5] == "M"  ) f[ mapa[ data[i][2]] ].M++;
        else                      f[ mapa[ data[i][2]] ].F++;

        if( data[i][7] == "Y"){
            f[ mapa[ data[i][2]] ].sT++;
            if(  data[i][5] == "M"  ) f[ mapa[ data[i][2]] ].sM++;
            else                      f[ mapa[ data[i][2]] ].sF++;
        }
        
    }

    printEst();
    printM();
    printF();
    printMS();
    printFS();
    
}

//TODO : PRINTS
void print(){

    for(int i=0;i<data.size();i++){
        cout << data[i] << endl;
    }

}

int main(){

    leitura();
    
    contStructure();
    //print();

    return 0;
}