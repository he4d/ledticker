/*
 * This file is part of ledticker
 * ledticker - a ledticker software for lunartec led-dotmatrix 70x5 via rs232
 * Copyright (C) 2015 Tobias Tschinkowitz
 *
 * ledticker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ledticker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ledticker. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LEDTICKER_LEDTICKER_H_
#define LEDTICKER_LEDTICKER_H_

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <algorithm>

extern unsigned char conversionTable[2][67];

bool fileExists(const std::string &filename);
void parseFile(const std::string &filename,
 std::vector<std::string> &inputVector);
std::string generateText(const std::vector<std::string> &text);
void convertTextIso8859_15(std::string &inputText);
int generateChecksum(const std::string &sendString);
void sendSerial(const std::string &sendText);
std::string intToHex(const int &i);
std::ifstream::pos_type getFileSize(const char* filename);
double calculateDisplayTime(const std::ifstream::pos_type &fileSize);

#endif  // LEDTICKER_LEDTICKER_H_
