/*
 * Fichier : trim.h
 *
 * Les fonctions suivantes permettent de supprimer les caractères blancs 
 * (espaces, tabulations, sauts de ligne, tabulations...) en début et/ou fin de 
 * chaînes (std::string).
 *
 * http://qanuq.com/index.php?post/2009/06/05/Supprimer-les-espaces-en-début-et-en-fin-de-chaîne
 *
 */
#ifndef STRING_TRIM
#define STRING_TRIM
 
#include <string>
 
using std::string;
 
string& ltrim(string& s);
string& rtrim(string& s);
string& trim(string& s);
string trim(const string& s);
string rtrim(const string& s);
string ltrim(const string& s);
 
#endif
