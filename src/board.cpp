#include "board.h"
#include "utils.h"
#include "engine.h"

Board::Board(const char* fen)
{
	ClearCurrentBitboard();

	std::string fenString(fen);
	size_t index = 0;

	// Parse board layout
	for (int rank = 7; rank >= 0; rank--) {
		int file = 0;

		while (file < 8 && index < fenString.size()) {
			char c = fenString[index++];

			if (c == '/') {
				
			}
			else if (isdigit(c)) {
				file += (c - '0');
			}
			else {
				bool pieceFound = false;
				for (size_t pieceIndex = 0; pieceIndex < 12; pieceIndex++) {
					if (c == PIECE_CHAR[pieceIndex]) {
						bitboards[pieceIndex] |= (1ULL << (rank * 8 + file));
						pieceFound = true;
						break;
					}
				}

				if (pieceFound) {
					file++;
				}
				else {
					std::cerr << "Not found character: " << c << std::endl;
					file++;
				}
			}
		}
	}

	while (index < fenString.size() && fenString[index] == ' ') {
		index++;
	}

	if (index < fenString.size()) {
		turn = (fenString[index] == 'w');
		index++;
	}

	while (index < fenString.size() && fenString[index] == ' ') {
		index++;
	}

	while (index < fenString.size() &&
		(fenString[index] == 'K' || fenString[index] == 'Q' ||
			fenString[index] == 'k' || fenString[index] == 'q')) {
		switch (fenString[index]) {
		case 'K': wCastlingKing = true; break;
		case 'Q': wCastlingQueen = true; break;
		case 'k': bCastlingKing = true; break;
		case 'q': bCastlingQueen = true; break;
		}
		index++;
	}

	while (index < fenString.size() && fenString[index] == ' ') {
		index++;
	}

	enPassantSquare = -1;
	if (index < fenString.size()) {
		if (fenString[index] == '-') {
			index++;
		}
		else if (index + 1 < fenString.size()) {
			char fileChar = fenString[index];
			char rankChar = fenString[index + 1];
			if (fileChar >= 'a' && fileChar <= 'h' && rankChar >= '1' && rankChar <= '8') {
				int file = fileChar - 'a';
				int rank = 8 - (rankChar - '0');
				enPassantSquare = rank * 8 + file;
			}
			index += 2;
		}
	}

	while (index < fenString.size() && fenString[index] == ' ') {
		index++;
	}

	ClearHalfMoves();
	if (index < fenString.size()) {
		std::string halfMoveStr = "";
		while (index < fenString.size() && isdigit(fenString[index])) {
			halfMoveStr += fenString[index];
			index++;
		}
		if (!halfMoveStr.empty()) {
			halfMoves = std::stoi(halfMoveStr);
		}
	}

	while (index < fenString.size() && fenString[index] == ' ') {
		index++;
	}

	turns = 1;

	if (index < fenString.size()) {
		std::string turnStr = "";
		while (index < fenString.size() && isdigit(fenString[index])) {
			turnStr += fenString[index];
			index++;
		}
		if (!turnStr.empty()) {
			turns = std::stoi(turnStr);
		}
	}
}

void Board::ClearCurrentBitboard()
{
	for (int i = 0; i < 12; i++) {
		bitboards[i] = 0ULL;
	}
}


Board::Board()
{
}

Board::~Board()
{
}

/*std::string Board::ConvertToBoardPosition(uint8_t squareIndex) {
	return squareIndex < 64 ? (std::string)BOARD_STRINGS[squareIndex] : "null";
}*/

