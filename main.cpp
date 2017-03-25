#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <vector>
#include <fstream>
using namespace std;

//Design of file will be as following:
//id$name$type$price$amount$shelfNumber

char filename[] = "products.txt";
int recourdlength = 50;

void split(string &txt, vector<string> &strs){
    int pos = txt.find( '$' );
    int sPos = 0;
    strs.clear();
    while( pos != string::npos ) {
        strs.push_back( txt.substr( sPos, pos - sPos  ) );
        sPos = pos + 1;
        pos = txt.find( '$', sPos );
    }   
    strs.push_back( txt.substr( sPos, pos - sPos  ) );
}

float strtofloat(string s){ 
    char d[100];
    char* pEnd;
    float f1;
    strcpy(d, s.c_str());
    return strtof(d, &pEnd);
}

class product{
    public:
        string id,name,type;
        float price;
        int amount,shelfNumber;
    product(){ } 
    product(string s){ 
        vector<string> temp;
        split(s,temp);
        id = temp[0];
        name = temp[1];
        type= temp[2];  
        price= strtofloat(temp[3]);
        amount = atoi(temp[4].c_str());
        shelfNumber = atoi(temp[5].c_str());
    } 
    string getString(){
        string s;
        s += this->id + "$";
        s += this->name + "$";
        s += this->type + "$";
        s += to_string(this->price) + "$";
        s += to_string(this->amount) +"$";
        s += to_string(this->shelfNumber) +"$";
        while (s.length()<recourdlength)
            s.append("0"); ///make record recourdlength digits 
        return s;
    }
    void incAmount(){
        amount++;
    }
    void update(){
        cout <<"crunt data :\n ";
        cout<<setw(20)<<"id : "<<this->id<<setw(25)<<"name :"<<this->name<<setw(15)<<"price : "<<this->price;
        cout<<setw(20)<<"type :"<<this->type<<setw(20)<<"amount : "<<this->amount<<setw(15)<<"SN :"<<this->shelfNumber;
        cout <<"\nEnter id :";
        cin >> this->id ;
        cout <<"Enter name : ";
        cin >> this->name;
        cout <<"enter price : ";
        cin >> this->price;
        cout << "enter type : ";
        cin >> this->type;
        cout << "amount : ";
        cin >> this->amount;
        cout <<"shelfNumber : ";
        cin >> this->shelfNumber;
    }
};

void update(long int pos,product &p){
    fstream file;
    file.open(filename);
    file.seekg(pos);
    file.seekp(pos);
    string s = p.getString();
    file << s << "\n";
    file.close();
}

short check(vector<string> &key){
    string line;
    fstream file;
    file.open(filename);
    while (getline(file, line)){
        product p = product(line);
            if(p.id == key[0]){
                if(p.name == key[1]){
                    p.incAmount();
                    int long ps = file.tellg();
                    file.close();
                    update(ps-recourdlength-1,p);
                    return 1; //found id and name
                }
                return 2;  //found id
            }
    }
    return 0; // not found 
}
//do search by id .name or type in the file and return products found
//TypeOfSearch: 0 = id , 1 = name , 2= type
void searchAndGet(short TypeOfSearch,string &key,vector<product> &foundProducts){
    string line;
    fstream file;
    file.open(filename);
    foundProducts.clear();
    while (getline(file, line)){
        product p = product(line);
            if (TypeOfSearch == 0){ //id
                if(p.id == key)
                    foundProducts.push_back(p);
            }else if(TypeOfSearch == 1){// name
                if(p.name == key)
                    foundProducts.push_back(p);
            }else if (TypeOfSearch == 2) // type
                if(p.type == key)
                     foundProducts.push_back(p);
    }
    file.close();
}
// get start of record in file .. need for modify
long int searchPostion(string &key){
    string line;
    long int location;
    fstream file;
    file.open(filename);
    while (getline(file, line)){
        product p = product(line);
            if (p.id == key){
                location = file.tellg();
                file.close();
                return location - recourdlength-1; //start of record 
            }
    }
    file.close();
    return -1;
}
void deleteRecord(string &key){
    string line;
    fstream file,temp;
    file.open(filename);
    temp.open("temp.txt",ios::out);
    while (getline(file, line)){
        product p = product(line);
            if (p.id != key)
                temp << line <<"\n";
    }
    temp.close();
    file.close();
    remove(filename);
    rename("temp.txt", filename);
}

