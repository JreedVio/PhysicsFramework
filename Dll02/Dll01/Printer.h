// MathLibrary.h - Contains declarations of math functions
#pragma once

#ifdef PRINTER_EXPORTS
#define PRINTER_API __declspec(dllexport)
#else
#define PRINTER_API __declspec(dllimport)
#endif

extern "C" PRINTER_API void PrintDog();
extern "C" PRINTER_API void PrintCat();
extern "C" PRINTER_API void PrintParrot();
extern "C" PRINTER_API void PrintLine();