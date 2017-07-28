#include <Utils/RetiUtilities.h>
#include <Core/RetiLog.h>

using namespace std;

void RetiUtilities::LoadFileToString(string& str, ifstream& file)
{
    str = "";
    string ln;
    if(!file.is_open()) return;
    getline(file, ln);
    while(getline(file, ln)) str = str + ln;
    return;
}

string RetiUtilities::SubstrByChar(char bch, char ech, size_t& start, const string& str)
{
    size_t bpos;
    if((bpos = str.find_first_of(bch, start)) == string::npos)
    {
        RetiLog::logln("WARNING: Expected " + string(1, bch) + "After" + to_string(start));
        return "";
    }
    size_t epos;
    if((epos = str.find_first_of(ech, bpos)) == string::npos)
    {
        RetiLog::logln("WARNING: Unmatched " + string(1, bch) + "At" + string(1, bpos));
        return "";
    }
    start = epos;
    return str.substr(bpos+1, epos-bpos-1);
}

void RetiUtilities::TokenizeString(const string& data, char ch, vector<string>& out)
{
    #ifdef DEBUG_CODE
    RetiLog::logln("Breaking string " + data);
    #endif // DEBUG_CODE
    size_t pos = 0;
    size_t prev = 0;
    while((pos = data.find_first_of(ch, prev)) != string::npos)
    {
        out.push_back(data.substr(prev, pos-prev));
        prev = pos + 1;
    }
}

string RetiUtilities::TrimString(const string& str, const string& charset)
{
    size_t bg = str.find_first_not_of(charset);
    size_t nd = str.find_last_not_of(charset);
    return str.substr(bg, nd-bg+1);
}
