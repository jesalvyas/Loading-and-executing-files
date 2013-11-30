// A simple shell
// Jesal Vyas

char line[80];
char buffer[512];
char fileBuffer[13312];
char typeString[] = "type\0";
char executeString[] = "execute\0";
int processType(char* string);
int processExecute(char* string);

int main() {
	while(1) {
		interrupt(0x21, 0, "Welcome to my shell!", 0, 0);
        interrupt(0x21, 1, line, 0, 0);
        interrupt(0x21, 0, "$\0", 0, 0);
                
        if (processType(line) == 1) {
        	interrupt(0x21, 3, line + 5, fileBuffer, 0);
            interrupt(0x21, 0, fileBuffer, 0, 0);
               
                } else if (processExecute(line) == 1) {
                        interrupt(0x21, 6, line + 8, 0x2000, 0);
                } else {
                        interrupt(0x21, 0, "Bad command!\n\r\0", 0, 0);
                }
        }

        return 0;
}

int processType(char* string) {
        
         int found = 1;
         int i = 0;
         while (i < 7) {
                 if (string[i] != typeString[i]) {
                         found = 0;
                         break;
                 }
                 i++;
         }
         return found;
}

int processExecute(char * string) {
        
         int found = 1;
         int i = 0;
         while (i < 7) {
                 if (string[i] != executeString[i]) {
                         found = 0;
                         break;
                 }
                 i++;
         }
         return found;
}