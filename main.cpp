// program szyfruje/deszyfruje podany tekst zgodnie z szyfrem Vigenere'a.
// input i output tekstu mozliwy zarowno za pomoca konsoli, jak i plikow.
// klucz podaje sie wylacznie w konsoli, zgodnie z umowionymi zalozeniami.
// szyfrowanie/deszyfrowanie z pliku jest wykonywane po 'mergingu tekstu', aby zminimalizowac prosta mozliwosc odkrycia klucza,
// istniejaca przy szyfrowaniu linijka po linijce.

#include <iostream>
#include <fstream>

using namespace std;

void menu(char[]);//wyswietla menu poczatkowe (zaczytujac odpowiedzi do char wybor[3])
char atoA(char);//zamienia małe litery na wielkie, jak nazwa sama wskazuje
bool dWybor(char, bool);//sprawdza poprawnosc wcisnietych klawiszy przy wyborze przez uzytkownika
void odpWybor (char, bool);//odpowiada userowi (sam cout) jak sie pomylil przy wyborze
bool czyLitera(char);//sprawdza czy char jest litera angielskiego alfabetu
bool podanieWejscia(char, string, string&);//menu wyboru inputu tekstu do (de)szyfrowania
void podanieKlucza(string&);//coutuje prosbe o podanie klucza i go cinuje do zmiennej klucz
bool szyfDeszyf(char, string&, string);//(de)szyfruje tekst
string kluczoDestruktor(string);//zamienia klucz z wartosci int ASCII nadanych literom na wartosci przedzialu <0;25>, jak w specyfikacji szyfru
string kluczoWydluzacz(string, string);//wydluza klucz na dlugosc rowna ilosci liter tekstu do szyfrowania (pomija znaki niebedace literami alf. lac.)
bool czyPrzenosimy(char, char);//sprawdza czy zaczynamy alfabet od nowa przy szyfrowaniu
bool czyWracamy(char, char); //to samo co wyżej tylko dla deszyfracji
void szyfrarka(string, string&);//szyfruje tekst
void deszyfrarka(string, string&);//to samo co wyżej tylko dla deszyfracji
bool podanieWyjscia(char, string, string&);//menu wyboru outputu tekstu do (de)szyfrowania

int main()
{
    char wybor[3];
    menu(wybor);

    string tekst, klucz, plikWe, plikWy;

    if(!podanieWejscia(wybor[0], plikWe, tekst))
    {
        return(-1);
    }

    podanieKlucza(klucz);

    if(!szyfDeszyf(wybor[2], tekst, klucz))
    {
        return(-1);
    }


    if(!podanieWyjscia(wybor[1], plikWy, tekst))
    {
        return(-1);
    }

    return 0;
}

void menu(char wybor[])
{
    bool czyOK=false;
    while(!czyOK)
    {
        cout << "podaj pozadany sposob podania danych:\n\t(P)lik\n\t(K)onsola\n>>";
        cin >> wybor[0];
        odpWybor(wybor[0], false);
        czyOK=dWybor(wybor[0],false);
    }
    czyOK=false;
    while(!czyOK)
    {
        cout << "podaj pozadany sposob zapisu wyniku:\n\t(P)lik\n\t(K)onsola\n>>";
        cin >> wybor[1];
        odpWybor(wybor[1], false);
        czyOK=dWybor(wybor[1],false);
    }
    czyOK=false;
    while(!czyOK)
    {
        cout<<"wybierz pozadana funkcje:\n\t(S)zyfrowanie\n\t(D)eszyfrowanie\n>>";
        cin>>wybor[2];
        odpWybor(wybor[2],true);
        czyOK=dWybor(wybor[2],true);
    }
}

char atoA(char a)
{
    if(a>96)
        return a-32;
    return a;
}

bool dWybor(char a, bool x)
{
    if(x)
    {
        if(atoA(a)=='S'||atoA(a)=='D')
            return 1;
        return 0;
    }
    else
    {
        if(atoA(a)=='P'||atoA(a)=='K')
            return 1;
        return 0;
    }
}

void odpWybor (char wybor, bool x)
{
    if(!dWybor(wybor,x))
    {
        cout<<"PODANO NIEPRAWIDLOWY ZNAK!\n\n";
    }
}

bool czyLitera(char a)//sprawdza czy char jest litera angielskiego alfabetu
{
    if((a>64&&a<91)||(a<123&&a>96))
        return true;
    return false;
}

