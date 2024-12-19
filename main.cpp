#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Graf
{
	int id_wierzcholka;
	vector<int> nastepnicy;
};

struct GrafPoPrzeksztalceniu
{
	int id_wierzcholka;
	vector<int> nastepnicy;
};

void wczytajGraf(vector<Graf>& graf);
bool czyKrawedzieWielokrotne(vector<Graf>& graf);
bool sprzezenieNastepnikow(vector<Graf>& graf);
vector<pair<int, int>> zliczenie(vector<Graf>& graf, vector<int> pary_pobrane);
vector<pair<int,int>> relacjeWierzcholkow(vector<Graf>& graf);
vector<int> wierzcholkiGrafu(vector<Graf>& graf);
vector<pair<int, int>> przeksztalcenieGrafu(vector<Graf>& graf);
void zapisz(vector<pair<int, int>>& graf_nowy);
void wczytajGrafPoZapisaniu(vector<GrafPoPrzeksztalceniu>& graf);
bool czyKrawedzieWielokrotneDlaNowego(vector<GrafPoPrzeksztalceniu>& graf);
bool szukaniePary(const vector<pair<int, int>>& wektorPar, const pair<int, int>& szukanaPara);


void wczytajGraf(vector<Graf>& graf)
{
	fstream plik;
	plik.open("input.txt", ios::in);
	if (plik.good() == false)
	{
		cout << "Nie odnaleziono pliku! Wychodze z programu..." << endl;
		exit(0);
	}
	
	string linia;
	char przecinek, dwukropek;
	int licznik = 0;
	int nr_linii, wierzcholek, nastepnik;

	while (getline(plik, linia)	&&	licznik	<=	20)
	{
		stringstream ss(linia);
		ss >> wierzcholek >> dwukropek;

		Graf nowyGraf;
		nowyGraf.id_wierzcholka = wierzcholek;

		while (ss >> nastepnik)
		{
			nowyGraf.nastepnicy.push_back(nastepnik);
			ss >> przecinek;
		}
		graf.push_back(nowyGraf);
		licznik++;
	}
	
	plik.close();
}

void czySprzezony(vector<Graf>& graf)
{
	
	if (czyKrawedzieWielokrotne(graf))
	{
		cout << "Graf nie jest sprzezony, poniewaz ma krawedzie wielokrotne" << endl;
		exit(0);
	}


	if(sprzezenieNastepnikow(graf) == false)
	{
		cout << "Graf nie spelnia warunkow sprzezenia nastepnikow! Graf nie jest sprzezony!" << endl;
		exit(0);
	}

	if (sprzezenieNastepnikow(graf) && czyKrawedzieWielokrotne(graf) == false)
	{
		cout << "Graf jest sprzezony!" << endl;
		return;
	}
}

bool czyKrawedzieWielokrotne(vector<Graf>& graf)
{
	for (int i = 0; i < graf.size(); i++)
	{
		
		set<int> odwiedzone;

		for (int j = 0; j < graf[i].nastepnicy.size(); ++j) {
			int nastepnik = graf[i].nastepnicy[j];

			if (odwiedzone.find(nastepnik) != odwiedzone.end()) {
				cout << "Znaleziono krawedz wielokrotna!" << endl;
				return true;
			}
			odwiedzone.insert(nastepnik);
		} 
		
	}
	return false;
}

bool czyKrawedzieWielokrotneDlaNowego(vector<GrafPoPrzeksztalceniu>& graf)
{
	for (int i = 0; i < graf.size(); i++)
	{
		const GrafPoPrzeksztalceniu& wierzcholek = graf[i];
		set<int> odwiedzone;

		for (int j = 0; j < wierzcholek.nastepnicy.size(); ++j) {
			int nastepnik = wierzcholek.nastepnicy[j];

			if (odwiedzone.find(nastepnik) != odwiedzone.end()) {
				cout << "Znaleziono krawedz wielokrotna w grafie oryginalnym!" << endl;
				return true;
			}
			odwiedzone.insert(nastepnik);
		}

	}
	return false;
}


