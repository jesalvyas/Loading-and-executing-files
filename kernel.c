// A simple kernel that loads and executes files
// Jesal Vyas

void printString(char* string);
void readString(char* buffer);
void readSector(char* buffer, int sector);
void readFile(char* filename, int segment);
void executeProgram(char* name, int segment);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void terminate();
int mod (int a, int b);
 
char input[80];
char buffer[13312];
//~~~~~~~~~~~~~~~~~~~~~~~~~~ main function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int main() {
  makeInterrupt21();
  //interrupt(0x21, 3, "message\0", buffer, 0); ~~~~~~~Step 1
  //interrupt(0x21, 3, buffer, 0, 0);
  //printString("\r\n\0");
  //interrupt(0x21, 6, "tstprg\0", 0x2000, 0);  ~~~~~~~Step 2
  //printString("\r\n\0");
  //interrupt(0x21, 7, "tstpr2\0", 0x2000, 0);  ~~~~~~~Step 3
  //printString("\r\n\0");
	interrupt(0x21, 6, "shell\0", 0x2000, 0); //  ~~~~~~~Step 4
  interrupt(0x21, 7, 0, 0, 0);
  
  while(1);
}

//~~~~~~~~~~~~~~~~~~ function for printing to screen ~~~~~~~~~~~~~~~~~~
void printString(char* string) {
	int index = 0;
	char c = string[index];
	while (c != 0) {
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
		c = string[++index];
	}
}

// ~~~~~~~~~~~~~~~~ function to read from the keyboard ~~~~~~~~~~~~~~~~
void readString(char* string) {
	int index = 0;
	char c = 0;
	while (1) {
		c = interrupt(0x16, 0, 0, 0, 0);
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
		if (c == 0xD) break;
		string[index] = c;
		++index;
	}
	string[index] = '\n';
	string[index + 1] = 0;
}

// ~~~~~~~~~~~~~~ function to read a sector from disk ~~~~~~~~~~~~~~~~~
void readSector(char* buffer, int sector) {
  int CH = sector/36;
  int CL = mod (sector, 18);
  int DH = mod (sector/18, 2);
  interrupt(0x13, 2 * 256 + 1, buffer, CH * 256 + CL + 1, DH * 256 + 0);
}

// ~~~~~~~~~~~~~~~~~~~~ function to read a file ~~~~~~~~~~~~~~~~~~~~~~
void readFile(char* filename, int segment) {
	    int i,j;
        char directory[512];
        char filename[7];
        char test[1000];

        readSector(directory, 2);
        for (i = 0; i < 512; i+=32) {
                for(j = 0; j < 6; j++) {
                        if(directory[i + j] != filename[j]) break;
                }
                if(j == 6) break;
        }
        if (j != 6) {
          return;
        }
        for (j = 6; j < 32; j++) {
                if(directory[i + j] == 0) {
                	break;
                } 
                readSector(buffer + (j - 6) * 512, directory[i + j]);
        }
}
void executeProgram(char* name, int segment) {
  char buffer[0x1000];
  int i = 0;
  readFile(name, buffer);
  
  while ( i < 0x1000) {
      putInMemory(segment, i, buffer[i]);
      i++;
   }
   launchProgram(segment);
}

// ~~~~~~~~~~~~~~ function for calculating remainder ~~~~~~~~~~~~~~~~~~~
int mod(int a, int b) {
        while (a >= b) {
        	a = a - b;
        }
        return a;
}

// ~~~~~~~~~~~~~~~~~~ function to handle interrupt ~~~~~~~~~~~~~~~~~~~~~
void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	} else if (ax == 1) {
		readString(bx);
	} else if (ax == 2) {
		readSector(bx, cx);
	} else if (ax == 3) {
		readFile(bx, cx);
	} else if (ax = 4) {
    executeProgram(bx, cx);
  } else if (ax = 5) {
    terminate();
  } else {
		printString("Interrupt21 error \r\n\0");
  }
	
}

// ~~~~~~~~~~~~~~~~~ function to terminate ~~~~~~~~~~~~~~~~~~~~~~
void terminate() {
        interrupt(0x21, 4, "shell\0", 0x2000, 0);
}