bool podanieWejscia(char wybor, string plikWe, string& tekst)
{
    cin.ignore();
    switch(atoA(wybor))
    {
        case 'K':
            cout<<"Podaj tekst do (de)szyfrowania: ";
            getline(cin,tekst);
            break;
        case 'P':
            cout<<"Podaj nazwe, pliku wejsciowego: ";
            getline(cin,plikWe);
            break;
        default:
            cout<<"BLA,D INPUTU";
            return 0;
    }
    if(plikWe.length()>0)
    {
        fstream weHandle(plikWe, ios::in);
        bool x=weHandle.good();
        if(!x)
        {
            cout << "blad. zadany plik nie istnieje. program konczy dzialanie.";
            return 0;
        }
        int i=0;
        while(!weHandle.eof())
        {
            string temp;
            getline(weHandle,temp);
            if(i>0)
                tekst+="\n";
            tekst+=temp;
            i++;
        }
        weHandle.close();
    }
    return 1;
}

void podanieKlucza(string& klucz)
{
    cout<<"Podaj klucz: ";
    cin>>klucz;
}

bool szyfDeszyf(char wybor, string& tekst, string klucz)
{
    switch(atoA(wybor))
    {
        case 'S':
            szyfrarka(klucz,tekst);
            break;
        case 'D':
            deszyfrarka(klucz,tekst);
            break;
        default:
            cout<<"BLAD INPUTU";
            return 0;
    }
    return 1;
}

string kluczoDestruktor(string klucz)
{
    for(int i=0; i<klucz.length(); i++)
    {
        if(klucz[i]>64&&klucz[i]<91)
            klucz[i]=klucz[i]-65;
        else
            klucz[i]=klucz[i]-97;
    }
    return klucz;
}

string kluczoWydluzacz(string klucz, string slowo)
{
    string axa;
    int dlugoscLiterowa=0;
    for(int i=0; i<slowo.length(); i++)
    {
        if(czyLitera(slowo[i]))
        {
            dlugoscLiterowa++;
        }
    }
    //cout<<"\tH"<<dlugoscLiterowa<<"H\n";
    for(int i=0; i<dlugoscLiterowa; i++)
    {
        axa+=klucz[i%klucz.length()];
    }
    return axa;
}

bool czyPrzenosimy(char a, char secret)//sprawdza czy zaczynamy alfabet od nowa
{
    int ai=a, si=secret;
    //cout<<"\t"<<ai<<"|"<<si;
    if((ai>64&&ai<91&&(ai+si)>90)||(ai>96&&ai<123&&(ai+si)>122))
        return true;
    return false;
}

bool czyWracamy(char a, char secret)
{
    int ai=a, si=secret;
    if((ai>64&&ai<91&&(ai-si)<65)||(ai>96&&ai<123&&(ai-si)<97))
        return true;
    return false;
}

void szyfrarka(string klucz, string& slowo)
{
    string axa;
    string kluczOstateczny=kluczoWydluzacz(kluczoDestruktor(klucz), slowo);
    int liczbaNieliter=0;
    for(int i=0; i<slowo.length(); i++)
    {
        if(czyLitera(slowo[i]))
        {
            char kluczyk=kluczOstateczny[i-liczbaNieliter];
            if(czyPrzenosimy(slowo[i], kluczyk))
                axa+=slowo[i]-26+kluczyk;
            else
                axa+=slowo[i]+kluczyk;
        }
        else
        {
            liczbaNieliter++;
            axa+=slowo[i];
        }
    }
    slowo=axa;
}

void deszyfrarka(string klucz, string& slowo)
{
    string axa;
    string kluczOstateczny=kluczoWydluzacz(kluczoDestruktor(klucz), slowo);
    int liczbaNieliter=0;
    for(int i=0; i<slowo.length(); i++)
    {
        if(czyLitera(slowo[i]))
        {
            char kluczyk=kluczOstateczny[i-liczbaNieliter];
            if(czyWracamy(slowo[i], kluczyk))
                axa+=slowo[i]+26-kluczyk;
            else
                axa+=slowo[i]-kluczyk;
        }
        else
        {
            liczbaNieliter++;
            axa+=slowo[i];
        }
    }
    slowo=axa;
}

bool podanieWyjscia(char wybor, string plikWy, string& tekst)
{
    cin.ignore();
    switch(atoA(wybor))
    {
        case 'K':
            cout<<tekst;
            break;
        case 'P':
            cout<<"Podaj pozadana nazwe pliku wynikowego: ";
            getline(cin,plikWy);
            break;
        default:
            cout<<"BLAD INPUTU";
            return 0;
    }

    if(plikWy.length()>0)
    {
        ofstream wyHandle(plikWy);
        wyHandle<<tekst;
        wyHandle.close();
    }

    return 1;
}