// A minimal test runner: TOAST_TEST registers a function, CHECK fails it,
// SKIP marks it not run. Run all tests with `toast_tests`, or one with
// `toast_tests <name>`; a single skipped test exits with SKIP_EXIT_CODE.

#ifndef TOASTTEST_H
#define TOASTTEST_H

#include <functional>
#include <string>
#include <vector>

namespace ToastTest
{
	struct Case
	{
		std::string name;
		std::function<void()> body;
	};

	// ctest reports this exit code as "Not Run" (SKIP_RETURN_CODE)
	const int SKIP_EXIT_CODE = 77;

	std::vector<Case>& registry();
	void fail(const char* file, int line, const char* expr);
	void skip(const char* reason);

	struct Registrar
	{
		Registrar(const char* name, std::function<void()> body) { registry().push_back(Case{name, body}); }
	};
}

#define TOAST_TEST(name) \
	static void name(); \
	static ToastTest::Registrar name##_registrar(#name, name); \
	static void name()

#define CHECK(cond) \
	do { if(!(cond)) { ToastTest::fail(__FILE__, __LINE__, #cond); return; } } while(0)

#define SKIP(reason) \
	do { ToastTest::skip(reason); return; } while(0)

#endif
