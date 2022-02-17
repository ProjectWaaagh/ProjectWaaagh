#ifndef PROJECTWAAAGH_APPENDERCONSOLE_HPP
#define PROJECTWAAAGH_APPENDERCONSOLE_HPP

#include "Appender.hpp"

enum ColorTypes : uint8 {
	BLACK,
	RED,
	GREEN,
	BROWN,
	BLUE,
	MAGENTA,
	CYAN,
	GREY,
	YELLOW,
	LRED,
	LGREEN,
	LBLUE,
	LMAGENTA,
	LCYAN,
	WHITE,
	NUM_COLOR_TYPES
};

class PRW_FRAMEWORK_API AppenderConsole : public Appender {
public:
	static constexpr AppenderType type = APPENDER_CONSOLE;

	AppenderConsole(uint8 id, std::string const& name, LogLevel level, AppenderFlags flags, std::vector<std::string_view> const& args);
	void InitColors(std::string const& name, std::string_view init_str);
	AppenderType GetType() const override { return type; };

private:
	void SetColor(bool stdout_stream, ColorTypes color);
	void ResetColor(bool stdout_stream);
	void _Write(LogMessage const* message) override;
	bool mColored;
	ColorTypes mColors[NUM_ENABLED_LOG_LEVELS];
};

#endif // !PROJECTWAAAGH_APPENDERCONSOLE_HPP
