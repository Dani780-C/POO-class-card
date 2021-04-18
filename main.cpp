#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Card{
    string nr_card;
    string nume_detinator;
    string data_expirare;
    int CVV;
    double credit;
    static int nr_carduri_std;
    static int nr_carduri_prem;
public:
    Card(){
        nr_card = "";
        nume_detinator = "Anonim";
        data_expirare = "";
        CVV = -1;
        credit = 0.0;
    }
    Card(string nrCard, string numeDetinator, string data_expirare, int CVV, double credit){
        this->nr_card = nrCard;
        this->nume_detinator = numeDetinator;
        this->data_expirare = data_expirare;
        this->CVV = CVV;
        this->credit = credit;
    }
    virtual ~Card(){
        nr_card = "";
        nume_detinator = "";
        data_expirare = "";
        CVV = -1;
        credit = 0.0;
    }
    Card(const Card &c){
        this->nr_card = c.nr_card;
        this->nume_detinator = c.nume_detinator;
        this->data_expirare = c.data_expirare;
        this->CVV = c.CVV;
        this->credit = c.credit;
    }
    Card& operator= (const Card& c){
        this->nr_card = c.nr_card;
        this->nume_detinator = c.nume_detinator;
        this->data_expirare = c.data_expirare;
        this->CVV = c.CVV;
        this->credit = c.credit;
    }
    void afisare(){
        cout<<this->nr_card<<endl;;
        cout<<this->nume_detinator<<endl;
        cout<<this->data_expirare<<endl;
        cout<<this->CVV<<endl;
        cout<<this->credit<<endl;
    }
    friend ostream& operator<< (ostream&, const Card&);
    friend istream& operator>> (istream&, Card&);

    virtual void extragere(double suma) = 0;

    void depunere(double suma){
        credit=credit+suma;
    }

    double get_credit(){
        return credit;
    }

    void set_credit(double val){
        credit = val;
    }

    void scade_credit(double suma){
        credit-=suma;
    }

    string get_nr_card(){
        return this->nr_card;
    }

    static void card_std(){
        nr_carduri_std++;
    }

    static void card_prem(){
        nr_carduri_prem++;
    }
};

int Card::nr_carduri_std = 0;
int Card::nr_carduri_prem = 0;

istream& operator>> (istream& in, Card& k) {
    cout<<"\n------------ Cititi datele cardului ------------\n"<<'\n';
    cout<<"Numarul cardului: ";
    in>>k.nr_card;
    cin.get();
    cout<<"Numele detinatorului: ";
    getline(in,k.nume_detinator);
    cout<<"Data expirarii cardului(DD-MM-YYYY): ";
    in>>k.data_expirare;
    cout<<"CVV(3cifre): ";
    in>>k.CVV;
    cout<<"Creditul(lei): ";
    in>>k.credit;
    return in;
}

ostream& operator<< (ostream& out, const Card& c){
    out<<"Numarul cardului: "<<c.nr_card<<'\n';
    out<<"Numele detinatorului: "<<c.nume_detinator<<'\n';
    out<<"Data expirarii cardului: "<<c.data_expirare<<'\n';
    out<<"CVV-ul: "<<c.CVV<<'\n';
    out<<"Credit: "<<c.credit<<'\n';
    return out;
}

class Card_standard: public Card{
    int limita_extragere;
    double comision_depasire_limita;
public:
    Card_standard(){
        this->limita_extragere = 0;
        this->comision_depasire_limita = 1;
    }
    Card_standard(string nrCard, string numeDetinator, string data_expirare, int CVV, double credit, int limitaExtragere = 0, double comisionDepasireLimita = 1):Card(nrCard,numeDetinator,data_expirare,CVV,credit){
        this->limita_extragere = limitaExtragere;
        this->comision_depasire_limita = comisionDepasireLimita;
    }
    ~Card_standard(){
        limita_extragere = 0;
        comision_depasire_limita = 0;
    }
    Card_standard(const Card_standard& c):Card(c){
        this->limita_extragere = c.limita_extragere;
        this->comision_depasire_limita = c.comision_depasire_limita;
    }
    Card_standard& operator= (const Card_standard& c){
        Card::operator=(c);
        this->limita_extragere = c.limita_extragere;
        this->comision_depasire_limita = c.comision_depasire_limita;
    }
    void afisare(){
        Card::afisare();
        cout<<this->limita_extragere<<endl;
        cout<<this->comision_depasire_limita<<endl;
    }
    friend ostream& operator<< (ostream&, const Card_standard&);
    friend istream& operator>> (istream&, Card_standard&);

    virtual void extragere(double suma){
        double credit_card_standard = this->get_credit();
        if(suma>credit_card_standard){
            cout<<"Credit insuficient!"<<'\n';
            return;
        }
        else if(suma>limita_extragere){
                double com = (suma - limita_extragere)*comision_depasire_limita/100;
                cout.setf(std::ios::fixed,std::ios::floatfield);
                cout<<"Comisionul de depasire a limitei este de "<<setprecision(2)<<com<<" lei.\n";
                this->scade_credit(suma+com);
                if(this->get_credit() < 0) this->set_credit(0);
                cout<<"Creditul dumneavoastra este de ";
                cout<<setprecision(3)<<this->get_credit()<<" lei.";
            }else{
                if(this->get_credit() < 0) this->set_credit(0);
                this->scade_credit(suma);
                cout<<"Creditul dumneavoastra este de ";
                cout.setf(std::ios::fixed,std::ios::floatfield);
                cout<<setprecision(3)<<this->get_credit()<<" lei.";
            }
        cout<<'\n';
    }

