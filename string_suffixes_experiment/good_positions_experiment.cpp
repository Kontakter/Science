#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <fstream>

std::string getRandomStr(int len, int range) {
    std::string res;
    for (int i = 0; i < len; ++i) {
        res = res + static_cast<char>(('a' + (rand() % range)));
    }
    return res;
}

std::vector<int> findGoodPositions(const std::string& str, const std::vector<std::string>& subStrings) {
    std::set<int> positions;
    for (const auto& substr: subStrings) {
        std::vector< std::pair<std::string, int> > suffixes;
        for (int i = 0; i < str.length(); ++i) {
            suffixes.push_back(make_pair(str.substr(i) + substr, i));
        }
        sort(suffixes.begin(), suffixes.end());
        positions.insert(suffixes.front().second);
    }
    return std::vector<int>(positions.begin(), positions.end());
}

std::vector<int> findPowersPositions(const std::string& str) {
    std::vector<int> result;
    int length = 1;
    int l = str.length() - 1, r = str.length();
    while (true) {
        int pos = l;
        for (int i = l; i < r; ++i) {
            if (str.substr(i) < str.substr(pos)) {
                pos = i;
            }
        }
        if (result.empty() || 
            str.substr(pos, str.length() - result.back()) == str.substr(result.back()))
        {
            result.push_back(pos);
        }
        if (str.substr(pos) < str.substr(result.back())) {
            result.push_back(pos);
            //result = std::vector<int>(1, pos);
        }
        if (l == 0) {
            break;
        }
        length *= 2;
        r = l;
        l -= length;
        if (l < 0) l = 0;
    }

    std::sort(result.begin(), result.end());
    return result;
}

int main() {
    int seed;
    {
        std::ifstream fin("seed");
        fin >> seed;
    }
    srand(seed++);
    {
        std::ofstream fout("seed");
        fout << seed;
    }
    //std::string str = "aabab";
    const int C = 2;
    const int L = 10;
    std::string str = getRandomStr(L, C);
    
    std::vector<std::string> subStrings;
    for (char c = 'a'; c < 'a' + C + 1; ++c) {
        subStrings.push_back(std::string(1, c));
    }
    for (int i = 0; i < str.length(); ++i) {
        for (int j = 1; j <= str.length(); ++j) {
            subStrings.push_back(str.substr(i, j));
        }
    }

    for (int i = 0; i < str.length(); ++i) {
        std::string prefix = str.substr(0, i + 1);
        std::cout << prefix << std::endl;
        {
            std::vector<int> positions = findGoodPositions(prefix, subStrings);
            for (int j = 0; j < positions.size(); ++j) {
                std::cout << positions[j] << " ";
            }
            std::cout << std::endl;
        }
        {
            std::vector<int> positions = findPowersPositions(prefix);
            for (int j = 0; j < positions.size(); ++j) {
                std::cout << positions[j] << " ";
            }
            std::cout << std::endl;
        }
        //std::vector<int> positions(1, 0);
        //std::cout << "0 ";
        //int opt = 0;
        //for (int j = 1; j <= i; ++j) {
        //    std::string optS = str.substr(i - opt, opt + 1);
        //    std::string s = str.substr(i - j, j + 1);
        //    if (s < optS) {
        //        opt = j;
        //        positions.push_back(opt);
        //        std::cout << opt << " ";
        //    }
        //    else if (str.substr(i - j, opt + 1) == optS) {
        //        std::cout << "\033[1;32m" << j << "\033[0m ";
        //    }
        //}
        //std::cout << "\n";
    }

    return 0;
}
