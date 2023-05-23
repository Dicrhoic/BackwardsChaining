/*
 * main.cpp
 *
 *  Created on: 12 Oct 2020
 *      Author: Home
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

#include "reasoner.h"

int main(){
	predicate p;
	p.storeWords();
	p.testKB();
}
