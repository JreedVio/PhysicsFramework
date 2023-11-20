// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include <iostream>
#include "Printer.h"

using namespace std;

void HelloFromPrinter() {
    printf("HelloFromPrinter\n");
}

void PrintDog()
{
    cout << "   / \__" << endl <<
            "  (    @\___" << endl <<
            "  /         O" << endl <<
            " /   (_____/" << endl << 
            "/_____/   U" << endl;
}

void PrintCat()
{
    cout << " ,_     _" << endl <<
            " |\\_,-~/" << endl <<
            " / _  _ |    ,--." << endl <<
            "(  @  @ )   / ,-'" << endl <<
            " \  _T_/-._( (" << endl <<
            " /         `. \ " << endl <<
            "|         _  \ |" << endl <<
            " \ \ ,  /      |" << endl <<
            "  || |-_\__   /" << endl <<
            " ((_/`(____,-'" << endl;
}
void PrintParrot()
{
    cout << "            .------." << endl <<
            "           /  ~ ~   \,------.      ______" << endl <<
            "         ,'  ~ ~ ~  /  (@)   \   ,'      \." << endl <<
            "       ,'          /`.    ~ ~ \ /         \." << endl <<
            "     ,'           | ,'\  ~ ~ ~ X     \  \  \." << endl <<
            "   ,'  ,'          V--<       (       \  \  \." << endl <<
            " ,'  ,'               (vv      \/\  \  \  |  |" << endl <<
            "(__,'  ,'   /         (vv   ""    \  \  | |  |" << endl <<
            "  (__,'    /   /       vv       \ |  / / /" << endl <<
            "      \__,'   /  |     vv          / / / / /" << endl <<
            "          \__/   / |  | \         / /,',','" << endl <<
            "             \__/\_^  |  \       /,'',','\." << endl <<
            "                    `-^.__>.____/  ' ,'   \." << endl <<
            "                            // //---'      |" << endl <<
            "          ===============(((((((=================" << endl <<
            "                                     | \ \  \." << endl <<
            "                                     / |  |  \." << endl <<
            "                                    / /  / \  \." << endl <<
            "                                    `.     |   \." << endl <<
            "                                      `--------'" << endl;
}
void PrintLine()
{
    cout << "----------------------\n" << endl;
}
