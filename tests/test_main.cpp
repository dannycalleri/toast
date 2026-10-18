#include "ToastTest.h"

#include <SDL3/SDL_main.h>
#include <cstdio>

namespace ToastTest
{
	static int failures = 0;
	static int skips = 0;

	std::vector<Case>& registry()
	{
		static std::vector<Case> cases;
		return cases;
	}

	void fail(const char* file, int line, const char* expr)
	{
		std::fprintf(stderr, "  FAILED %s:%d: %s\n", file, line, expr);
		failures++;
	}

	void skip(const char* reason)
	{
		std::printf("  skipped: %s\n", reason);
		skips++;
	}
}

int main(int argc, char* argv[])
{
	int ran = 0;
	for(const ToastTest::Case& c : ToastTest::registry())
	{
		if(argc > 1 && c.name != argv[1])
			continue;

		int failuresBefore = ToastTest::failures;
		int skipsBefore = ToastTest::skips;
		c.body();
		const char* status = ToastTest::failures != failuresBefore ? "FAIL"
							: ToastTest::skips != skipsBefore ? "skip" : "ok  ";
		std::printf("%s %s\n", status, c.name.c_str());
		ran++;
	}

	if(ran == 0)
	{
		std::fprintf(stderr, "no test named %s\n", argc > 1 ? argv[1] : "(any)");
		return 1;
	}
	if(ToastTest::failures > 0)
		return 1;
	return ToastTest::skips == ran ? ToastTest::SKIP_EXIT_CODE : 0;
}