bool sprzezenieNastepnikow(vector<Graf>& graf)
{
	for (int i = 0; i < graf.size(); ++i)
	{
		for (int j = i + 1; j < graf.size(); ++j)
		{
			if (!graf[i].nastepnicy.empty() && !graf[j].nastepnicy.empty())
			{
				set<int> nastepcyI(graf[i].nastepnicy.begin(), graf[i].nastepnicy.end());
				set<int> nastepcyJ(graf[j].nastepnicy.begin(), graf[j].nastepnicy.end());
				vector<int> intersection;
				set_intersection(nastepcyI.begin(), nastepcyI.end(), nastepcyJ.begin(), nastepcyJ.end(), back_inserter(intersection));

				if (!intersection.empty() && nastepcyI != nastepcyJ)
					return false;
			}
		}
	}
	return true;
}

vector<pair<int,int>> zliczenie(vector<Graf>& graf, vector<int> pary_pobrane)
{
	vector<pair<int, int>> pary_wierzcholkow_oryginalnie_nadane;
	for (int i = 0; i < pary_pobrane.size(); i++)
	{
			int first = 2 * (i + 1) - 1;
			int second = 2 * (i + 1);
			pary_wierzcholkow_oryginalnie_nadane.push_back(make_pair(first, second));
		
	}
	return pary_wierzcholkow_oryginalnie_nadane;
}

vector<pair<int,int>> relacjeWierzcholkow(vector<Graf>& graf)//TEN MA SENS ZE MA MNIEJ RELACJI
{
	vector<pair<int, int>> rel_wierz;
	for (int i = 0; i < graf.size(); i++)
	{
		if (graf[i].nastepnicy.size() > 1)
		{
			for (int j = 0; j < graf[i].nastepnicy.size(); j++)
			{
				int first = graf[i].id_wierzcholka;
				int second = graf[i].nastepnicy[j];
				rel_wierz.push_back(make_pair(first, second));
			}
		}
		else if (graf[i].nastepnicy.size() == 1)
		{ 
			int first = graf[i].id_wierzcholka;
			int second = graf[i].nastepnicy[0];
			rel_wierz.push_back(make_pair(first, second));
		}

	}
	return rel_wierz;
}

vector<int> wierzcholkiGrafu(vector<pair<int,int>> pobrane)
{	

	unordered_set<int> pobrane_wierzcholki;
	for (int i = 0; i < pobrane.size(); i++)
	{
			pobrane_wierzcholki.insert(pobrane[i].first);
			pobrane_wierzcholki.insert(pobrane[i].second);

	}
	vector<int> pobrane_w_wektor(pobrane_wierzcholki.begin(), pobrane_wierzcholki.end());
	sort(pobrane_w_wektor.begin(), pobrane_w_wektor.end());
	cout << "Ilosc wierz: " << pobrane_w_wektor.size() << endl;
	return pobrane_w_wektor;
}

bool szukaniePary(const vector<pair<int, int>>& wektorPar, const pair<int, int>& szukanaPara) {
	for (auto para : wektorPar) {
		if (para == szukanaPara)
			return true;
	}
	return false;
}

vector<pair<int, int>> przeksztalcenieGrafu(vector<Graf>& graf)
{
	vector<pair<int, int>> relacje = relacjeWierzcholkow(graf);
	vector<int> wierzcholki_pobrane = wierzcholkiGrafu(relacjeWierzcholkow(graf));
	vector<pair<int, int>> zliczone = zliczenie(graf, wierzcholki_pobrane);

	for (int i = 0; i < wierzcholki_pobrane.size(); ++i) {
		for (int j = 0; j < wierzcholki_pobrane.size(); ++j) {
			if (szukaniePary(relacje, { wierzcholki_pobrane[i], wierzcholki_pobrane[j] })) {
				auto luk = zliczone[i];
				auto lukZamieniany = zliczone[j];

				for (auto& krawedz : zliczone) {

					if (krawedz.first == lukZamieniany.first)
						krawedz.first = luk.second;
					if (krawedz.second == lukZamieniany.first)
						krawedz.second = luk.second;
				}

			}
		}
	}
	for (auto krawedz : zliczone) {
		cout << krawedz.first << ", " << krawedz.second << endl;
	}
	zapisz(zliczone);
	return zliczone;
}

