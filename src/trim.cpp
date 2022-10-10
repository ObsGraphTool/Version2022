/*
 * Fichier : trim.cpp
 *
 * Les fonctions suivantes permettent de supprimer les caractères blancs 
 * (espaces, tabulations, sauts de ligne, tabulations...) en début et/ou fin de 
 * chaînes (std::string).
 *
 * http://qanuq.com/index.php?post/2009/06/05/Supprimer-les-espaces-en-début-et-en-fin-de-chaîne
 *
 */
 
#include "trim.h"
 
// Les fonctions trim, rtrim et ltrim prennent une référence sur une chaîne 
// en paramètre d'entrée. Ce paramètre d'entrée sera modifié s'il y a lieu.
// La chaîne modifiée est également retournée sous forme de référence afin 
// de permettre une utilisation chaînée comme do_something(trim(ma_chaine)).
 
// Chacune de ces fonctions existe aussi avec un paramètre constant. La 
// référence en entrée est constante, et le retour de la fonction est une copie
// modifiée de cette chaîne.
 
// Supprime les espaces à gauche de la chaîne.
string& ltrim(string& s)
{
	// Parcours des caractères de la chaîne, jusqu'à trouver un caractère qui 
	// n'est pas un caractère blanc.
	string::iterator it = s.begin();
	while (it != s.end())
	{
		if (!isspace(*it)) break;
		++it;
	}
	// Effacement des caractères allant du début à la position trouvée.
	s.erase(s.begin(), it);
	return s;
}
 
// Supprime les espaces à droite de la chaîne.
string& rtrim(string& s)
{
	// Parcours à l'envers des caractères de la chaîne, jusqu'à trouver un 
	// caractère qui n'est pas un caractère blanc.
	string::reverse_iterator it = s.rbegin();
	while (it != s.rend())
	{
		if (!isspace(*it)) break;
		++it;
	}
	// Différence entre l'itérateur de fin et celui tout juste trouvé.
	string::difference_type diff = s.rend() - it;
	// Effacement des caractères allant du (début + diff) à la fin.
	s.erase(s.begin() + diff, s.end());
	return s;
}
 
// Supprime les espaces à gauche et à droite de la chaîne.
string& trim(string& s)
{
	return ltrim(rtrim(s));
}
 
// Version de trim avec chaîne constante.
string trim(const string& s)
{
	string str = s;
	return trim(str);
}
 
// Version de rtrim avec chaîne constante.
string rtrim(const string& s)
{
	string str = s;
	return rtrim(str);
}
 
// Version de ltrim avec chaîne constante.
string ltrim(const string& s)
{
	string str = s;
	return ltrim(str);
}
