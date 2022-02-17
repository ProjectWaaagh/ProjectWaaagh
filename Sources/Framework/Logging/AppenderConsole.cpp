#include "AppenderConsole.hpp"
#include "LogMessage.hpp"
#include "StringFormat.hpp"
#include "StringConverter.hpp"
#include "Utilities.hpp"
#include "SmartEnum.hpp"

#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
#	include <Windows.h>
#endif // PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS

AppenderConsole::AppenderConsole(uint8 id, std::string const& name, LogLevel level, AppenderFlags flags, std::vector<std::string_view> const& args) :
	Appender(id, name, level, flags), mColored(false) {
	for (uint8 i = 0; i < NUM_ENABLED_LOG_LEVELS; ++i) {
		mColors[i] = ColorTypes(NUM_COLOR_TYPES);
	}

	if (3 < args.size())
		InitColors(name, args[3]);
}


void AppenderConsole::InitColors(std::string const& name, std::string_view str) {
	if (str.empty()) {
		mColored = false;
		return;
	}

	std::vector<std::string_view> colorStr = ProjectWaaagh::Tokenize(str, ' ', false);
	if (colorStr.size() != NUM_ENABLED_LOG_LEVELS) {
		throw InvalidAppenderArgsException(ProjectWaaagh::StringFormat("Log::CreateAppenderFromConfig: Invalid color data '%s' for console appender %s (expected %u entries, got %zu)",
			std::string(str).c_str(), name.c_str(), NUM_ENABLED_LOG_LEVELS, colorStr.size()));
	}

	for (uint8 i = 0; i < NUM_ENABLED_LOG_LEVELS; ++i) {
		if (Optional<uint8> color = ProjectWaaagh::StringTo<uint8>(colorStr[i]); color && EnumUtils::IsValid<ColorTypes>(*color))
			mColors[i] = static_cast<ColorTypes>(*color);
		else {
			throw InvalidAppenderArgsException(ProjectWaaagh::StringFormat("Log::CreateAppenderFromConfig: Invalid color '%s' for log level %s on console appender %s",
				std::string(colorStr[i]).c_str(), EnumUtils::ToTitle(static_cast<LogLevel>(i)), name.c_str()));
		}
	}

	mColored = true;
}

void AppenderConsole::SetColor(bool stdout_stream, ColorTypes color) {
#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
	static WORD WinColorFG[NUM_COLOR_TYPES] = {
		0,																							// Black
		FOREGROUND_RED,																				// Red
		FOREGROUND_GREEN,																			// Green
		FOREGROUND_RED		| FOREGROUND_GREEN,														// Brown
		FOREGROUND_BLUE,																			// Blue
		FOREGROUND_RED		| FOREGROUND_BLUE,														// Magenta
		FOREGROUND_GREEN	| FOREGROUND_BLUE,														// Cyan
		FOREGROUND_RED		| FOREGROUND_GREEN		| FOREGROUND_BLUE,								// White
		FOREGROUND_RED		| FOREGROUND_GREEN		| FOREGROUND_INTENSITY,							// Yellow
		FOREGROUND_RED		| FOREGROUND_INTENSITY,													// Bold Red
		FOREGROUND_GREEN	| FOREGROUND_INTENSITY,													// Bold Green
		FOREGROUND_BLUE		| FOREGROUND_INTENSITY,													// Bold Blue
		FOREGROUND_RED		| FOREGROUND_BLUE		| FOREGROUND_INTENSITY,							// Bold Magenta
		FOREGROUND_GREEN	| FOREGROUND_BLUE		| FOREGROUND_INTENSITY,							// Bold Cyan
		FOREGROUND_RED		| FOREGROUND_GREEN		| FOREGROUND_BLUE		| FOREGROUND_INTENSITY	// Bold White
	};

	HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
	SetConsoleTextAttribute(hConsole, WinColorFG[color]);
#else
	enum ANSITextAttr {
		TA_NORMAL			= 0,
		TA_BOLD				= 1,
		TA_BLINK			= 5,
		TA_REVERSE			= 7
	};

	enum ASNIFgTextAttr {
		FG_BLACK			= 30,
		FG_RED
		FG_GREEN
		FG_BROWN
		FG_BLUE
		FG_MAGENTA
		FG_CYAN
		FG_WHITE
		FG_YELLOW
	};

	enum ANSIBgTextAttr {
		BG_BLACK			= 30,
		BG_RED
		BG_GREEN
		BG_BROWN
		BG_BLUE
		BG_MAGENTA
		BG_CYAN
		BG_WHITE
		BG_YELLOW
	};

	static uint8 UnixColorFG[NUM_COLOR_TYPES] = {
		FG_BLACK,			// Black
		FG_RED,				// Red
		FG_GREEN,			// Green
		FG_BROWN,			// Brown
		FG_BLUE,			// Blue
		FG_MAGENTA,			// Magenta
		FG_CYAN,			// Cyan
		FG_WHITE,			// White
		FG_YELLOW,			// Yellow

		FG_RED,				// Light Red
		FG_GREEN,			// Light Green
		FG_BLUE,			// Light Blue
		FG_MAGENTA,			// Light Magenta
		FG_CYAN,			// Light Cyan
		FG_WHITE			// Light White
	};

	fprintf((stdout_stream ? stdout : stderr), "\x1b[%d%sm"m UnixColorFG[color], (color >= YELLOW && color < NUM_COLOR_TYPES ? ";1" : ""));
#endif
}

void AppenderConsole::ResetColor(bool stdout_stream) {
#if PROJECTWAAAGH_PLATFORM == PROJECTWAAAGH_PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
	fprintf((stdout_stream ? stdout : stderr), "\x1b[0m");
#endif
}

void AppenderConsole::_Write(LogMessage const* message) {
	bool stdout_stream = !(message->level == LOG_LEVEL_ERROR || message->level == LOG_LEVEL_FATAL);

	if (mColored) {
		uint8 index;
		switch (message->level) {
		case LOG_LEVEL_TRACE:
			index = 5;
			break;
		case LOG_LEVEL_DEBUG:
			index = 4;
			break;
		case LOG_LEVEL_INFO:
			index = 3;
			break;
		case LOG_LEVEL_WARN:
			index = 2;
			break;
		case LOG_LEVEL_FATAL:
			index = 0;
			break;
		case LOG_LEVEL_ERROR:
			[[fallthrough]];
		default:
			index = 1;
			break;
		}

		SetColor(stdout_stream, mColors[index]);
        utf8printf(stdout_stream ? stdout : stderr, "%s%s\n", message->prefix.c_str(), message->text.c_str());
        ResetColor(stdout_stream);
	} else
        utf8printf(stdout_stream ? stdout : stderr, "%s%s\n", message->prefix.c_str(), message->text.c_str());
}
