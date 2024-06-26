#include "Knjiga.h"
#include "Kupac.h"
#include "Osoba.h"
#include "Pisac.h"
#include "Util.h"
#include "ImePrezimeExc.h"
#include "BrojTelefonaExc.h"
#include <memory>
#include <fstream>
#include <vector>

using namespace std;

int main() {
    string imeKupca, prezimeKupca, brojTelefonaKupca,
        sifra, nastavakKupovine, potvrdaKupovine,
        potvrdaTransakcije, pregledTransakcija;

    int godineKupca, brojTransakcije = 1;

    ofstream outfile;
    ifstream infile;

    vector<unique_ptr<Pisac>> pisci = Pisac::ucitajPisce("pisac.txt");

    vector<unique_ptr<Knjiga>> knjige = Knjiga::ucitajKnjige("knjiga.txt", pisci);

    for (auto& knjiga : knjige) {
        Knjiga::dodajKnjigu(move(knjiga));
    }

    Knjiga::stampajListuKnjiga();

    unique_ptr<Kupac> kupac = make_unique<Kupac>();


    while (true) {
        try {
            cout << "\nUnesite vase ime : ";
            cin >> imeKupca;
            kupac->setIme(imeKupca);
            cout << endl;

            if (Util::samoSlova(imeKupca) == 0) {
                throw ImePrezimeExc();
            }

            cout << "Unesite vase prezime : ";
            cin >> prezimeKupca;
            kupac->setPrezime(prezimeKupca);
            cout << endl;

            if (Util::samoSlova(prezimeKupca) == 0) {
                throw ImePrezimeExc();
            }

            break;
        }
        catch (const exception& e) {
            cout << e.what() << endl;
            cout << "Molimo vas da ponovo unesete ime i prezime." << endl;
        }
    }

 
    while (true) {
        cout << "Unesite Broj godina : ";
        cin >> godineKupca;

        if (cin.fail() || godineKupca <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Neispravan unos. Molimo vas da ponovo unesete broj godina." << endl;
        }
        else {
            kupac->setGodine(godineKupca);
            cout << endl;
            break;
        }
    }



    while (true) {
        try {
            cout << "Unesite Broj telefona : ";
            cin >> brojTelefonaKupca;
            kupac->setBrojTelefona(brojTelefonaKupca);
            cout << endl;

            if (Util::samoBrojevi(brojTelefonaKupca) == 0) {
                throw BrojTelefonaExc();
            }

            break;
        }
        catch (const exception& e) {
            cout << e.what() << endl;
            cout << "Molimo vas da ponovo unesete broj telefona." << endl;
        }
    }

    cout << "Da li zelite da kupite knjigu? ";
    cin >> potvrdaKupovine;
    cout << endl;

    if (potvrdaKupovine == "da" || potvrdaKupovine == "DA" || potvrdaKupovine == "Da") {
        do {
            while (true) {
                cout << "Unesite sifru knjige koju zelite da kupite ";
                cin >> sifra;
                cout << endl;

                float racun = Knjiga::kupiKnjigu(godineKupca, sifra);
                if (racun == 0.0f) {
                    cout << "Ne postoji knjiga sa unetom sifrom. Molimo unesite ponovo." << endl;
                }
                else {
                    cout << "Cena knjige " << Knjiga::imeKnjigePoSifri(sifra) << " je " << racun << "RSD" << endl;

                    cout << "Da li zelite da kupite ovu knjigu? " << endl;
                    cin >> potvrdaTransakcije;
                    if (potvrdaTransakcije == "da" || potvrdaTransakcije == "Da" || potvrdaTransakcije == "DA") {
                        outfile.open("transakcije.txt", ios_base::trunc);
                        outfile << "Broj transakcije: " << brojTransakcije << endl;
                        outfile << "Kupac: " << kupac->getIme() << " " << kupac->getPrezime() << " " << kupac->getGodine() << " godina, Broj Telefona: " << kupac->getBrojTelefona() << endl;
                        outfile << "Kupio je knjigu: " << Knjiga::imeKnjigePoSifri(sifra) << " po ceni od " << racun << " Dinara" << endl;
                        outfile << "---------------------------------------------------------" << endl;
                        brojTransakcije++;
                        outfile.close();
                    }
                    break;
                }
            }

            cout << "Da li zelite da nastavite kupovinu ";
            cin >> nastavakKupovine;
            cout << endl;
        } while (nastavakKupovine == "da" || nastavakKupovine == "DA" || nastavakKupovine == "Da");
    }

    cout << "Da li zelite da pregledate izvrsene transakcije ?" << endl;
    cin >> pregledTransakcija;
    cout << endl;
    if (pregledTransakcija == "da" || pregledTransakcija == "DA" || pregledTransakcija == "Da") {
        infile.open("transakcije.txt");
        if (infile.is_open()) {
            string line;
            while (getline(infile, line)) {
                cout << line << endl;
            }
            infile.close();
        }
        else {
            cout << "Fajl transakcije.txt ne moze biti otvoren." << endl;
        }
    }

    return 0;
}