// TODO: refatorize for struct Move				
bool Board::MovePiece(const Move& move)
{
	// const Board buffer = *this;
	UndoInfo undo = Utils::CreateUndoInfo(*this, move);

	const uint8_t from = move.from;
	const uint8_t to = move.to;

	const bool startTurn = turn;
	const ZobristHash startHash = Utils::GetZobristHash(*this, Engine::hashSettings);

	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
			break;
		}
	}

	if (pieceType >= 12) {
		// std::cerr << "Piece not found, piece: " << pieceType << "\n";
		// Utils::DebugMove(move);
		// Utils::PrintBitboards(*this);
		return false;
	}

	Bitboard bitboard = bitboards[pieceType];

	if (turn == WHITE_TURN && pieceType >= 6) {
		// std::cerr << "ERROR: trying to move a black piece.\n";
		goto ret_false;
	}
	else if (turn == !WHITE_TURN && pieceType < 6) {
		// std::cerr << "ERROR: trying to move a white piece.\n";
		goto ret_false;
	}

	if (move.promotion < 12) {
		Promotion(move);

		// std::cerr << "PROMOTION DO\n";
	}
	else
	{
		// std::cerr << "DEBUG: " << Utils::MoveToStr(move) << " " << (int)(move.promotion) << "\n";
	}

	if (move.castling) {
		if (CanCastle(move))
		{
			Castle(move);
		}
		else
		{
			// std::cerr << "HI\n";
		}
	}

	switch (pieceType)
	{
	case 0:
		if (CanMovePawn(move))
		{
			MovePawn(move);
			halfMoves--;		
		}
		break;
	case 1:
		if (CanMoveKnight(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 2:
		if (CanMoveBishop(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 3:
		if (CanMoveRook(move))
		{
			if (from == 7)
				wCastlingQueen = false;
			if (from == 0)
				wCastlingQueen = false;

			MoveWithoutComprobe(from, to);

			enPassantSquare = 255;
		}
		break;
	case 4:
		if (CanMoveQueen(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 5:
		if (CanMoveKing(move))
		{
			MoveWithoutComprobe(from, to);
			wCastlingKing = false;
			wCastlingQueen = false;
			enPassantSquare = 255;

		}
		break;
	case 6:

		if (CanMovePawn(move))
		{
			MovePawn(move);
			halfMoves--;
		}
		break;
	case 7:
		if (CanMoveKnight(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 8:
		if (CanMoveBishop(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;

		}
		break;
	case 9:
		if (CanMoveRook(move))
		{
			if (from == 63-7)
				bCastlingQueen = false;
			if (from == 63)
				bCastlingQueen = false;

			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 10:
		if (CanMoveQueen(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
		break;
	case 11:
		if (CanMoveKing(move))
		{
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;

			bCastlingKing = false;
			bCastlingQueen = false;
		}

		break;
	}

	if (IsCheck((startTurn == WHITE_TURN) ? WHITE : BLACK, false)) {
		// std::cerr << "ERROR: move puts king in check.\n";
		// std::cerr << ((turn == WHITE_TURN) ? "WHITE":"BLACK") << "\n";
		UndoMove(undo);
		// *this = buffer;
		goto ret_false;
	}

	if (startTurn == BLACK_TURN) {
		turns++;
	}

	halfMoves++;

	goto ret_true;

ret_true:
	repetitionsHistory.push_back(startHash);
	return true;

ret_false:
	return false;
}

bool Board::MovePieceFast(const Move& move)		// for search
{
	const UndoInfo undo = Utils::CreateUndoInfo(*this, move);

	const uint8_t from = move.from;
	const uint8_t to = move.to;

	int pieceType = 255;
	for (int i = 0; i < 12; ++i) {
		if ((bitboards[i] >> from) & 1ULL) {
			pieceType = i;
			break;
		}
	}
	if (pieceType >= 12) return false;

	bool pawnMove = false;

	if (pieceType == W_PAWN_I || pieceType == B_PAWN_I) {
		pawnMove = true;
	}

	for (int i = 0; i < 12; ++i)
		bitboards[i] &= ~(1ULL << to);

	bitboards[pieceType] &= ~(1ULL << from);
	bitboards[pieceType] |= (1ULL << to);

	if (move.promotion < 12) {
		bitboards[pieceType] &= ~(1ULL << to);
		bitboards[move.promotion] |= (1ULL << to);
	}

	if (IsCheck(turn == !WHITE_TURN ? BLACK : WHITE, false)) {
		// std::cout << "\n \nIS CHECK DETECTED FOR DEBUG: \n";
		
		UndoMove(undo);

		return false;
	}

	enPassantSquare = 255;
	turn = !turn;
	if (pawnMove)
		ClearHalfMoves();
	else
		++halfMoves;
	if (turn == WHITE_TURN)
		++turns;

	return true;
}

bool Board::Promotion(const Move move)
{
	const int from = move.from;
	const int position = move.to;
	const int promotion = move.promotion;
	// std::cerr << "PROMOTION ----" << from << " " << position << " " << promotion << "\n";

	int pieceType = 255;
	for (int i = 0; i < 12; i++) {
		if (Utils::GetBitboardValueOnIndex(bitboards[i], from)) {
			pieceType = i;
		}
	}

	// std::cout << promotion;

	if (promotion == 0 || promotion == 5 || promotion == 6 || promotion == 11) {
		std::cerr << "ERROR: cannot promove to pawn or king.\n";
		return false;
	}

	if (turn == WHITE_TURN && pieceType != W_PAWN_I) {
		std::cerr << "ERROR: only pawns can promote." << pieceType << "\n";
		return false;
	}
	if (turn == !WHITE_TURN && pieceType != B_PAWN_I) {
		std::cerr << "ERROR: only pawns can promote.\n";
		return false;
	}


	if (turn == WHITE_TURN && position < 56) {
		std::cerr << "ERROR: cannot promote a piece that is not in the last rank.\n";
		return false;
	}
	else if (turn == !WHITE_TURN && position > 7) {
		std::cerr << "ERROR: cannot promote a piece that is not in the last rank.\n";
		return false;
	}

	if (!CanMovePawn(move)) {
		std::cerr << "ERROR: promotion error.\n";
		return false;
	}

	MoveWithoutComprobe(from, position);
	ClearBitInAllBitboards(position);

	SetBitboardBit(promotion, position);

	// std::cerr << "Board representation: \n";
	return true;
}

bool Board::CanMovePawn(const Move move, const bool _turn) const {
	const Bitboard allPieces = Utils::GetAllBitboards(bitboards, BOTH);
	const uint64_t occupied = allPieces;
	const uint64_t empty = ~occupied;

	const uint8_t from = move.from;
	const uint8_t to = move.to;

	const uint64_t fromBB = 1ULL << from;
	const uint64_t toBB = 1ULL << to;

	if (_turn == WHITE_TURN) {

		const uint64_t oneStep = fromBB << NORTH;
		const uint64_t twoSteps = fromBB << NORTH*2;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) << 7) |
			((fromBB & ~FILE_A_MASK) << 9);

		if ((to == (from + NORTH)) && !Utils::GetBitboardValueOnIndex(allPieces, to)) {

			return true;
		}
		else if ((toBB & twoSteps) && (fromBB & RANK_2_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			return true;
		}
		else if ((fromBB & ~FILE_H_MASK) && (to == from + NORTH_EAST) && Utils::IsBlackPieceAt(*this, to)) {
			return true;
		}
		else if ((fromBB & ~FILE_A_MASK) && (to == from + NORTH_WEST) && Utils::IsBlackPieceAt(*this, to)) {
			return true;
		}
		const bool isEnPassantCapture =
			to == enPassantSquare &&
			((from + NORTH_EAST == to && (fromBB & ~FILE_H_MASK)) || 
			(from + NORTH_WEST == to && (fromBB & ~FILE_A_MASK)));  

		if (isEnPassantCapture) {
			return true;
		}
	}
	else { // BLACK_TURN
		const uint64_t oneStep = fromBB >> 8;
		const uint64_t twoSteps = fromBB >> 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) >> 7) |
			((fromBB & ~FILE_A_MASK) >> 9);

		if ((to == (from + SOUTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) < 1) {
			return true;
		}
		else if ((toBB & twoSteps) && (fromBB & RANK_7_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			return true;
		}
		else if ((fromBB & ~FILE_H_MASK) && (to == from + SOUTH_EAST) && Utils::IsWhitePieceAt(*this, to)) {
			return true;
		}
		else if ((fromBB & ~FILE_A_MASK) && (to == from + SOUTH_WEST) && Utils::IsWhitePieceAt(*this, to)) {
			return true;
		}
		
		const bool isEnPassantCapture =
			to == enPassantSquare &&
			((from + SOUTH_EAST == to && (fromBB & ~FILE_H_MASK)) || 
			(from + SOUTH_WEST == to && (fromBB & ~FILE_A_MASK)));  
		if (isEnPassantCapture) {
			return true;
		}
	}

	return false;
}

bool Board::CanMovePawn(const Move move) const {
	return CanMovePawn(move, this->turn);
}

bool Board::CanMoveKnight(const Move move) const {
	return CanMoveKnight(move, this->turn);
}

bool Board::CanMoveBishop(const Move move) const {
	return CanMoveBishop(move, this->turn);
}

bool Board::CanMoveQueen(const Move move) const {
	return CanMoveQueen(move, this->turn);
}

bool Board::CanMoveRook(const Move move) const {
	return CanMoveRook(move, this->turn);
}

bool Board::CanMoveKing(const Move move) const {
	return CanMoveKing(move, this->turn);
}


void Board::MovePawn(const Move move) {
	if (!CanMovePawn(move))
		return;

	const Bitboard allPieces = Utils::GetAllBitboards(bitboards, BOTH);
	const uint64_t occupied = allPieces;
	const uint64_t empty = ~occupied;

	const uint8_t from = move.from;
	const uint8_t to = move.to;

	const uint64_t fromBB = 1ULL << from;
	const uint64_t toBB = 1ULL << to;

	ClearHalfMoves();

	if (turn == WHITE_TURN) {
		const uint64_t oneStep = fromBB << 8;
		const uint64_t twoSteps = fromBB << 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) << 7) |
			((fromBB & ~FILE_A_MASK) << 9);

		if ((to == (from + NORTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) != 1) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((toBB & twoSteps) && (fromBB & RANK_2_MASK) &&
			((oneStep & empty) && (toBB & empty))) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = from+NORTH;
		}

		else if (((to == from + NORTH_EAST || to == from + NORTH_WEST)) &&
			Utils::IsBlackPieceAt(*this, to)) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((to == enPassantSquare) &&
			((from + NORTH_EAST == to) || (from + NORTH_WEST == to))) {

			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
	}

	else { // BLACK_TURN
		const uint64_t oneStep = fromBB >> 8;
		const uint64_t twoSteps = fromBB >> 16;
		const uint64_t captures =
			((fromBB & ~FILE_H_MASK) >> 7) |
			((fromBB & ~FILE_A_MASK) >> 9);

		if ((to == (from + SOUTH)) && Utils::GetBitboardValueOnIndex(allPieces, to) != 1) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((toBB & twoSteps) && (fromBB & RANK_7_MASK) &&
			((oneStep & empty) && (toBB & empty))) {

			MoveWithoutComprobe(from, to);
			enPassantSquare = from + SOUTH;
		}

		else if (((to == from + SOUTH_EAST || to == from + SOUTH_WEST)) &&
			Utils::IsWhitePieceAt(*this, to)) {
			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}

		else if ((to == enPassantSquare) &&
			((from + SOUTH_EAST == to) || (from + SOUTH_WEST == to))) {

			MoveWithoutComprobe(from, to);
			enPassantSquare = 255;
		}
	}
}

void Board::UndoMove(const UndoInfo& undo, const bool debug) {
	// std::cout << "DEBUG UNDO MOVE" << "\n";
	// Utils::PrintBoard(*this);

	// const bool debug = false;

	// std::cout << "FEN START: " << Utils::ConvertToFEN(*this) << "\n";

	/*if (Utils::ConvertToFEN(*this) == "rnbPk1nr/ppppbppp/8/4p3/4P3/P7/1PPP1PPP/RNBQKBNR w KQkq - 0 1") {
		debug = true;
		std::cout << "DEBUGGING POSITION WITH ERROR -------------------------\n";
	}*/

	if (undo.move.castling)
	{
		// TODO: castling, now i'm so lazy 

		if (undo.move.mode)
		{
			MoveWithoutComprobe(undo.turn == WHITE_TURN? 6 : 62, undo.turn == WHITE_TURN ? 4 : 60);
			MoveWithoutComprobe(undo.turn == WHITE_TURN ? 5 : 61, undo.turn == WHITE_TURN ? 7 : 63);
		}
	}

	Move reverseMove; // (undo.move.to, undo.move.from);
	reverseMove.from = undo.move.to;
	reverseMove.to = undo.move.from;

	MoveWithoutComprobe(reverseMove.from, reverseMove.to);

	if (undo.capturedPiece < 12) {
		SetBitboardBit(undo.capturedPiece, undo.move.to);
	}

	if (undo.promotedPiece < 12) {
		ClearBitInAllBitboards(undo.move.to);
		SetBitboardBit((undo.turn == WHITE_TURN ? 0 : 6), undo.move.from);		// TODO: solve bug in this part.
	}

	wCastlingKing = undo.wCastlingKing;
	wCastlingQueen = undo.wCastlingQueen;
	bCastlingKing = undo.bCastlingKing;
	bCastlingQueen = undo.bCastlingQueen;
	enPassantSquare = undo.enPassantSquare;
	turn = undo.turn;
	turns = undo.turns;

	// std::cout << "FEN REVERSED MOVE:" << Utils::ConvertToFEN(*this) << "\n";
}

inline bool Board::CanMoveKnight(const Move move, const bool _turn) const{
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (!((Engine::knightMasks[from] >> to) & 1ULL))
		return false;

	if (_turn == WHITE_TURN)
		return !Utils::IsWhitePieceAt(*this, to);
	else
		return !Utils::IsBlackPieceAt(*this, to);
}

inline bool Board::CanMoveBishop(const Move move, const bool _turn) const{
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (from == to)
		return false;

	const Bitboard fromBB = 1ULL << from;
	const Bitboard toBB = 1ULL << to;

	if (_turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	const int fromFile = from % 8;
	const int fromRank = from / 8;
	const int toFile = to % 8;
	const int toRank = to / 8;

	if (abs(toFile - fromFile) != abs(toRank - fromRank))
		return false;

	const int fileStep = (toFile - fromFile) > 0 ? 1 : -1;
	const int rankStep = (toRank - fromRank) > 0 ? 1 : -1;

	int file = fromFile + fileStep;
	int rank = fromRank + rankStep;

	while (file != toFile && rank != toRank) {
		int square = rank * 8 + file;
		if (IsOccupied(square))
			return false;

		file += fileStep;
		rank += rankStep;
	}

	return true;
}

bool Board::IsOccupied(uint8_t square) const{
	for (int i = 0; i < 12; i++) {

		if (Utils::GetBitboardValueOnIndex(bitboards[i], square))
			return true;
	}
	return false;
}

bool Board::CanMoveRook(const Move move, const bool _turn) const{
	const uint8_t from = move.from;
	const uint8_t to = move.to;

	if (from == to)
		return false;

	const int fromFile = from % 8;
	const int fromRank = from / 8;
	const int toFile = to % 8;
	const int toRank = to / 8;

	if (_turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	if (fromFile != toFile && fromRank != toRank)
		return false;

	int fileStep = 0;
	int rankStep = 0;

	if (fromFile != toFile)
		fileStep = (toFile - fromFile) > 0 ? 1 : -1;
	else
		rankStep = (toRank - fromRank) > 0 ? 1 : -1;

	int file = fromFile + fileStep;
	int rank = fromRank + rankStep;

	while (file != toFile || rank != toRank) {
		int square = rank * 8 + file;
		if (IsOccupied(square)) {
			// if (debug) std::cerr << "RETURNED FALSE FROM CAN MOVE ROOK\n";
			return false;
		}

		file += fileStep;
		rank += rankStep;
	}

	return true;
}																	
																	
																	
bool Board::CanMoveQueen(const Move move, const bool _turn) const{	
	return CanMoveRook(move, _turn) || CanMoveBishop(move, _turn);	
}																	
																	
bool Board::CanMoveKing(const Move move, const bool _turn) const {	
	const uint8_t from = move.from;									
	const uint8_t to = move.to;										
																	
	if (from == to || from >= 64 || to >= 64)						
		return false;												
																	
	if (!(Engine::kingMasks[from] & (1ULL << to)))
		return false;

	if (_turn == WHITE_TURN) {
		if (Utils::IsWhitePieceAt(*this, to))
			return false;
	}
	else {
		if (Utils::IsBlackPieceAt(*this, to))
			return false;
	}

	return true;
}

void Board::Castle(const Move move)
{
	if (CanCastle(move))
		DoCastleMove(move);
}

bool Board::CanCastle(const Move move) {
	if (!move.castling)
		return false;

	const bool shortCastle = move.mode;

	const bool color = turn;

	if (color == WHITE_TURN) {
		if ((shortCastle && !wCastlingKing) || (!shortCastle && !wCastlingQueen))
			return false;

		if (shortCastle) {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 5) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 6)) {
				return true;
			}
		}
		else {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 1) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 2) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 3)) {
				return true;
			}
		}
	}
	else {
		if ((shortCastle && !bCastlingKing) || (!shortCastle && !bCastlingQueen))
			return false;

		if (shortCastle) {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 61) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 62)) {
				return true;
			}
		}
		else {
			if (!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 57) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 58) &&
				!Utils::GetBitboardValueOnIndex(Utils::GetAllBitboards(bitboards, BOTH), 59)) {
				return true;
			}
		}
	}

	return false;
}


