#define M_CE_UCI
#include "engine.h"
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <map>

class UciOption {
public:
	virtual ~UciOption() {}
	std::string name;
	virtual void set_value(const std::string& val) = 0;
	virtual std::string to_uci_string() const = 0;
};

class SpinOption : public UciOption {
public:

	const int default_value = 1;
	int value, min, max;
	void set_value(const std::string& val) override {
		int v = std::stoi(val);
		if (v >= min && v <= max)
			value = v;
	}

	std::string to_uci_string() const override {
		std::ostringstream oss;
		oss << "option name " << name
			<< " type spin"
			<< " default " << default_value
			<< " min " << min
			<< " max " << max;
		return oss.str();
	}
};

class CheckOption : public UciOption {
public:
	const bool default_value = false;
	bool value;
	void set_value(const std::string& val) override {
		value = (val == "true");
	}

	std::string to_uci_string() const override {
		std::ostringstream oss;
		oss << "option name " << name
			<< " type check"
			<< " default " << (default_value ? "true" : "false");
		return oss.str();
	}
};

class ComboOption : public UciOption {
public:
	const std::string default_value = "";
	std::vector<std::string> values;
	std::string value;
	void set_value(const std::string& val) override {
		if (std::find(values.begin(), values.end(), val) != values.end())
			value = val;
	}

	std::string to_uci_string() const override {
		std::ostringstream oss;
		oss << "option name " << name
			<< " type combo"
			<< " default " << default_value;
		for (const auto& v : values)
			oss << " var " << v;
		return oss.str();
	}
};

class StringOption : public UciOption {
public:
	const std::string default_value = "";
	std::string value;
	void set_value(const std::string& val) override {
		value = val;
	}

	std::string to_uci_string() const override {
		std::ostringstream oss;
		oss << "option name " << name
			<< " type string"
			<< " default " << default_value;
		return oss.str();
	}
};

class ButtonOption : public UciOption {
public:
	std::function<void()> callback;
	void set_value(const std::string&) override {
		callback();
	}

	std::string to_uci_string() const override {
		std::ostringstream oss;
		oss << "option name " << name
			<< " type button";
		return oss.str();
	}
};


class Engine;
class UCI
{
private:
	const char* m_uciName = "M_CE";
	const char* m_uciAuthor = "Marc Maravall Díez";

	Engine engine;

public:
	UCI();

	std::map<std::string, std::unique_ptr<UciOption>> options;

	void SendInfo();

	void uci();
	void isready();
	void init_options();
	void setoption(const char* name, const char* value);
	void register_uci();
	void ucinewgame();
	void position(const char* fen);
	// void go();
	void stop();
	void ponderhit();
	void quit();
	void perft(int depth);
	void draw();

	void ManageInput(const char* input);

	void SpeedTest();
	void EvalTest();
	void UndoMoveTest();
	void NPSTest();
	void SearchTest();
	void InsertCommand(const char* command);

	// static bool IsSearching;

	bool sendInfo = true;
};
