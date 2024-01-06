#include "slash.h"

int main(int argc, char **argv)
{
	if (argc == 1) FUCK(-1, "Not enough arguments");
	
	return 0;
}

void v_report(const char *fn, unsigned line, report_level_t lvl, const char *fmt, va_list vargs)
{
	char buf[512];
	const char *str;
	uint64_t off;
	switch (lvl)
	{
	case report_level_NONE:
		UNREACHABLE();
		break;
	case report_level_NOTE:
		off = sizeof("NOTE:");
		str = "NOTE:";
		break;
	case report_level_WARNING:
		off = sizeof("WARNING:");
		str = "WARNING:";
		break;
	case report_level_ERROR:
		off = sizeof("ERROR:");
		str = "ERROR:";
		break;
	}
	(void)memcpy(buf, str, off);
	off += snprintf(buf +off - 1, sizeof(buf), "%s:%u", fn, line);
	off += vsnprintf(buf+off-1, sizeof(buf) - off, fmt, vargs);
	(void)fputs(buf, stderr);
}

void report(const char *fn, unsigned line, report_level_t lvl, const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	v_report(fn, line, lvl, fmt, vargs);
	va_end(vargs);
}

void fuck(const char *fn, unsigned line, int code, const char *fmt, ...)
{
	va_list vargs;
	va_start(vargs, fmt);
	v_report(fn, line, report_level_ERROR, fmt, vargs);
	va_end(vargs);
	exit(code);
}
