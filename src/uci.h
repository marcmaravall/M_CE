#define M_CE_UCI

class Engine;
class UCI
{
private:
	const char* m_uciName = "M_CE";
	const char* m_uciAuthor = "Marc Maravall Díez";

public:
	void uci();
	void isready();
	void setoption(const char* name, const char* value);
	void register_uci();
	void ucinewgame();
	void position(const char* fen);
	void go();
	void stop();
	void ponderhit();
	void quit();
};