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

#include "ledticker.h"

int main(int argc, char **argv) {
  if (argc < 2 ) {
		std::cout << "Usage: ledticker <filename>\n"
				<< "Examples:\n"
				<< "  ledticker /var/log/foo.log\n";
		return 0;
	} else {
    std::ifstream::pos_type fileSizeOld = 0;
    std::ifstream::pos_type fileSizeNew = 0;
    std::vector<std::string> allEntrys;
    if (fileExists(argv[1])) {
    double elapsedTime = 0.0;
    for(;;) {
      fileSizeOld = getFileSize(argv[1]);
      if(fileSizeNew != fileSizeOld && elapsedTime <= 0.0) {
        elapsedTime = calculateDisplayTime(fileSizeOld);
        parseFile(argv[1], allEntrys);
        sendSerial(generateText(allEntrys));
        allEntrys.clear();
        fileSizeNew = fileSizeOld;
      }
      sleep(1);
      elapsedTime -= 1.0;
    }
  }
  return 0;
}
}
