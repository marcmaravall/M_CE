#include "uci.h"
#include "engine.h"
#include "search.h"

// bool UCI::IsSearching = false;

UCI::UCI()
{
	engine.init();
	// std::cout << "HI: " << Evaluation::GetGamePhase(engine.currentBoard) << "\n";
}

void UCI::uci()
{
	init_options();

	std::cout << "id name " << m_uciName << "\n";
	std::cout << "id author " << m_uciAuthor << "\n \n";

	for (const auto& option : options) {
		std::cout << option.second->to_uci_string() << "\n";
	}

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

		if ((tokens.size() > index) && tokens[index] == "moves") {
			index++;
			while (index < tokens.size()) {
				std::string moveStr = tokens[index];
				engine.MovePiece(moveStr.c_str());
				index++;
			}
		}
	}

	else if (command == "eval")
	{
		index++;

		if (index < tokens.size()) {
			if (tokens[index] == "simple")
				Evaluation::PrintSimpleEvaluation(engine.currentBoard);
		}
		else
			Evaluation::PrintEvaluation(engine.currentBoard);
	}

	else if (command == "getphase") {
		std::cout << "+----------+----------+" << "\n";
		
		std::cout << "Game Phase: ";
		switch (Evaluation::GetGamePhase(engine.currentBoard))
		{
		case OPENING:
			std::cout << "opening";
			break;

		case MIDGAME:
			std::cout << "midgame";
			break;
		case ENDGAME:
			std::cout << "endgame";
			break;
		default:
			break;
		}
		std::cout << "\n";
		std::cout << "Value phase: " << Evaluation::GetGamePhaseValue(engine.currentBoard) << "\n";
	
		std::cout << "+----------+----------+" << "\n";
	}

	else if (command == "setoption")
	{
		index++;
		if (tokens[index] != "name") {
			std::cerr << "ERROR: name not setted.\n";
			return;
		}
		index++;

		std::string name = "";
		std::string value = "";

		while (tokens[index] != "value") {
			name += tokens[index];
			index++;
		}
		index++;

		while (index < tokens.size())
		{
			value += tokens[index];
			index++;
		}

		setoption(name, value);
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
		else if (tokens[index] == "nodes") {
			index++;
			int nodes = std::stoi(tokens[index]);

			NODES = 0; 
			const auto start = std::chrono::high_resolution_clock::now();

			const MoveEval moveEval = engine.SearchNodes(nodes);

			const auto end = std::chrono::high_resolution_clock::now();
			const double elapsedSeconds = std::chrono::duration<double>(end - start).count();

			std::cout << "info nodes " << NODES << " nps " << static_cast<int>(NODES / elapsedSeconds) << " info score cp " << moveEval.eval << "\n";

			std::cout << "bestmove " << Utils::MoveToStr(moveEval.move) << "\n";
		}
		else if (tokens[index] == "movetime") {
			const int time = std::stoi(tokens[index + 1]);
		
			MoveEval moveEval = engine.SearchTime(time);
			std::cout << "info score cp " << moveEval.eval << "\n";
			std::cout << "bestmove " << Utils::MoveToStr(moveEval.move) << "\n";
		}
		else if (tokens[index] == "searchmoves") {
			index++;
			std::vector<Move> moves;
			while (index < tokens.size()) {
				index++;
			}
		}
		else if (tokens[index] == "infinite") {
			engine.SearchInfinite();
		}

		// depth search
		else if (tokens[index] == "depth") {
			index++;
			int depth = std::stoi(tokens[index]);
			NODES = 0; 
			auto start = std::chrono::high_resolution_clock::now();

			MoveEval moveEval = engine.Search(depth);

			auto end = std::chrono::high_resolution_clock::now();
			double elapsedSeconds = std::chrono::duration<double>(end - start).count();

			std::cout << "info nodes " << NODES <<
				" nps " << static_cast<int>(NODES / elapsedSeconds) << " score ";
			
			if (moveEval.eval > MATE_SCORE - 500) {
				std::cout << "mate " << (MATE_SCORE - (moveEval.eval+1))/2;
			}
			else if (moveEval.eval < -MATE_SCORE + 500) {
				std::cout << "mate " << abs((MATE_SCORE + (moveEval.eval-1))/2);
			}
			else {
				std::cout << "cp " << moveEval.eval;
			}
			std::cout << "\n";

			char promotion = (moveEval.move.promotion < 12) ? tolower(PIECE_CHAR[moveEval.move.promotion]) : ' ';
			std::cout << "bestmove " << Utils::MoveToStr(moveEval.move) << "\n";
		}
		else if (tokens[index] == "wtime")
		{
			index++;
			uint64_t wTime = std::stoi(tokens[index]);
			uint64_t bTime = 0;
			uint64_t wInc = 0;
			uint64_t bInc = 0;

			if (tokens[index] == "btime")
			{
				index++;
				bTime = std::stoi(tokens[index]);
			}

			if (index < tokens.size() && tokens[index] == "winc")
			{
				index++;
				wInc = std::stoi(tokens[index]);
			}
			if (index < tokens.size() && tokens[index] == "btime")
			{
				index++;
				bInc = std::stoi(tokens[index]);
			}

			const bool engineTurn = engine.currentBoard.turn;
			const float engineTime = (engineTurn == WHITE_TURN ? wTime : bTime);

			MoveEval moveEval = engine.SearchTime(engine.manager.CalculateTimeBasic(wTime, bTime, wInc, bInc, engineTurn));
			std::cout << "bestmove " << Utils::MoveToStr(moveEval.move) << "\n";
			std::cout << "info nodes" << NODES  << "score cp " << moveEval.eval << "\n";
		}
		else if (tokens[index] == "ponder") {
			// TODO: ponder search
		}
		else if (tokens[index] == "wtime" || tokens[index] == "btime" || tokens[index] == "winc" || tokens[index] == "binc") {
			index += 4; 
		}
	}

	else if (command == "setoption") {
		index++;
		if (tokens[index] == "name") {
			index++;
			const std::string name = tokens[index];
			index++;
			if (tokens[index] == "value") {
				index++;
				std::string value = tokens[index];
				setoption(name.c_str(), value.c_str());
			}
		}
	}

	else
	{
		std::cout << "ERROR: command not found: " << input << "\n \n";
	}
}

