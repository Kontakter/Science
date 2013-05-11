#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::string getRandomStr(int len, int range) {
    std::string res;
    for (int i = 0; i < len; ++i) {
        res = res + static_cast<char>(('a' + (rand() % range)));
    }
    return res;
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
    std::string str = getRandomStr(10, 3);
    for (int i = 0; i < str.length(); ++i) {
        std::cout << str.substr(0, i + 1) << std::endl;
        std::vector<int> positions(1, 0);
        std::cout << "0 ";
        int opt = 0;
        for (int j = 1; j <= i; ++j) {
            std::string optS = str.substr(i - opt, opt + 1);
            std::string s = str.substr(i - j, j + 1);
            if (s < optS) {
                opt = j;
                positions.push_back(opt);
                std::cout << opt << " ";
            }
            else if (str.substr(i - j, opt + 1) == optS) {
                std::cout << "\033[1;32m" << j << "\033[0m ";
            }
        }
        std::cout << "\n";
        //for (int j = 0; j < positions.size(); ++j) {
        //    std::cout << positions[j];
        //    if (j + 1 < positions.size()) {
        //        std::cout << " ";
        //    }
        //    else {
        //        std::cout << "\n";
        //    }
        //}
    }

    return 0;
}
