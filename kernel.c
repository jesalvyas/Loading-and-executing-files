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

int main {
	

	makeInterrupt21();
    interrupt(0x21, 3, "message\0", buffer, 0);
    interrupt(0x21, 3, buffer, 0, 0);

	while(1);
}



void printString(char* string) {
	int index = 0;
	char c = string[index];
	while (c != 0) {
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
		c = string[++index];
	}
}

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

void readFile(char* filename, int segment); {
	    int i,j;
        char directory[512];
        char dirFilename[7];
        char test[1000];

        readSector(dir, 2);

        for (i = 0; i < 512; i+=32) {
                for(j = 0; j < 6; j++) {
                        if(directory[i + j] != filename[j]) break;
                }
                if(j == 6) break;
        }
        if (j != 6) return;

        for (j = 6; j < 32; j++) {
                if(directory[i + j] == 0) {
                	break;
                } 
                	
                readSector(buffer + (j - 6) * 512, directory[i + j]);
        }
}
void executeProgram(char* name, int segment) {
        char buffer[0x1000];
        int i;

        if(mod(segment, 0x1000) != 0 || segment <= 0x1000 || segment > 0xA000) {
        	return;
        }     
        readFile(name, buffer);
        
        for(i = 0; i < 0x1000; i++) {
                putInMemory(segment, i, buffer[i]);
        }

        launchProgram(segment);
}


int mod(int a, int b) {
        while (a >= b) {
        	a = a - b;
        }
                
        return a;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
	if (ax == 0) {
		printString(bx);
	} else if (ax == 1) {
		readString(bx);
	} else if (ax == 2) {
		readSector(bx, cx);
	} else if (ax == 3) {
		readFile(bx, cx);
	} else {
		printString("Interrupt21 error \r\n\0");
    }
	
}

void terminate() {
        interrupt(0x21, 4, "shell\0", 0x2000, 0);
}