void display(vector<product> &p){
    cout<<setw(13)<<"id"<<setw(15)<<"name"<<setw(10)<<"price";
    cout<<setw(15)<<"type"<<setw(15)<<"amount"<<setw(20)<<"shelfNumber\n\n";
    for (int i=0; i<p.size() ;i++){
        cout<<setw(13)<<p[i].id<<setw(15)<<p[i].name<<setw(10)<<p[i].price;
        cout<<setw(15)<<p[i].type<<setw(15)<<p[i].amount<<setw(20)<<p[i].shelfNumber;
        cout<<endl;
    }
}
void reade(vector<product> &ps){
    string line;
    ps.clear();
    fstream file;
    file.open(filename);
    while (getline(file, line)){
        product p = product(line);
        ps.push_back(p);
    }
    file.close();
}
void write(){
    vector<string> te;
    product b = product();
    cout <<"Enter id : ";
    cin >> b.id ;
    te.push_back(b.id);
    cout <<"Enter name : ";
    cin >> b.name;
    te.push_back(b.name);
    int rs = check(te);
    if(rs==1)
        cout <<"   found .. increment amount \n";
    else if(rs == 2)
        cout <<"   The id must be unique .";
    else {
        cout <<"enter price : ";
        cin >> b.price;
        cout << "enter type : ";
        cin >> b.type;
        cout << "amount : ";
        cin >> b.amount;
        cout <<"shelfNumber : ";
        cin >> b.shelfNumber;
        string s = b.getString();
        if (s.size()>recourdlength){
            cout <<"your record is more than "<<recourdlength<<" digits\nCant fit.. exit!";
        }else{
            fstream file;
            file.open(filename,ios::app);
            file << s << "\n";
            file.close();
            cout <<"your record  " << s <<"\n";
            cout <<"entred ...";
        }
    }
}
int main(){
    vector<product> products;
    cout <<"Enter choice :\n0: exit\n1: display products\n2: write products data\n3: search\n4: update\n5: delete\n";
    int choice; 
    while(cin >> choice){ 
        if(choice == 0){
                exit(0);
        }else if(choice == 1){
            reade(products);
            if(products.size()>0)
                display(products);
            else 
                cout <<"no data to show ..";
        }else if(choice == 2){
                write();
        }else if(choice ==3){
            cout <<"search by :\n\t0:id \t1:nam\t2:type :  ";
            int cho;
            cin >> cho;
            string key;
            if(cho== 0){
                cout <<"enter an id :";
                cin >> key;
                searchAndGet(cho,key,products);
            }else if(cho == 1){
                cout <<"enter a neme :";
                cin >> key;
                searchAndGet(cho,key,products);
            }else if(cho== 2){
                cout <<"enter the type :";
                cin >> key;
                searchAndGet(cho,key,products);
            }
            if (products.size()>0)
                display(products);
            else
                cout<<"no data found ";
        }else if(choice ==4){
            string key,record;
            cout <<"enter an id :";
            cin >> key;
            long int pos = searchPostion(key);
            if (pos != -1){
                fstream file;
                file.open(filename);
                file.seekp(pos);
                file.seekg(pos);
                getline(file, record);
                file.close();
                product p = product(record);
                p.update();
                key = p.id;
                if(searchPostion(key)>-1)
                    cout <<"entred key is already used";
                else
                    update(pos,p);
                    cout <<"updated..";
            }else
                cout <<"record not found to update";
        }else if(choice == 5){
            string key;
            cout <<"enter id of record to remove :";
            cin >> key;
            deleteRecord(key);
        }else{    
            cout << "no entry .. quiting ..." << endl;
            exit(0);
        }
        getchar();
        cout <<"\n\t0: exit\t\t1: display products\t2: write products data\t3: search\t4: update\t5: delete\n";
    }
    return 0;
}
