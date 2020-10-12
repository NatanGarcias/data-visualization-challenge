#include<bits/stdc++.h>
using namespace std;

template<typename T> ostream& operator<<(ostream &os, const vector<T> &v) { os << "{"; for (typename vector<T>::const_iterator vi = v.begin(); vi != v.end(); ++vi) { if (vi != v.begin()) os << ", "; os << *vi; } os << "}"; return os; }

#define fi first
#define se second

vector<vector<string>> data;
ofstream out3("data/Nuvem.txt");

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
    string l[17] = {"alcohol","psychosis","anxiety-non-trauma","somatic_disorder","eating","bipolar spectrum_illness","depression",
                    "interpersonal trauma","PD-Cluster_C-anxiety","PD-Cluster_B-emotional","PD","Impulse_control_disorder","obesity",
                    "cardiovascular","COPD","asthma","immune-autoimmune" };
    int v[17];
    int T;

    void init(){
        T = 0;
        memset(v, 0, sizeof(v));
    }

    void print(int k){

        ofstream out("data/Familia_" + to_string(k+1) + ".csv");
        
        for(int i=0;i<17;i++) out << l[i] << ",\n"[i == 16];
        for(int i=0;i<17;i++) out << v[i] << ",\n"[i == 16];
        for(int i=0;i<17;i++) out << fixed  << setprecision(3) << 100 * v[i]/(T*1.0) << ",\n"[i == 16];
        for(int i=0;i<17;i++) out <<  (int) (100 * v[i]/(T*1.0)) << ",\n"[i == 16];

        out.close();

        ofstream out2("data/Nuvem_Fam_" + to_string(k+1) + ".txt");

        for(int i=0;i<17;i++){
            for(int j=0;j<v[i];j++){
                out2 << l[i] << " ";
                out3 << l[i] << " ";
            }
            if(v[i]) out2 << endl;
            if(v[i]) out3 << endl;
        } 
        out2 << endl;
        out3 << endl << endl;
        
        out2.close();
    }
};

Familia f[9];

void printAtr(int k){

    ofstream out("data/Atributos_" + to_string(k+1) + ".csv");

    for(int i=0;i<9;i++) out << "Família_" << i+1 << ",\n"[i == 8];
    for(int i=0;i<9;i++) out << f[i].v[k] << ",\n"[i == 8];
    for(int i=0;i<9;i++) out << fixed  << setprecision(3) << 100 * f[i].v[k]/(f[i].T*1.0) << ",\n"[i == 8];
    for(int i=0;i<9;i++) out <<  (int) (100 * f[i].v[k]/(f[i].T*1.0)) << ",\n"[i == 8];

    out.close();
}

void contAttr(){

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
        f[ mapa[ data[i][0]] ].T++;

        for(int j=12;j<=59;j++){
            if( data[i][j] == "1"){
                f[ mapa[ data[i][0]] ].v[(j-12)/3]++;
            }
        }
    }

    for(int i=0;i<9;i++) {
        f[i].print(i);
    }

    for(int i=0;i<17;i++){
        printAtr(i);
    }
}

//TODO : PRINTS
void print(){

    for(int i=0;i<data.size();i++){
        cout << data[i] << endl;
    }

}

int main(){

    leitura();
    
    contAttr();
    
    return 0;
}