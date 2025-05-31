#include "uci.h"
#include "engine.h"
#include "search.h"

UCI::UCI()
{
	engine.init();
}

void UCI::uci()
{
	std::cout << "id name " << m_uciName << "\n";
	std::cout << "id author " << m_uciAuthor << "\n";
	std::cout << "uciok\n";
}

void UCI::isready()
{
	std::cout << "readyok\n";
}

void UCI::quit()
{
	std::cout << "bye\n";
	exit(0);
}

void UCI::ucinewgame()
{
	engine.StartPos();
}

void UCI::position(const char* fen)
{
	engine.SetPosition(fen);
}

void UCI::perft(int depth)
{
	engine.DivideTest(depth);
}

std::vector<std::string> SplitBySpaces(const char* input) {
	std::vector<std::string> result;
	std::istringstream iss(input);
	std::string token;

	while (iss >> token) {
		result.push_back(token);
	}

	return result;
}


// MANAGE INPUT
void UCI::ManageInput(const char* input)
{
	std::vector<std::string> tokens = SplitBySpaces(input);

	if (tokens.empty()) {
		return;
	}

	uint8_t index = 0;

	std::string command = tokens[index];
	std::unordered_map<std::string, std::function<void()>> command_map = {
		{"uci", [this]() { uci(); }},
		{"isready", [this]() { isready(); }},
		{"setoption", [this, &tokens, &index]() {
			index++;
			std::string name = tokens[index];
			index++;
			std::string value = tokens[index];
			setoption(name.c_str(), value.c_str());
		}},
		{"register",			[this]() { register_uci(); }},
		{"d",					[this]() { draw(); }},
		{"draw",				[this]() { draw(); }},
		{"ucinewgame",			[this]() { ucinewgame(); }},
		{"stop",				[this]() { stop(); }},
		{"ponderhit",			[this]() { ponderhit(); }},
		{"quit",				[this]() { quit(); }},
	};

	auto it = command_map.find(command);
	if (it != command_map.end()) {
		it->second();
	}
	else if (command == "position") {
		index++;
		if (tokens[index] == "startpos") {
			engine.StartPos();
			index++;
		}
		else if (tokens[index] == "fen") {
			index++;
			std::string s = "";

			while (index < tokens.size() && tokens[index] != "moves") {
				s += tokens[index] + " ";
				index++;
			}

			engine.SetPosition(s.c_str());
		}

		if (tokens[index] == "moves") {
			index++;
			// std::cerr << "Moves: ";
			while (index < tokens.size()) {
				std::string moveStr = tokens[index];
				engine.MovePiece(moveStr.c_str());
				index++;
			}
		}
	}

	else if (command == "go") {
		index++;

		if (tokens[index] == "perft")
		{
			index++;
			int depth = std::stoi(tokens[index]);
			perft(depth);
		}
		else if (tokens[index] == "searchmoves") {
			index++;
			std::vector<Move> moves;
			while (index < tokens.size()) {
				index++;
			}
		}
		else if (tokens[index] == "infinite") {
			index++;
			MoveEval moveEval = engine.Search(5);

			char promotion = (moveEval.move.promotion != 255) ? tolower(PIECE_CHAR[moveEval.move.promotion]) : ' ';
			std::cout << "bestmove " << Utils::ConvertToBoardPosition(moveEval.move.from)
				<< Utils::ConvertToBoardPosition(moveEval.move.to)
				<< promotion << "\n";
		}
		else if (tokens[index] == "depth") {
			index++;
			int depth = std::stoi(tokens[index]);
			MoveEval moveEval = engine.Search(depth);

			char promotion = (moveEval.move.promotion != 255) ? tolower(PIECE_CHAR[moveEval.move.promotion]) : ' ';
			std::cout << "bestmove " << Utils::ConvertToBoardPosition(moveEval.move.from) 
					  << Utils::ConvertToBoardPosition(moveEval.move.to) 
					  << promotion << "\n";
		}
		else if (tokens[index] == "movetime")
		{
			index++;
			int time = std::stoi(tokens[index]);
			MoveEval moveEval = engine.Search(static_cast<float>(time) / 1000.0f);
			std::cout << "bestmove " << Utils::ConvertToBoardPosition(moveEval.move.from) << Utils::ConvertToBoardPosition(moveEval.move.to) << "\n";
		}
		else if (tokens[index] == "ponder") {
			// Handle ponder search
		}
		else if (tokens[index] == "wtime" || tokens[index] == "btime" || tokens[index] == "winc" || tokens[index] == "binc") {
			index += 4; // Skip time and increment values
		}
	}
}

void UCI::setoption(const char* name, const char* value)
{
	std::cout << "Setting option: " << name << " to " << value << "\n";
}

void UCI::register_uci()
{
	std::cout << "UCI registered\n";
}

void UCI::ponderhit()
{
	std::cout << "Ponder hit\n";
}

void UCI::stop()
{
	std::cout << "Stop command received\n";
}

void UCI::draw()
{
	engine.PrintBoard();
}