void Board::DoCastleMove(const Move move)
{
	const bool color = turn == WHITE_TURN ? WHITE_TURN : BLACK_TURN;
	const bool shortCastle = move.mode;

	if (color == WHITE_TURN) {

		if (shortCastle) {
			MoveWithoutComprobe(4, 6);
			MoveWithoutComprobe(7, 5);
		}
		else {
			MoveWithoutComprobe(4, 2);
			MoveWithoutComprobe(0, 3);
		}

		wCastlingKing = false;
		wCastlingQueen = false;
	}
	else
	{
		if (shortCastle) {
			MoveWithoutComprobe(60, 62);
			MoveWithoutComprobe(63, 61);
		}
		else {
			MoveWithoutComprobe(60, 58);
			MoveWithoutComprobe(56, 59);
		}
		
		bCastlingKing = false;
		bCastlingQueen = false;
	}

	turn = !turn;
}


Bitboard Board::GetKingAttacks(uint8_t square) {
	Bitboard attacks = 0;
	const int file = square % 8;
	const int rank = square / 8;

	for (int dr = -1; dr <= 1; dr++) {
		for (int df = -1; df <= 1; df++) {
			if (dr == 0 && df == 0) continue;

			int r = rank + dr;
			int f = file + df;

			if (r >= 0 && r < 8 && f >= 0 && f < 8) {
				attacks |= (1ULL << (r * 8 + f));
			}
		}
	}

	return attacks;
}

