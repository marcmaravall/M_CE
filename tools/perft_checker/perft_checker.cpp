#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>

inline std::vector<std::string> splitString_find_substr(const std::string& str) {
    std::vector<std::string> tokens;
    size_t start = 0, end;
    while ((end = str.find(' ', start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

void Check(const std::string& m_ce, const std::string& stockfish)
{
    std::istringstream m_ce_stream(m_ce);
    std::istringstream stockfish_stream(stockfish);

    std::map<std::string, int> m_ce_moves;
    std::map<std::string, int> stockfish_moves;

    std::string line;
    int m_ce_total = 0, stockfish_total = 0;

    // mce
    while (std::getline(m_ce_stream, line)) {
        if (line.find("Total nodes:") != std::string::npos) {
            std::istringstream total_stream(line);
            std::string temp;
            total_stream >> temp >> temp >> m_ce_total;
        }
        else if (line.find(':') != std::string::npos) {
            auto parts = splitString_find_substr(line);
            if (parts.size() >= 3) {
                std::string move = parts[0] + " " + parts[1];
                int nodes = std::stoi(parts[2]);
                m_ce_moves[move] = nodes;
            }
        }
    }

    // stockfish
    while (std::getline(stockfish_stream, line)) {
        if (line.find("Total nodes:") != std::string::npos) {
            std::istringstream total_stream(line);
            std::string temp;
            total_stream >> temp >> temp >> stockfish_total;
        }
        else if (line.find(':') != std::string::npos) {
            auto parts = splitString_find_substr(line);
            if (parts.size() >= 3) {
                std::string move = parts[0] + " " + parts[1];
                int nodes = std::stoi(parts[2]);
                stockfish_moves[move] = nodes;
            }
        }
    }

    int correct = 0;
    int incorrect = 0;
    int missing_in_stockfish = 0;

    std::cout << "\033[33m === COMPARISON ===\n";
    for (const auto& [move, m_ce_nodes] : m_ce_moves) {
        auto it = stockfish_moves.find(move);
        if (it != stockfish_moves.end()) {
            int sf_nodes = it->second;
            if (sf_nodes == m_ce_nodes) {
                std::cout << "  \033[32mCorrect    : \033[0m" << move << ": " << m_ce_nodes << "\n";
                correct++;
            }
            else {
                std::cout << "  \033[31mIncorrect  : \033[0m" << move << ": " << m_ce_nodes
                    << " (expected: " << sf_nodes << ")\n";
                incorrect++;
            }
        }
        else {
            std::cout << "  Missed in stockfish: \033[0m" << move << ": " << m_ce_nodes << "\n";
            missing_in_stockfish++;
        }
    }


    int missing_in_m_ce = 0;
    for (const auto& [move, sf_nodes] : stockfish_moves) {
        if (m_ce_moves.find(move) == m_ce_moves.end()) {
            std::cout << "  Missed in M_CE     : " << move << ": " << sf_nodes << "\n";
            missing_in_m_ce++;
        }
    }

    std::cout << "\n\033[33m=== SUMMARY ===\n";
    std::cout << "\033[33m Total moves in M_CE      : \033[0m" << m_ce_moves.size() << "\n";
    std::cout << "\033[33m Total moves in stockfish : \033[0m" << stockfish_moves.size() << "\n";
    std::cout << "\033[33m Correct moves            : \033[0m" << correct << "\n";
    std::cout << "\033[33m Incorrect moves          : \033[0m" << incorrect << "\n";
    std::cout << "\033[33m Missing in stockfish     : \033[0m" << missing_in_stockfish << "\n";
    std::cout << "\033[33m Missing in M_CE          : \033[0m" << missing_in_m_ce << "\n";
    std::cout << "\033[33m Total nodes (M_CE)       : \033[0m" << m_ce_total << "\n";
    std::cout << "\033[33m Total nodes (Stockfish)  : \033[0m" << stockfish_total << "\n";
}

int main()
{
    // line: d2d3 d7d5 e1d2 d5d4
    const std::string m_ce = R"(
d5 d4: 1
a7 a6: 1
b7 b6: 1
b7 b5: 1
f7 f6: 1
f7 f5: 1
g7 g6: 1
g7 g5: 1
h7 h6: 1
h7 h5: 1
c6 b4: 1
c6 d4: 1
c6 e5: 1
c6 e7: 1
c6 b8: 1
c6 d8: 1
g8 f6: 1
g8 h6: 1
g8 e7: 1
f8 a3: 1
f8 b4: 1
f8 c5: 1
f8 d6: 1
f8 e7: 1
a8 b8: 1
a8 c8: 1
a8 d8: 1
a5 c3: 1
a5 b4: 1
a5 b6: 1
a5 c7: 1
a5 d8: 1
a5 a2: 1
a5 a3: 1
a5 a4: 1
a5 b5: 1
a5 c5: 1
a5 a6: 1
e8 d7: 1
e8 e7: 1
e8 d8: 1
)";

    const std::string stockfish = R"(
d5 d4: 1
a7 a6: 1
b7 b6: 1
f7 f6: 1
g7 g6: 1
h7 h6: 1
b7 b5: 1
f7 f5: 1
g7 g5: 1
h7 h5: 1
c6 b4: 1
c6 d4: 1
c6 e5: 1
c6 e7: 1
c6 b8: 1
c6 d8: 1
g8 f6: 1
g8 h6: 1
g8 e7: 1
f8 a3: 1
f8 b4: 1
f8 c5: 1
f8 d6: 1
f8 e7: 1
a8 b8: 1
a8 c8: 1
a8 d8: 1
a5 a2: 1
a5 a3: 1
a5 c3: 1
a5 a4: 1
a5 b4: 1
a5 b5: 1
a5 c5: 1
a5 a6: 1
a5 b6: 1
a5 c7: 1
a5 d8: 1
e8 d7: 1
e8 e7: 1
e8 d8: 1
e8 c8: 1
)";

    Check(m_ce, stockfish);
	return 0;
}
