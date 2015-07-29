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

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "ledticker.h"

#define deviceFile "/dev/ttyUSB0"

unsigned char conversionTable[2][67]  = {
  { 0x0A, 0x0D, 0xC3, 0xC2, 0xC1, 0xC0, 0xC4, 0xC5, 0xC6, 0xDF,
    0xC7, 0xD0, 0xC9, 0xCA, 0xC8, 0xCB, 0xCD, 0xCC, 0xCE, 0xCF,
    0xD1, 0xD3, 0xD4, 0xD2, 0xD6, 0xD5, 0xD8, 0xDE, 0xDA, 0xD9,
    0xDB, 0xDC, 0xBE, 0xDD, 0xE3, 0xE2, 0xE1, 0xE0, 0xE4, 0xE5,
    0xE6, 0xE7, 0xE9, 0xEA, 0xE8, 0xEB, 0xED, 0xEC, 0xEE, 0xEF,
    0xF1, 0xF3, 0xF4, 0xF2, 0xF6, 0xF5, 0xF8, 0xFE, 0xFA, 0xF9,
    0xFB, 0xFC, 0xFF, 0xFD, 0xA5, 0xA3, 0xA4 },
  { 0x20, 0x20, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86,
    0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90,
    0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A,
    0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4,
    0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE,
    0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8,
    0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF }
};

/*
 * fileExists() is just checking if a file exists and returns true if yes
 * or false if it doesnt exist.
 */
bool fileExists(const std::string &filename) {
  std::ifstream theFile;
  theFile.open(filename.c_str());
  if(!theFile.is_open()) {
    std::cerr << "File not found!!" << std::endl;
    return false;
  } else {
  theFile.close();
  return true;
}
}

/*
 * parseFile() is opening the input file that was given by console and takes
 * a vector as reference where it puts all the lines out of the input file into
 * a new vector element.
 */
void parseFile(const std::string &filename,
  std::vector<std::string> &inputVector) {
  std::string temp;
  std::ifstream theFile;
  theFile.open(filename.c_str());
  if(!theFile.is_open()) {
    std::cerr << "Could not open the file!" << std::endl;
  }
  else {
    while(std::getline(theFile,temp)) {
      inputVector.push_back(temp);
    }
    theFile.close();
  }
}

/*
 * generateText() is taking a vector with the lines of the input file and
 * adds all the necessary config values for the device.
 * <ID00> = ID of the designated LED board
 * <L1>   = Print on Line 1
 * <PB>   = Write to Page B (420 characters max)
 * <FE>   = Display always scrolling
 * <MB>   = Fast speed
 * <WC>   = Must fill
 * <FE>   = Must fill
 * <E>    = End of Transmission
 * For more details see the interface manual "PE9441_12_65915.pdf"
 * The final string which is written to the serial file looks like this:
 * <ID00><L1><PB><FE><MB><WC><FE>text&checksum<E>
 * All the lines of the vector will be written in one sendString, so they appear
 * one by one.
 */
std::string generateText(const std::vector<std::string> &text) {
  std::string initID = "<ID00>";
  std::string constStr = "<L1><PB><FE><MB><WC><FE>";
  std::string inputLines;
  for(auto it = begin(text); it < end(text); it++) {
    inputLines.append(*it);
    if(it != end(text)-1)
    inputLines.append(" ");
  }
  convertTextIso8859_15(inputLines);
  constStr.append(inputLines);
  constStr.append(intToHex(generateChecksum(constStr)));
  initID.append(constStr);
  initID.append("<E>");
  return initID;
}

/*
 * convertTextIso8859_15() converts the string which was read from the file into
 * Iso8859_15 chars, because otherwise "umlauts" will not be displayed correctly
 */
void convertTextIso8859_15(std::string &inputText) {
  for(int i = 0; i < 67; i++) {
    std::replace( inputText.begin(), inputText.end(),
    conversionTable[0][i], conversionTable[1][i]);
  }
}

/*
 * generateChecksum() is taking the sendString and calculates the checksum of it
 * For more details see the interface manual "PE9441_12_65915.pdf"
 */
int generateChecksum(const std::string &sendString) {
  int value = 0;
  for(int i = 0; i < sendString.length(); i++) {
    value ^= (int)sendString[i];
  }
  return value;
}

/*
 * sendSerial() just opens up the device file which is defined on top and writes
 * the complete sendString into it.
 */
void sendSerial(const std::string &sendText) {
  std::fstream file(deviceFile, std::fstream::out);
  if (file) {
    file << sendText;
    file.close();
  } else {
    std::cerr << "Could not open file..." << std::endl;
  }
}

/*
 * intToHex() takes an int value and converts it into a hexadecimal value.
 * This is necessary because the checksum that have to be generated must be in
 * hex-format.
 */
std::string intToHex(const int &i)
{
  std::ostringstream oss;
  oss << std::hex << i;
  return oss.str();
}

/*
 * getFileSize() gets the file Size of the input file.
 */
std::ifstream::pos_type getFileSize(const char* filename)
{
  std::ifstream in(filename, std::ios::ate | std::ios::binary);
  return in.tellg();
}

/*
 * calculateDisplayTime() takes the fileSize and calculates, how long the lines
 * of the input file will need to run completely through the dot-matrix.
 * In speed mode B as we use, one character needs about 9 seconds to run
 * completely through. The other chars just get multiplied by
 * 0.68 which might have to be adjusted to the correct value
 */
double calculateDisplayTime(const std::ifstream::pos_type &fileSize) {
  double time = 9.0+((int)fileSize*0.68);
  return time;
}