/*inline bool Board::IsSquareAttacked(const PIECE_COLORS attackerColor, const int square, const bool debug) {
	const bool attackingColor = attackerColor;
	for (size_t i = 0; i < 12; i++) {
			if ((attackingColor == WHITE_TURN && i < 6) ||
				(attackingColor == BLACK_TURN && i >= 6)) {
				continue;
			}

			Bitboard b = bitboards[i];
				while (b) {
					const int from = Utils::PopLSB(b);

					switch (i % 6) {
					case 0: if (CanMovePawn(Move{ from, square }, !attackerColor)) { return true; } break;
					case 1: if (CanMoveKnight(Move{ from, square }, !attackerColor)) { return true; } break;
					case 2: if (CanMoveBishop(Move{ from, square }, !attackerColor)) { return true; } break;
					case 3: if (CanMoveRook(Move{ from, square }, !attackerColor)) { return true; } break;
					case 4: if (CanMoveQueen(Move{ from, square }, !attackerColor)) { return true; } break;
					case 5: if (CanMoveKing(Move{ from, square }, !attackerColor)) { return true; } break;
					}
				}
	}

	return false;
}*/

bool Board::IsSquareAttacked(const PIECE_COLORS attackerColor, const int square, const bool debug) {
	const Bitboard targetBB = 1ULL << square;
	const Bitboard occupancy = Utils::GetAllBitboards(bitboards, BOTH);			// why both is not the default value? I'm so dumb ...

	const bool isWhite = (!attackerColor == WHITE);

	const Bitboard pawnAttackers = bitboards[isWhite ? B_PAWN_I : W_PAWN_I];
	const Bitboard pawnAttacks = isWhite
		? ((targetBB & ~FILE_A_MASK) << 7) | ((targetBB & ~FILE_H_MASK) << 9)
		: ((targetBB & ~FILE_A_MASK) >> 9) | ((targetBB & ~FILE_H_MASK) >> 7);

	if (pawnAttacks & pawnAttackers) return true;

	if (Engine::knightMasks[square] & bitboards[isWhite ? B_KNIGHT_I : W_KNIGHT_I]) return true;

	if (Engine::kingMasks[square] & bitboards[isWhite ? B_KING_I : W_KING_I]) return true;

	const Bitboard bishopLike = bitboards[isWhite ? B_BISHOP_I : W_BISHOP_I] | bitboards[isWhite ? B_QUEEN_I : W_QUEEN_I];
	Bitboard bb = bishopLike;
	
	while (bb) {
		int from = Utils::PopLSB(bb);
		// if (debug)
		// 	std::cout << from << " from\n";
		const Bitboard attacks = Utils::GenerateBishopAttacks(from, occupancy);
		if ((attacks & targetBB) && ((Engine::between[from][square] & occupancy) == 0)) {
			if (debug) {
				/*											i'm so fucking dumb, 
				Utils::PrintBoard(*this);

				std::cout << "Occupancy: \n";
				Utils::DebugBitboard(occupancy);

				std::cout << "Bishop: \n";
				Utils::DebugBitboard(bb);

				std::cout << "Attacks: \n";
				Utils::DebugBitboard(attacks);
			
				std::cout << "Target: \n";
				Utils::DebugBitboard(targetBB);

				std::cout << "Attacks & Target: \n";
				Utils::DebugBitboard(Utils::GenerateBishopAttacks(from, occupancy) & targetBB);

				std::cout << "Between: \n";
				Utils::DebugBitboard(Engine::between[from][square]);*/
			}
			return true;
		}
	}

	bb = bitboards[isWhite ? B_ROOK_I : W_ROOK_I] | bitboards[isWhite ? B_QUEEN_I : W_QUEEN_I];
	while (bb) {
		int from = Utils::PopLSB(bb);
		const Bitboard attacks = Utils::GenerateRookAttacks(from, occupancy);

		if (debug) {
			std::cout << "Rook from: " << from << "\n";
			Utils::DebugBitboard(attacks);
		}

		if ((attacks & targetBB) && ((Engine::between[from][square] & occupancy) == 0)) return true;
	}

	return false;
}