    int get_limita_extragere(){
        return this->limita_extragere;
    }
    double get_comision_depasire_limita(){
        return this->comision_depasire_limita;
    }
};

istream& operator>> (istream& in, Card_standard& c){
    in>>dynamic_cast<Card&>(c);
    cout<<"Fixati limita extragerii: ";
    in>>c.limita_extragere;
    cout<<"Fixati comisionul pentru depasirea limitei(%): ";
    in>>c.comision_depasire_limita;
    return in;
}

ostream& operator<< (ostream& out, const Card_standard& c){
    out<<dynamic_cast<const Card&>(c);
    out<<"Limita extragerii: "<<c.limita_extragere<<'\n';
    out<<"Comisionul depasirii limitei de extragere: "<<c.comision_depasire_limita<<'\n';
    return out;
}

class Card_premium: public Card_standard{
    double cashback;
public:
    Card_premium(){
        this->cashback = 0;
    }
    Card_premium(string nrCard, string numeDetinator, string data_expirare, int CVV, double credit,int limita_extragere, double comision_depasire_limita, double cashback = 0):Card_standard(nrCard,numeDetinator,data_expirare,CVV,credit,limita_extragere,comision_depasire_limita){
        this->cashback = cashback;
    }
    ~Card_premium(){
        cashback = 0;
    }
    Card_premium(const Card_premium& c):Card_standard(c){
        this->cashback = c.cashback;
    }
    Card_premium& operator= (const Card_premium& c){
        Card_standard::operator=(c);
        this->cashback = c.cashback;
    }

    void afisare(){
        Card_standard::afisare();
        cout<<this->cashback<<endl;
    }

    void extragere(double suma){
        double credit_card_premium = this->get_credit();
        double calcul_cashback = cashback/100*suma;
        if(suma>credit_card_premium){
            cout<<"Credit insuficient!"<<'\n';
            return;
        }
        else{
            double limita = this->get_limita_extragere();
            double comision = this->get_comision_depasire_limita();
            if(suma>limita){
                double com = (suma - limita)*comision/100;
                cout.setf(std::ios::fixed,std::ios::floatfield);
                cout<<"Comisionul de depasire a limitei este de "<<setprecision(2)<<com<<" lei.\n";
                this->scade_credit(suma+com-calcul_cashback);
                if(this->get_credit() < 0) this->set_credit(0);
                cout<<"Creditul dumneavoastra este de ";
                cout<<setprecision(3)<<this->get_credit()<<" lei.\n";
                cout<<"Cashback-ul: "<<setprecision(2)<<calcul_cashback;
            }else{
                if(this->get_credit() < 0) this->set_credit(0);
                this->scade_credit(suma-calcul_cashback);
                cout<<"Creditul dumneavoastra este de ";
                cout.setf(std::ios::fixed,std::ios::floatfield);
                cout<<setprecision(3)<<this->get_credit()<<" lei.\n";
                cout<<"Cashback-ul: "<<setprecision(2)<<calcul_cashback;
            }
            cout<<'\n';
        }
    }
    friend istream& operator>> (istream&, Card_premium&);
    friend ostream& operator<< (ostream&, const Card_premium&);
};

istream& operator>> (istream& in, Card_premium& c){
    in>>dynamic_cast<Card_standard&>(c);
    cout<<"Setati cashback-ul(%): ";
    in>>c.cashback;
    return in;
}

ostream& operator<< (ostream& out, const Card_premium& c){
    out<<dynamic_cast<const Card_standard&>(c);
    out<<"Cashback: "<<c.cashback<<'\n';
    return out;
}

void f1(){
    cout<<"Doriti sa creati un card standard sau unul premium?\n\n - cod 1 - card standard\n - cod 2 - card premium\n\n";
    cout<<"Cititi optiunea: ";
}

void afisare_optiuni_card(){
    cout<<"~~~~~~~ Bancomat deschis! ~~~~~~~~\n"<<'\n';
    cout<<" - cod 1 -- Depuneti bani pe card\n";
    cout<<" - cod 2 -- Extrageti bani de pe card\n";
    cout<<" - cod 3 -- Investigare sold\n";
    cout<<" - cod 4 -- Folositi alt card\n";
    cout<<" - cod 5 -- Afisati toate cardurile\n";
    cout<<" - cod 0 -- Parasiti bancomatul\n";
    cout<<'\n';
    cout<<"~~~~~~~~~~~~ING_Bank~~~~~~~~~~~~~\n";
}

