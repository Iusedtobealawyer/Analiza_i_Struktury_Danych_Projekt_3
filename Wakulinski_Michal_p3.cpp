#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

struct Sdane{
    string word;
    Sdane *next;
};

void printInstruction(){

    cout<<"Napisz 'instrukcja', zeby wystwietlic instruckje."<<endl;
    cout<<"Napisz 'zamien', zeby zamienic ze soba dwa ciagi znakow."<<endl;
    cout<<"Napisz 'wypisz', zeby wyswietli aktualny tekst."<<endl;
    cout<<"Napisz 'cofnij', zeby cofnac ostatnia zmiane tekstu."<<endl;
    cout<<"Napisz 'koniec', zeby zakonczyc dzialanie programu."<<endl;
    cout<<"Napisz 'zapisz', zeby zakonczyc dzialanie programu."<<endl;
    cout<<endl;
}

// tworzy liste poprzednich wersji tekstu, do wywolania funkcja undo
void createList(Sdane *&head, string text){

    string word;

    Sdane *present = head;

    Sdane *newElement = new Sdane;
    newElement->word = text;
    newElement->next = NULL;

    if(head == NULL){
        head = newElement;
    }else{
        while(present->next != NULL){
            present = present->next;
        }
        present->next = newElement;
    }
}
// wypisuje wszystkie poprzednie wersje tekstu
void printList(Sdane *head){
    Sdane *present = head;
    while(present != NULL){
        cout<<present->word<<endl;
        present = present->next;
    }
    cout<<endl;
}

void deleteList(Sdane *&head){

    Sdane *temp  = NULL;

    while(head!=NULL){
        temp = head;
        head = head->next;
        delete temp;
    }
}

void prefixKMP(string wordToReplace, int prefixArray[])
{
    int m = wordToReplace.length(), i, j;
    prefixArray[0] = -1;

    for ( i = 0,  j = -1; i < m;){
        while(j>=0 && wordToReplace[i] != wordToReplace[j]){
            j = prefixArray[j];
        }
        i++;
        j++;
        prefixArray[i] = j;
    }
}

string findAndReplace_KPM(string wordToReplace, string text, string wordToReplaceWith, Sdane *&head)
{
    string newText = "";
    int n = text.size();
    int m = wordToReplace.size();

    int prefixArray[m];

    prefixKMP(wordToReplace, prefixArray);

    int a = 0;
    int b = 0;

    for(a = 0, b = 0; b<m && a<n; a++, b++){
        while(b>=0 && text[a] != wordToReplace[b]){
            b = prefixArray[b];
        }
    }

    if(b == m){
        int startingIndex = a-m;
       int endingIndex = a;

       for(int k = 0; k<startingIndex; k++){
        newText +=text[k];
       }
       newText +=wordToReplaceWith;

       for(int k = endingIndex; k<n; k++){
        newText +=text[k];
       }

       createList(head, text);
    }else{
        newText = text;
    }

   return newText;
}

string undo(Sdane *&head){

    string previousText;

    Sdane *present = head;

    if(present->next != NULL){
        while(present->next->next != NULL){
            present = present->next;
        }

        previousText = present->next->word;
        delete present->next;
        present->next = NULL;
    }else{
        previousText=present->word;
        delete head;
        head = NULL;
    }

    return previousText;
}

string readFromFile(){
    string buffor;
    string fileName;
    ifstream inFile;
    string text;

    do{
        cout<<"Podaj nazwe pliku, z ktorego maja byc wczytany tekst: ";
        getline(cin,fileName);
        fileName = fileName +".txt";
        inFile.open(fileName.c_str());
    }while(!inFile.is_open());

    while(getline(inFile,buffor)){
        text +=buffor;
        text= text+"\n";
        if(buffor == ""){
            text= text+"\n";
        }
    }

    cout<<endl;
    inFile.close();
    return text;
}

void wrieToFile(string text){

    string toFile;

    cout<<"Podaj nazwe pliku, do ktorego ma byc zapisany tekst: ";
    getline(cin,toFile);
    toFile = toFile +".txt";

    ofstream outFile(toFile.c_str());

    outFile<<text;

    cout<<endl;
    cout<<"Tekst zostal zapisany w pliku o nazwie: "<<toFile<<endl;
    outFile.close();
}

int main()
{
    string text, wordToReplace, wordToReplaceWith, newText = "";
    Sdane *head = NULL;
    string option ="instrukcja";

    text = readFromFile();

    while(option != "koniec"){
        if(option == "instrukcja")
            printInstruction();

        if(option == "zamien"){

            cout<<"Podaj slowo do zamiany: ";
            getline(cin,wordToReplace);

            cout<<"Podaj na jakie slowo chcesz zamienic podany wyraz: ";
            getline(cin,wordToReplaceWith);
            newText = findAndReplace_KPM(wordToReplace, text, wordToReplaceWith, head);
            text = newText;
            cout<<endl;
        }

        if(option == "wypisz"){
            cout<<text<<endl;
        }

        if(option == "cofnij" && head != NULL){
            text = undo(head);
        }

        if(option == "lista"){
            printList(head);
            cout<<endl;
        }

        if(option == "zapisz"){
            wrieToFile(text);
            cout<<endl;
        }

        cout<<"Wybierze opcje: ";
        getline(cin,option);
        cout<<endl;
    }

    deleteList(head);

    return 0;
}
