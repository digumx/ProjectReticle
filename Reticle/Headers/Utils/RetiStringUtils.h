#ifndef RETISTRINGUTILS_H_INCLUDED
#define RETISTRINGUTILS_H_INCLUDED

#include <string>
#include <vector>


/// Tokenizes str using sep as a seperator, and stores list of tokens as strings in toks.
/// Clears toks before adding new members.
void RetiTokenizeString(const std::string& str, char sep, std::vector<std::string>& toks);

#endif // RETISTRINGUTILS_H_INCLUDED