bool verificare(int index, int index_prem, int index_std){
    if(index<0){
        cout<<"Un index negativ nu e valabil!\n";
        return false;
    }
    if(index%2 == 0 and index>index_std){
        cout<<"Acest card nu exista!\n";
        return false;
    }
    else if(index%2 == 1 and index>index_prem){
        cout<<"Acest card nu exista!\n";
        return false;
    }
    return true;
}

int main()
{
    vector<Card_standard> cds;
    vector<Card_premium> cdp;
    string run = "da";
    int option;
    int index;
    int index_std = 0;
    int index_prem = 1;
    while(run == "da"){    /// creeare carduri
        f1();
        cin>>option;
        switch(option){
            case 1:{    ///Card standard
                Card *a = new Card_standard;
                Card_standard *b = dynamic_cast<Card_standard*>(a);
                cin>>*b;
                cds.push_back(*b);
                cout<<'\n';
                cout<<"--------------------------------------------------------------------------\n";
                cout<<"Cardul standard a fost inregistrat!       INDEX -> "<<index_std<<'\n';
                cout<<"--------------------------------------------------------------------------\n";
                cout<<'\n';
                index_std+=2;
                b->card_std();
                break;
            }
            case 2:{    ///Card premium
                Card_standard *d = new Card_premium;
                Card_premium *c = dynamic_cast<Card_premium*>(d);
                cin>>*c;
                cdp.push_back(*c);
                cout<<'\n';
                cout<<"--------------------------------------------------------------------------\n";
                cout<<"Cardul premium a fost inregistrat!        INDEX -> "<<index_prem<<'\n';
                cout<<"--------------------------------------------------------------------------\n";
                cout<<'\n';
                index_prem+=2;
                c->card_prem();
                break;
            }
            default:{
                break;
            }
        }
        cout<<"Doriti sa mai faceti un card?( da / nu): ";
        string raspuns;
        cin.get();
        cin>>raspuns;
        if(raspuns == "nu"){
                cout<<"\nFolositi un card creat. Pentru asta folositi indexul unic al cardului.\nCiteste index unic: ";
                cin>>index;
                run = "nu";
        }
        cout<<'\n';
    }

    run = "da";
    index_prem-=2;
    index_std-=2;

    while(verificare(index,index_prem,index_std) == false){
        cout<<"Alegeti un index corect: ";
        cin>>index;
        cout<<'\n';
    }
    while(run == "da"){
        afisare_optiuni_card();
        cout<<"\nCititi optiunea: ";
        cin >> option;
        switch(option){
            case 1:{
                cout<<"Introduceti suma depusa: ";
                double suma;
                cin>>suma;
                if(index%2 == 0){
                    cds[index/2].depunere(suma);
                    cout.setf(std::ios::fixed,std::ios::floatfield);
                    cout<<"Suma de "<<suma<<" lei a fost depusa in contul bancar "<<cds[index/2].get_nr_card()<<". TOTAL CREDIT: ";
                    cout<<setprecision(3)<<cds[index/2].get_credit();
                    cout<<" lei\n";
                }
                else{
                    cdp[index/2].depunere(suma);
                    cout.setf(std::ios::fixed,std::ios::floatfield);
                    cout<<"Suma de "<<suma<<" lei a fost depusa in contul bancar "<<cdp[index/2].get_nr_card()<<". TOTAL CREDIT: ";
                    cout<<setprecision(3)<<cdp[index/2].get_credit();
                    cout<<" lei\n";
                }
                cout<<'\n';
                break;
            }
            case 2:{
                cout<<"Introduceti suma extrasa: ";
                double suma;
                cin>>suma;
                if(index%2 == 0){
                    cds[index/2].extragere(suma);
                }
                else{
                    cdp[index/2].extragere(suma);
                }
                cout<<'\n';
                break;
            }
            case 3:{
                if(index%2 == 0) {
                    cout<<"Creditul dumneavoastra este de ";
                    cout.setf(std::ios::fixed,std::ios::floatfield);
                    cout<<setprecision(3)<<cds[index/2].get_credit()<<" lei.\n";
                }else {
                    cout<<"Creditul dumneavoastra este de ";
                    cout.setf(std::ios::fixed,std::ios::floatfield);
                    cout<<setprecision(3)<<cdp[index/2].get_credit()<<" lei.\n";
                }
                cout<<'\n';
                break;
            }
            case 0:{
                cout<<"\n          -- !Nu uitati cardul in bancomat! -- \n";
                run = "nu";
                break;
            }
            case 4:{
                cout<<"Cititi noul index: ";
                int vechi = index;
                cin>>index;
                cout<<'\n';
                while(verificare(index,index_prem,index_std) == false){
                    cout<<"Alegeti un index corect: ";
                    cin>>index;
                    cout<<'\n';
                }
                break;
            }
            case 5:{
                cout<<"\n ================== Afisare Carduri ================== \n"<<'\n';
                for(int i = 0; i < cds.size(); i++){
                    cout<<cds[i]<<'\n';
                }
                for(int i = 0; i < cdp.size(); i++){
                    cout<<cdp[i]<<'\n';
                }
                cout<<" ================== Sfarsit Afisare ===================== \n"<<'\n';
                break;
            }
            default:{
                break;
            }
        }
    }
    cds.clear();
    cdp.clear();
    return 0;
}
