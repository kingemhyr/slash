#pragma once

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT(E) assert(E)

#define UNREACHABLE(E) ASSERT("UNREACHABLE" && 0)