void wypiszPary(const vector<pair<int, int>>& pary) {
	for (const auto& p : pary) {
		cout << "(" << p.first << ", " << p.second << ") ";
	}
	cout << endl;
}


void zapisz(vector<pair<int,int>>& graf_nowy)
{
	fstream plik2;
	plik2.open("output.txt", ios::out);
	if (plik2.good() == false)
	{
		cout << "Nie odnaleziono pliku! Wychodze z programu..." << endl;
		exit(0);
	}

	vector<int> wierzcholki_nowe;
	
	for (int i = 0; i < graf_nowy.size(); i++)
	{
		if (find(wierzcholki_nowe.begin(), wierzcholki_nowe.end(), graf_nowy[i].first) == wierzcholki_nowe.end())
		{
			wierzcholki_nowe.push_back(graf_nowy[i].first);
		}
	}

	for (int i = 0; i < wierzcholki_nowe.size(); i++)
	{
		vector<int> nastepniki_nowe;
		for (int j = 0; j < graf_nowy.size(); j++)
		{
			if (graf_nowy[j].first == wierzcholki_nowe[i])
			{
				nastepniki_nowe.push_back(graf_nowy[j].second);
			}
		}

		plik2 << wierzcholki_nowe[i] << ":";
		for (int k = 0; k < nastepniki_nowe.size(); k++)
		{
			plik2 << nastepniki_nowe[k];
			if (k < nastepniki_nowe.size() - 1) {
				plik2 << ",";
			}
		}
		plik2 << endl;

	}

	plik2.close();
	
}


void wczytajGrafPoZapisaniu(vector<GrafPoPrzeksztalceniu>& graf)
{
	fstream plik3;
	plik3.open("output.txt", ios::in);
	if (plik3.good() == false)
	{
		cout << "Nie odnaleziono pliku! Wychodze z programu..." << endl;
		exit(0);
	}

	string linia;
	char przecinek, dwukropek;
	int licznik = 0;
	int nr_linii, wierzcholek, nastepnik;

	while (getline(plik3, linia) && licznik <= 20)
	{
		stringstream ss(linia);
		ss >> wierzcholek >> dwukropek;

		GrafPoPrzeksztalceniu nowyGraf;
		nowyGraf.id_wierzcholka = wierzcholek;

		while (ss >> nastepnik)
		{
			nowyGraf.nastepnicy.push_back(nastepnik);
			ss >> przecinek;
		}
		graf.push_back(nowyGraf);
		licznik++;
	}
	plik3.close();
}


bool czyLiniowy(vector<GrafPoPrzeksztalceniu>& graf)
{
	if (czyKrawedzieWielokrotneDlaNowego(graf) == true)
	{
		cout << "Graf nie jest grafem liniowym (graf oryginalny nie jest 1-grafem)" << endl;
		return false;
	}
	else
	{
		cout << "Graf jest grafem liniowym!" << endl;
		return true;
	}
}


int main()
{
	vector<Graf> grafInput;
	wczytajGraf(grafInput); 
	czySprzezony(grafInput);
	vector<pair<int, int>> zliczone_po = przeksztalcenieGrafu(grafInput);
	wypiszPary(zliczone_po);
	vector<GrafPoPrzeksztalceniu> grafInputDrugi;
	wczytajGrafPoZapisaniu(grafInputDrugi);
	czyLiniowy(grafInputDrugi);
	
}