void UCI::setoption(const std::string& name, const std::string& value)
{
	std::cout << "Option setted: name: " << name << " value: " << value << "\n";

	options[name]->set_value(value);

	UpdateOptions();
}

void UCI::UpdateOptions()
{
	engine.useBooks = options["UseBooks"]->get_value() == "true";
	engine.ponder = options["Ponder"]->get_value() == "true";
}

void UCI::init_options()  
{  
    auto hash = std::make_unique<SpinOption>();  
    hash->name = "Hash";  
    hash->min = 1;  
    hash->max = 1024;  
    hash->value = 16;  
    options["Hash"] = std::move(hash);

    auto ponder = std::make_unique<CheckOption>();  
    ponder->name = "Ponder";
    ponder->value = true;  
    options["Ponder"] = std::move(ponder);  

    auto clear = std::make_unique<ButtonOption>();  
    clear->name = "Clear Hash";  
    clear->callback = []() { };  
    options["Clear Hash"] = std::move(clear);  

    auto useBooks = std::make_unique<CheckOption>();  
    useBooks->name = "UseBooks";
    useBooks->value = false;  
    options["UseBooks"] = std::move(useBooks);
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
	
	std::vector<Move> moves;
	GenerateLegalMoves(engine.currentBoard, moves);
	// Divide(engine.currentBoard, 5);
	auto end = high_resolution_clock::now();

	double duration = std::chrono::duration<double, std::micro>(end - start).count();
	std::cout << "Time generating legal moves: " << duration << "us. " << duration / 1000.0f << "ms. " << duration / 1'000'000.0 << "s. \n";
}

void UCI::EvalTest()
{
	using namespace std::chrono;

	std::vector<Move> moves;
	GenerateLegalMoves(engine.currentBoard, moves);

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

	double duration = std::chrono::duration<double, std::micro>(end - start).count();
	std::cout << "Time generating evaluations of " << moves.size() << " positions: " << duration << "us. " << duration / 1'000.0f << "ms. " << duration / 1'000'000.0 << "s. \n";
}

void UCI::UndoMoveTest()
{
	std::vector<Move> moves;
	GenerateLegalMoves(engine.currentBoard, moves);

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

	NODES = 0;
	const MoveEval m = engine.Search(2);

	auto end = std::chrono::high_resolution_clock::now();

	double duration = std::chrono::duration<double, std::micro>(end - start).count();
	std::cout << "Time with alphabeta nodes " << NODES << " duration " << duration << "us. " << duration / 1'000.0f << "ms. " << duration / 1'000'000.0 << "s. \n";
}

void UCI::SearchTest()
{
	
}

void UCI::InsertCommand(const char* command)
{
	ManageInput(command);
}

std::string UCI::GetOsCompiled() const
{
#ifdef __linux__
	return "linux";
#elif __APPLE__
	return "macos";
#elif _WIN32
	return "windows";
#endif 
}

std::string UCI::GetCompiler() const
{
#ifdef __clang__
	return "clang";
#elif __GNUC__
	return "gcc";
#elif _MSC_VER
	return "msvc";
#else
	return "unknown";
#endif
}

void UCI::InterpolateTest()
{
	std::cout << "\n" << "+----------+----------+\n";
	std::cout << "  Interpolation Test. \n";
	std::cout << "+----------+----------+\n";
	
	const int a = 0;
	const int b = 4;
	const double interpolate = 0.5;
	const double extrapolate = false;
		
	std::cout << "\nA=" << a << " B=" << b << " Interpolation=" << interpolate << "\n";
	std::cout << "Result: " << Utils::InterpolateInt(a, b, interpolate, extrapolate) << "\n" << "\n";

	const int* out = Utils::Interpolate(W_PAWN_BITMAP_OPENING, W_PAWN_BITMAP_MIDGAME, 0.5);

	for (size_t i = 0; i < 64; i++) {
		std::cout << out[i] << " ";
		if (i % 8 == 0) std::cout << "\n";
	}
}

