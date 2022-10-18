#pragma once

#include "String.hpp"
#include "Exception.hpp"
#include "Console.hpp"

enum class ExitStatus
{
	OK,
	ERROR,
};

ExitStatus Start();

int main();

void Exit(ExitStatus exitStatus);