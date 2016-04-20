


typedef bool[4] * * AuthorizedValues;

typedef struct {
	AuthorizedValues autho_values;
	bool * values;
} Problem;

-> i = 0

-> backtrack(i)
-> modele_existe = false
-> si i > taille
	-> afficher modèle
	-> returner vrai
-> affecte 1 à la variable i
	-> si non consistant returner faux
	-> sinon si backtract(i + 1)
		modele_existe = true
-> affecte 0 à la variable i
	-> si non consistant returner faux
	-> sinon si backtract(i + 1)
		modele_existe = true
-> retourner modele_existe