bool Board::IsCheck(const PIECE_COLORS king, const bool debug) {
	const int kingSquare = (king == WHITE)
		? GetWhiteKingPosition()
		: GetBlackKingPosition();

	if (debug) std::cerr << ((king == WHITE) ? "WHITE " : "BLACK ") << kingSquare << "\n";

	return IsSquareAttacked( king==WHITE? BLACK:WHITE, kingSquare, debug);
}

// wtf is this
/*
void Board::SetAllBitboards(uint8_t indexPosition) {
	for (size_t i = 0; i < 12; i++) {
		bitboards[i] |= (1ULL << indexPosition);
	}
}*/

void Board::SetBitboardBit(const int piece, const int indexPosition) {
	if (piece > 12)
		return;

	this->bitboards[piece] |= (1ULL << indexPosition);
}

void Board::ClearBitInAllBitboards(uint8_t indexPosition) {
	for (size_t i = 0; i < 12; i++) {
		bitboards[i] &= ~(1ULL << indexPosition);
	}
}

void Board::MoveWithoutComprobe(const int from, const int to) {
	int movedPiece = 255;
	for (int i = 0; i < 12; i++) {
		if ((bitboards[i] >> from) & 1) {
			movedPiece = i;
			break;
		}
	}

	for (int i = 0; i < 12; i++) {
		if ((bitboards[i] >> to) & 1) {
			bitboards[i] &= ~(1ULL << to);
			ClearHalfMoves();
			break;
		}
	}

	bitboards[movedPiece] &= ~(1ULL << from);
	bitboards[movedPiece] |= (1ULL << to);

	turn = !turn;
}

uint8_t Board::GetWhiteKingPosition(const bool debug)
{
	Bitboard whiteKingBB = bitboards[5]; 

	if (whiteKingBB == 0) {
		return 255;  
	}

	const uint8_t pos = Utils::BitScanForward(whiteKingBB);

	if (debug) std::cerr << "White king found at: " << (int)pos << std::endl;

	return pos;
}

uint8_t Board::GetBlackKingPosition(const bool debug)
{
	Bitboard whiteKingBB = bitboards[11];

	if (whiteKingBB == 0) {
		return 255;
	}

	uint8_t pos = Utils::BitScanForward(whiteKingBB);

	if (debug) std::cerr << "Black king found at: " << (int)pos << std::endl;

	return pos;
}

bool Board::TripleRepetition()
{
	int repetitions = 1;
	const ZobristHash currentHash = Utils::GetZobristHash(*this, Engine::hashSettings);

	for (const auto& position : repetitionsHistory) {
		if (position == currentHash) {
			repetitions++;
		}
	}

	if (repetitions >= 3) {
		std::cerr << "Triple repetition detected.\n";
		return true;
	}

	return false;
}

void Board::ClearHalfMoves()
{
	halfMoves = 0;
	repetitionsHistory.clear();
}
