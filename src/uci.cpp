#include "uci.h"
#include "engine.h"
#include "search.h"

// bool UCI::IsSearching = false;

UCI::UCI()
{
	// engine.init();
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

	else if (command == "eval")
	{
		std::cout << "info string eval " << Evaluation::Evaluate(engine.currentBoard) << "\n";
	}

	else if (command == "tests")
	{
		SpeedTest();
		UndoMoveTest();
		EvalTest();
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

			sendInfo = true;
			// std::future<void> infoFuture = std::async(std::launch::async, &UCI::SendInfo, this);

			std::cout << "info string search started\n";

			MoveEval moveEval = engine.Search(depth);

			std::cout << "info string search ended\n";

			sendInfo = false;
			// infoFuture.get(); 

			// std::cerr << moveEval.move.from << " " << moveEval.move.to << "\n";

			char promotion = (moveEval.move.promotion < 12) ? tolower(PIECE_CHAR[moveEval.move.promotion]) : ' ';
			std::cout << "bestmove " << Utils::MoveToStr(moveEval.move) << "\n";
						/* "bestmove " << Utils::ConvertToBoardPosition(moveEval.move.from) 
					  << Utils::ConvertToBoardPosition(moveEval.move.to) 
					  << promotion << "\n";*/
		}
		else if (tokens[index] == "movetime")	// TODO: Implement move time
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

void UCI::SendInfo()
{
	while (sendInfo)
	{
		int actualNodes = NODES;

		std::this_thread::sleep_for(std::chrono::seconds(1));

		int nps = (NODES - actualNodes);

		std::string info = "info";
		info += (" nodes " + std::to_string(NODES));
		info += (" nps " + std::to_string(nps));

		std::cout << info << std::endl;
	}
}

void UCI::SpeedTest()
{
	using namespace std::chrono;

	auto start = high_resolution_clock::now();
	
	std::vector<Move> moves = GenerateLegalMoves(engine.currentBoard);
	// Divide(engine.currentBoard, 5);
	auto end = high_resolution_clock::now();

	double duration = duration_cast<microseconds>(end - start).count();
	std::cout << "Time generating legal moves: " << duration << "us. " << duration / 1000.0f << "ms. " << duration / 1'000'000.0 << "s. \n";
}

void UCI::EvalTest()
{
	using namespace std::chrono;

	std::vector<Move> moves = GenerateLegalMoves(engine.currentBoard);

	std::vector<Board> boards;

	for (const Move& move : moves)
	{
		Board current = engine.currentBoard;
		current.MovePieceFast(move);

		boards.push_back(current);
	}

	auto start = high_resolution_clock::now();

	for (const Board& board : boards)
	{
		Evaluation::Evaluate(board);
	}

	auto end = high_resolution_clock::now();

	double duration = duration_cast<microseconds>(end - start).count();
	std::cout << "Time generating evaluations of " << moves.size() << " positions: " << duration << "ps. " << duration / 1'000.0f << "ms. " << duration / 1'000'000.0 << "s. \n";
}

void UCI::UndoMoveTest()
{
	const std::vector moves = GenerateLegalMoves(engine.currentBoard);

	auto start = std::chrono::high_resolution_clock::now();
	for (const Move& m : moves) {
		UndoInfo info = Utils::CreateUndoInfo(engine.currentBoard, m);
		engine.currentBoard.MovePiece(m);
		engine.currentBoard.UndoMove(info);
	}
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Move + undo of 20 moves: "
		<< std::chrono::duration<double, std::micro>(end - start).count() << "us\n";
}

void UCI::NPSTest()
{
	auto start = std::chrono::high_resolution_clock::now();

	const MoveEval m = AlphaBeta(engine.currentBoard, 5, -1000000, 1000000, true, 5);

	auto end = std::chrono::high_resolution_clock::now();

	double duration = duration_cast<std::chrono::microseconds>(end - start).count();
	std::cout << "Time with alphabeta" << duration << "us. " << duration / 1'000.0f << "ms. " << duration / 1'000'000.0 << "s. \n";
}

void UCI::SearchTest()
{
	
}
