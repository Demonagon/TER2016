class DomaineIntervalle {
	int _taille;
	int _nb_valeurs;
	bool *_presente;
	bool _affectee;
	int _valeur;
 public:	
	void alloue (int t) {
		_taille = t;
		_presente = new bool [_taille];
		init ();
	}
	void init () {
		for (int i = 0; i < _taille; i++) {
			_presente [i] = true;
		}
		_nb_valeurs = _taille;
		_affectee = false;
	}
	int taille () { return _taille; }
	int nb_valeurs () { return _nb_valeurs; }
	bool affectee () { return _affectee; }
	int valeur () { return _valeur; }
	bool presente (int v) { return _presente [v]; }
	bool ote (int v) { 
		if (_presente [v]) {
			_presente [v] = false;
			_nb_valeurs--;
			return true;
		}
		return false;
	}
	void remet (int v) {
		_presente [v] = true;
		_nb_valeurs++;
	}
	void affecte (int v) {
		_affectee = true;
		_valeur = v;
	}
	void desaffecte () {
		_affectee = false;
	}
};

#include<iostream>

using namespace std;

const int TailleMax = 25;

int tailleCarre = 3, taille = tailleCarre * tailleCarre;

DomaineIntervalle var [TailleMax] [TailleMax];

void init () {
	for (int i = 0; i < taille; i++) {
		for (int j = 0; j < taille; j++)
			var [i] [j].alloue (taille);
	}
}

bool consistant (int i, int j, int val) {
	/* une seule fois la meme valeur par ligne */
	for (int ii = 0; ii < taille; ii++)
		if (ii != i)
			if (var [ii] [j].affectee ())
				if (var [ii] [j].valeur () == val)
					return false;
	/* une seule fois la meme valeur par colonne */
	for (int jj = 0; jj < taille; jj++)
		if (jj != j)
			if (var [i] [jj].affectee ())
				if (var [i] [jj].valeur () == val)
					return false;
	/* une seule fois la meme valeur par carre */
	int starti = tailleCarre * (i / tailleCarre), 
		startj = tailleCarre * (j / tailleCarre);
	for (int ii = starti; ii < starti + tailleCarre; ii++)
		for (int jj = startj; jj < startj + tailleCarre; jj++)
			if ((jj != j) && (ii != i))
	if (var [ii] [jj].affectee ())
		if (var [ii] [jj].valeur () == val)
			return false;
	return true;
}

DomaineIntervalle * choisitVariable (int & i, int & j) {
	for (i = 0; i < taille; i++)
		for (j = 0; j < taille; j++)
			if (!var [i] [j].affectee ())
				return &var [i] [j];
	return NULL;
}

int enumereValeurs (int i, int j, int *val) {
	int nb = 0;
	for (int k = 0; k < taille; k++)
		if (var [i] [j].presente (k)) {
			val [nb] = k;
			nb++;
		}
	return nb;
}

bool backtrack () {
	int i, j;
	DomaineIntervalle *d = choisitVariable (i, j);
	if (d == NULL) return true;

	int * val = new int[9];

	enumereValeurs(i, j, val);
	
	for(int k = 0; k < d->nb_valeurs(); k++) {
		if ( consistant (i, j, val[k]) ) {
			d->affecte(val[k]);
			if( backtrack() ) return true;
			d->desaffecte();
		}
	}
}

void afficheSolution () {
	for (int i = 0; i < taille; i++) {
		for (int j = 0; j < taille; j++)
			if (var [i] [j].affectee ())
				cout << var [i] [j].valeur () + 1 << " ";
			else
				cout <<	"	";
				cout << endl;
	}
}

int main (int argc, char **argv) {
	init ();
	if (backtrack ())
		afficheSolution ();
}
