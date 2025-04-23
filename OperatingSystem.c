/*
 Robert Kulow
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char p0[2];
char p1[2];
char p2[2];
char p3[2];
int pc = 0;
char sp[2];
char psw[2];
char acc[4];
char r0[4];
char r1[4];
char r2[4];
char r3[4];
char ir[6];
int pid = 0;
int fin = 0;

struct pcb{
	char r0[4], r1[4], r2[4], r3[4];
	int pc, pid;
	char p0[2], p1[2], p2[2], p3[2];
	char psw[2];
	char ir[6], acc[4];
	int bar, limitReg;
	struct pcb *next_pcb;
	int ic;
	int fin;
};

struct semaphore{
	int count;
	struct pcb *semQueue;
};

struct semaphore que[5];

void imm(char* x, char* y, char bits[6]){
	*x = bits[4];
	*y = bits[5];
}

void add(char* x, char* y, char bits[6], char p[2]){
	int num1 = (((int)bits[4] - 48) * 10);
	int num2 = (((int)p[0] - 48) * 10);
	int total;
	num1 += ((int)bits[5] - 48);
	num2 += ((int)p[1] - 48);
	total = num1 + num2;
	*x = (char)((total/10) + 48);
	*y = (char)((total%10) + 48);
}

void addacc(char* x, char* y, char* z, char* a, char ir[6]){
	int inputnum = (((int)ir[2] - 48) * 1000);
	inputnum += (((int)ir[3] - 48) * 100);
	inputnum += (((int)ir[4] - 48) * 10);
	inputnum += ((int)ir[5] - 48);
	int accnum = (((int)*x - 48) * 1000);
	int total;
	accnum += (((int)*y - 48) * 100);
	accnum += (((int)*z - 48) * 10);
	accnum += ((int)*a - 48);
	total = accnum + inputnum;
	*x = (char)((total/1000) + 48);
	*y = (char)(((total/100)%10) + 48);
	*z = (char)(((total/10)%10) + 48);
	*a = (char)((total%10) + 48);
}

void addrn(char* x, char* y, char* z, char* a, char r[4]){
	int inputnum = (((int)r[0] - 48) * 1000);
	inputnum += (((int)r[1] - 48) * 100);
	inputnum += (((int)r[2] - 48) * 10);
	inputnum += ((int)r[3] - 48);
	int accnum = (((int)*x - 48) * 1000);
	int total;
	accnum += (((int)*y - 48) * 100);
	accnum += (((int)*z - 48) * 10);
	accnum += ((int)*a - 48);
	total = accnum + inputnum;
	*x = (char)((total/1000) + 48);
	*y = (char)(((total/100)%10) + 48);
	*z = (char)(((total/10)%10) + 48);
	*a = (char)((total%10) + 48);
}

void sub(char* x, char* y, char bits[6], char p[2]){
	int num1 = (((int)bits[4] - 48) * 10);
	int num2 = (((int)p[0] - 48) * 10);
	int total;
	num1 += ((int)bits[5] - 48);
	num2 += ((int)p[1] - 48);
	total = num1 - num2;
	*x = (char)((total/10) + 48);
	*y = (char)((total%10) + 48);
}

void subacc(char* x, char* y, char* z, char* a, char ir[6]){
	int inputnum = (((int)ir[2] - 48) * 1000);
	inputnum += (((int)ir[3] - 48) * 100);
	inputnum += (((int)ir[4] - 48) * 10);
	inputnum += ((int)ir[5] - 48);
	int accnum = (((int)*x - 48) * 1000);
	int total;
	accnum += (((int)*y - 48) * 100);
	accnum += (((int)*z - 48) * 10);
	accnum += ((int)*a - 48);
	total = accnum - inputnum;
	*x = (char)((total/1000) + 48);
	*y = (char)(((total/100)%10) + 48);
	*z = (char)(((total/10)%10) + 48);
	*a = (char)((total%10) + 48);
}

void subrn(char* x, char* y, char* z, char* a, char r[4]){
	int inputnum = (((int)r[0] - 48) * 1000);
	inputnum += (((int)r[1] - 48) * 100);
	inputnum += (((int)r[2] - 48) * 10);
	inputnum += ((int)r[3] - 48);
	int accnum = (((int)*x - 48) * 1000);
	int total;
	accnum += (((int)*y - 48) * 100);
	accnum += (((int)*z - 48) * 10);
	accnum += ((int)*a - 48);
	total = accnum - inputnum;
	*x = (char)((total/1000) + 48);
	*y = (char)(((total/100)%10) + 48);
	*z = (char)(((total/10)%10) + 48);
	*a = (char)((total%10) + 48);
}

void loadacc(char* x, char* y, char* z, char* a, char bits[6]){
	*x = bits[2];
	*y = bits[3];
	*z = bits[4];
	*a = bits[5];
}

void loadmem(char* x, char* y, char* z, char* a, char bits[4]){
	*x = bits[0];
	*y = bits[1];
	*z = bits[2];
	*a = bits[3];
}

char equal(char ac[4], char pn[6]){
	int tacc = (((int)ac[0] - 48) * 1000);
	tacc += (((int)ac[1] - 48) * 100);
	tacc += (((int)ac[2] - 48) * 10);
	tacc += ((int)ac[3] - 48);
	int p = (((int)pn[2] - 48) * 1000);
	p += (((int)pn[3] - 48) * 100);
	p += (((int)pn[4] - 48) * 10);
	p += ((int)pn[5] - 48);
	if (tacc == p){
		return 'T';
	}
	else{
		return 'F';
	}
}

char equalrn(char ac[4], char rn[4]){
	int tacc = (((int)ac[0] - 48) * 1000);
	tacc += (((int)ac[1] - 48) * 100);
	tacc += (((int)ac[2] - 48) * 10);
	tacc += ((int)ac[3] - 48);
	int r = (((int)rn[0] - 48) * 1000);
	r += (((int)rn[1] - 48) * 100);
	r += (((int)rn[2] - 48) * 10);
	r += ((int)rn[3] - 48);
	if (tacc == r){
		return 'T';
	}
	else{
		return 'F';
	}
}

char less(char ac[4], char pn[6]){
	int tacc = (((int)ac[0] - 48) * 1000);
	tacc += (((int)ac[1] - 48) * 100);
	tacc += (((int)ac[2] - 48) * 10);
	tacc += ((int)ac[3] - 48);
	int p = (((int)pn[2] - 48) * 1000);
	p += (((int)pn[3] - 48) * 100);
	p += (((int)pn[4] - 48) * 10);
	p += ((int)pn[5] - 48);
	if (tacc < p){
		return 'T';
	}
	else{
		return 'F';
	}
}

char lessrn(char ac[4], char rn[4]){
	int tacc = (((int)ac[0] - 48) * 1000);
	tacc += (((int)ac[1] - 48) * 100);
	tacc += (((int)ac[2] - 48) * 10);
	tacc += ((int)ac[3] - 48);
	int r = (((int)rn[0] - 48) * 1000);
	r += (((int)rn[1] - 48) * 100);
	r += (((int)rn[2] - 48) * 10);
	r += ((int)rn[3] - 48);
	if (tacc < r){
		return 'T';
	}
	else{
		return 'F';
	}
}

char greater(char ac[4], char pn[6]){
	int tacc = (((int)ac[0] - 48) * 1000);
	tacc += (((int)ac[1] - 48) * 100);
	tacc += (((int)ac[2] - 48) * 10);
	tacc += ((int)ac[3] - 48);
	int p = (((int)pn[2] - 48) * 1000);
	p += (((int)pn[3] - 48) * 100);
	p += (((int)pn[4] - 48) * 10);
	p += ((int)pn[5] - 48);
	if (tacc > p){
		return 'T';
	}
	else{
		return 'F';
	}
}

char greaterrn(char ac[4], char rn[4]){
	int tacc = (((int)ac[0] - 48) * 1000);
	tacc += (((int)ac[1] - 48) * 100);
	tacc += (((int)ac[2] - 48) * 10);
	tacc += ((int)ac[3] - 48);
	int r = (((int)rn[0] - 48) * 1000);
	r += (((int)rn[1] - 48) * 100);
	r += (((int)rn[2] - 48) * 10);
	r += ((int)rn[3] - 48);
	if (tacc > r){
		return 'T';
	}
	else{
		return 'F';
	}
}

void mod(char rx[4], char ry[4], char* x, char* y, char* z, char* a){
	int i = (((int)rx[0] - 48) * 1000);
	i += (((int)rx[1] - 48) * 100);
	i += (((int)rx[2] - 48) * 10);
	i += ((int)rx[3] - 48);
	int j = (((int)ry[0] - 48) * 1000);
	j += (((int)ry[1] - 48) * 100);
	j += (((int)ry[2] - 48) * 10);
	j += ((int)ry[3] - 48);
	int result = i % j;
	*x = (char)((result/1000) + 48);
	*y = (char)(((result/100) % 10) + 48);
	*z = (char)(((result/10) % 10) + 48);
	*a = (char)((result % 10) + 48);
}

void savePCB(struct pcb *tmp){
	tmp->pid = pid;
	tmp->bar = pid * 100;
	tmp->r0[0] = r0[0];
	tmp->r0[1] = r0[1];
	tmp->r0[2] = r0[2];
	tmp->r0[3] = r0[3];
	tmp->r1[0] = r1[0];
	tmp->r1[1] = r1[1];
	tmp->r1[2] = r1[2];
	tmp->r1[3] = r1[3];
	tmp->r2[0] = r2[0];
	tmp->r2[1] = r2[1];
	tmp->r2[2] = r2[2];
	tmp->r2[3] = r2[3];
	tmp->r3[0] = r3[0];
	tmp->r3[1] = r3[1];
	tmp->r3[2] = r3[2];
	tmp->r3[3] = r3[3];
	tmp->pc = pc;
	tmp->p0[0] = p0[0];
	tmp->p0[1] = p0[1];
	tmp->p1[0] = p1[0];
	tmp->p1[1] = p1[1];
	tmp->p2[0] = p2[0];
	tmp->p2[1] = p2[1];
	tmp->p3[0] = p3[0];
	tmp->p3[1] = p3[1];
	tmp->psw[0] = psw[0];
	tmp->psw[1] = psw[1];
	tmp->ir[0] = ir[0];
	tmp->ir[1] = ir[1];
	tmp->ir[2] = ir[2];
	tmp->ir[3] = ir[3];
	tmp->ir[4] = ir[4];
	tmp->ir[5] = ir[5];
	tmp->acc[0] = acc[0];
	tmp->acc[1] = acc[1];
	tmp->acc[2] = acc[2];
	tmp->acc[3] = acc[3];
	tmp->fin = fin;
}

void loadPCB(struct pcb *tmp){
	pid = tmp->pid;
	r0[0] = tmp->r0[0];
	r0[1] = tmp->r0[1];
	r0[2] = tmp->r0[2];
	r0[3] = tmp->r0[3];
	r1[0] = tmp->r1[0];
	r1[1] = tmp->r1[1];
	r1[2] = tmp->r1[2];
	r1[3] = tmp->r1[3];
	r2[0] = tmp->r2[0];
	r2[1] = tmp->r2[1];
	r2[2] = tmp->r2[2];
	r2[3] = tmp->r2[3];
	r3[0] = tmp->r3[0];
	r3[1] = tmp->r3[1];
	r3[2] = tmp->r3[2];
	r3[3] = tmp->r3[3];
	pc = tmp->pc;
	p0[0] = tmp->p0[0];
	p0[1] = tmp->p0[1];
	p1[0] = tmp->p1[0];
	p1[1] = tmp->p1[1];
	p2[0] = tmp->p2[0];
	p2[1] = tmp->p2[1];
	p3[0] = tmp->p3[0];
	p3[1] = tmp->p3[1];
	psw[0] = tmp->psw[0];
	psw[1] = tmp->psw[1];
	ir[0] = tmp->ir[0];
	ir[1] = tmp->ir[1];
	ir[2] = tmp->ir[2];
	ir[3] = tmp->ir[3];
	ir[4] = tmp->ir[4];
	ir[5] = tmp->ir[5];
	acc[0] = tmp->acc[0];
	acc[1] = tmp->acc[1];
	acc[2] = tmp->acc[2];
	acc[3] = tmp->acc[3];
	fin  = tmp->fin;
}

void intilizePCB(struct pcb *tmp){
	tmp->pid = 0;
	tmp->bar = 0;
	tmp->r0[0] = '0';
	tmp->r0[1] = '0';
	tmp->r0[2] = '0';
	tmp->r0[3] = '0';
	tmp->r1[0] = '0';
	tmp->r1[1] = '0';
	tmp->r1[2] = '0';
	tmp->r1[3] = '0';
	tmp->r2[0] = '0';
	tmp->r2[1] = '0';
	tmp->r2[2] = '0';
	tmp->r2[3] = '0';
	tmp->r3[0] = '0';
	tmp->r3[1] = '0';
	tmp->r3[2] = '0';
	tmp->r3[3] = '0';
	tmp->pc = 0;
	tmp->p0[0] = '0';
	tmp->p0[1] = '0';
	tmp->p1[0] = '0';
	tmp->p1[1] = '0';
	tmp->p2[0] = '0';
	tmp->p2[1] = '0';
	tmp->p3[0] = '0';
	tmp->p3[1] = '0';
	tmp->psw[0] = '0';
	tmp->psw[1] = '0';
	tmp->ir[0] = '0';
	tmp->ir[1] = '0';
	tmp->ir[2] = '0';
	tmp->ir[3] = '0';
	tmp->ir[4] = '0';
	tmp->ir[5] = '0';
	tmp->acc[0] = '0';
	tmp->acc[1] = '0';
	tmp->acc[2] = '0';
	tmp->acc[3] = '0';
	tmp->fin = 0;
}

int main(int argc, char *argv[]){
	system("clear");
	if (argc < 2){
		printf("Error! Need to specify how many instructions each program will get. \n(Ex. \"./test 5\")\n");
		return 1;
	}
	que->count = 1;
	char mem[1000][6] = {};
	char file[10];
	char input[10];
	int quit = 0;
	struct pcb *ptr, *tmp;
	int newer = 0;
	printf("Welcome! \nPlease enter one of the following commands \n1.load(Load a program into VM and execute it) \n2.mem(View what is stored in memory) \n3.quit(Shutdown the OS)\n");
	while (quit != 1){
		scanf(" %s", input);
		if ((input[0] == 'l') && (input[1] == 'o') && (input[2] == 'a') && (input[3] == 'd')){
			if (newer == 1){
				free(ptr);
				que->count = 1;
				newer--;
			}
			ptr = (struct pcb *)malloc(sizeof(struct pcb));
			ptr->next_pcb = NULL;
			intilizePCB(ptr);
			char line[9];
			int end = 0;
			int ear = 0;
			pid = 0;
			ptr->ic = ((int)*argv[1] - 48);
			tmp = ptr;
			while(end != 1){
				printf("Enter the file name (%d slot(s) left) (or type end to finish): ", 10 - pid);
				scanf(" %s", file);
				FILE *code = fopen(file, "r");
				if ((file[0] =='e') && (file[1] == 'n') && (file[2] == 'd' )){
					end = 1;
				}
				else if (code != NULL){
					if(pid > 0){
						tmp->next_pcb = (struct pcb *)malloc(sizeof(struct pcb));
						tmp->next_pcb->next_pcb = NULL;
						tmp = tmp->next_pcb;
						intilizePCB(tmp);
						tmp->pid = pid;
						tmp->ic = ((int)*argv[1] - 48);
					}
					ear = (100 * pid);
					while (fgets(line, 9, code)){
						for(int i = 0; i < 6; i++){
							mem[ear][i] = line[i];
						}
						ear++;
					}
					pid++;
					fclose(code);
					if (pid == 10){
						end = 1;
					}
				}
				else{
					printf("\nError! File not found \n");
				}
			}
			tmp = ptr;
			pc = 0;
			int halt = 0;
			int op;
			int comp = 0; int comp2 = pid;
			int counter = 0;
			pid = 0;
			while (comp != comp2){
				for(int i = 0; i < 6; i++){
					ir[i] = mem[pc][i];
				}
				op = (((int)mem[pc][0] - 48) * 10);
				op += ((int)mem[pc][1] - 48);
				switch(op){
					case 0: switch(((int)ir[3] - 48)){
							case 0: imm(&p0[0], &p0[1], ir); break;
							case 1: imm(&p1[0], &p1[1], ir); break;
							case 2: imm(&p2[0], &p2[1], ir); break;
							case 3: imm(&p3[0], &p3[1], ir); break;
						}
						pc++; break;
					case 1: switch(((int)ir[3] - 48)){
							case 0: add(&p0[0], &p0[1], ir, p0); break;
							case 1: add(&p1[0], &p1[1], ir, p1); break;
							case 2: add(&p2[0], &p2[1], ir, p2); break;
							case 3: add(&p3[0], &p3[1], ir, p3); break;
						}
						pc++; break;
					case 2: switch(((int)ir[3] - 48)){
							case 0: sub(&p0[0], &p0[1], ir, p0); break;
							case 1: sub(&p1[0], &p1[1], ir, p1); break;
							case 2: sub(&p2[0], &p2[1], ir, p2); break;
							case 3: sub(&p3[0], &p3[1], ir, p3); break;
						}
						pc++; break;
					case 3: loadacc(&acc[0], &acc[1], &acc[2], &acc[3], ir);
						pc++; break;
					case 4: switch(((int)ir[3] - 48)){
							case 0: loadacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
							case 1: loadacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
							case 2: loadacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
							case 3: loadacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 5: loadacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)]);
						pc++; break;
					case 6: switch(((int)ir[3] - 48)){
							case 0: mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][0] = '9'; mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][2], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][3], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][4], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][5], acc); break;
							case 1: mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][0] = '9'; mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][2], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][3], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][4], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][5], acc); break;
							case 2: mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][0] = '9'; mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][2], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][3], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][4], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][5], acc); break;
							case 3: mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][0] = '9'; mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][2], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][3], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][4], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][5], acc); break;
						}
						pc++; break;
					case 7: mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)][0] = '9'; mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)][2], &mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)][3], &mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)][4], &mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)][5], acc);
						pc++; break;
					case 8: switch(((int)ir[3] - 48)){
							case 0: switch(((int)ir[5] - 48)){
								case 0: mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][2], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][3], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][4], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][5], r0); break;
								case 1: mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][2], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][3], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][4], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][5], r0); break;
								case 2: mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][2], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][3], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][4], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][5], r0); break;
								case 3: mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][2], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][3], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][4], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][5], r0); break;
								}
								break;
							case 1: switch(((int)ir[5] - 48)){
								case 0: mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][2], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][3], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][4], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][5], r1); break;
								case 1: mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][2], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][3], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][4], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][5], r1); break;
								case 2: mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][2], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][3], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][4], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][5], r1); break;
								case 3: mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][2], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][3], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][4], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][5], r1); break;
								}
								break;
							case 2: switch(((int)ir[5] - 48)){
								case 0: mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][2], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][3], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][4], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][5], r2); break;
								case 1: mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][2], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][3], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][4], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][5], r2); break;
								case 2: mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][2], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][3], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][4], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][5], r2); break;
								case 3: mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][2], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][3], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][4], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][5], r2); break;
								}
								break;
							case 3: switch(((int)ir[5] - 48)){
								case 0: mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][2], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][3], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][4], &mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)][5], r3); break;
								case 1: mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][2], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][3], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][4], &mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)][5], r3); break;
								case 2: mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][2], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][3], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][4], &mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)][5], r3); break;
								case 3: mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][0]= '9'; mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][2], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][3], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][4], &mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)][5], r3); break;
								}
								break;
						}
						pc++; break;
					case 9: switch(((int)ir[3] - 48)){
							case 0: mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][0]= '9'; mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][2], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][3], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][4], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][5], r0); break;
							case 1: mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][0]= '9'; mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][2], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][3], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][4], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][5], r1); break;
							case 2: mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][0]= '9'; mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][2], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][3], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][4], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][5], r2); break;
							case 3: mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][0]= '9'; mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][1] = '9'; loadmem(&mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][2], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][3], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][4], &mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)][5], r3); break;
							}
						pc++; break;
					case 10: switch(((int)ir[3] - 48)){
							case 0: switch(((int)ir[5] - 48)){
								case 0: loadacc(&r0[0], &r0[1], &r0[2], &r0[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
								case 1: loadacc(&r0[0], &r0[1], &r0[2], &r0[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
								case 2: loadacc(&r0[0], &r0[1], &r0[2], &r0[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
								case 3: loadacc(&r0[0], &r0[1], &r0[2], &r0[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
								}
								break;
							case 1: switch(((int)ir[5] - 48)){
								case 0: loadacc(&r1[0], &r1[1], &r1[2], &r1[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
								case 1: loadacc(&r1[0], &r1[1], &r1[2], &r1[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
								case 2: loadacc(&r1[0], &r1[1], &r1[2], &r1[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
								case 3: loadacc(&r1[0], &r1[1], &r1[2], &r1[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
								}
								break;
							case 2: switch(((int)ir[5] - 48)){
								case 0: loadacc(&r2[0], &r2[1], &r2[2], &r2[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
								case 1: loadacc(&r2[0], &r2[1], &r2[2], &r2[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
								case 2: loadacc(&r2[0], &r2[1], &r2[2], &r2[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
								case 3: loadacc(&r2[0], &r2[1], &r2[2], &r2[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
								}
								break;
							case 3: switch(((int)ir[5] - 48)){
								case 0: loadacc(&r3[0], &r3[1], &r3[2], &r3[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
								case 1: loadacc(&r3[0], &r3[1], &r3[2], &r3[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
								case 2: loadacc(&r3[0], &r3[1], &r3[2], &r3[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
								case 3: loadacc(&r3[0], &r3[1], &r3[2], &r3[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
								}
								break;
						}
						pc++; break;
					case 11: switch(((int)ir[3] - 48)){
							case 0: loadacc(&r0[0], &r0[1], &r0[2], &r0[3], mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)]); break;
							case 1: loadacc(&r1[0], &r1[1], &r1[2], &r1[3], mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)]); break;
							case 2: loadacc(&r2[0], &r2[1], &r2[2], &r2[3], mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)]); break;
							case 3: loadacc(&r3[0], &r3[1], &r3[2], &r3[3], mem[((((int)ir[4] - 48) * 10) + ((int)ir[5] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 12: loadacc(&r0[0], &r0[1], &r0[2], &r0[3], ir);
						pc++; break;
					case 13: switch(((int)ir[3] - 48)){
							case 0: switch(((int)ir[5] - 48)){
								case 0: loadmem(&r0[0], &r0[1], &r0[2], &r0[3], r0); break;
								case 1: loadmem(&r0[0], &r0[1], &r0[2], &r0[3], r1); break;
								case 2: loadmem(&r0[0], &r0[1], &r0[2], &r0[3], r2); break;
								case 3: loadmem(&r0[0], &r0[1], &r0[2], &r0[3], r3); break;
								}
								break;
							case 1: switch(((int)ir[5] - 48)){
								case 0: loadmem(&r1[0], &r1[1], &r1[2], &r1[3], r0); break;
								case 1: loadmem(&r1[0], &r1[1], &r1[2], &r1[3], r1); break;
								case 2: loadmem(&r1[0], &r1[1], &r1[2], &r1[3], r2); break;
								case 3: loadmem(&r1[0], &r1[1], &r1[2], &r1[3], r3); break;
								}
								break;
							case 2: switch(((int)ir[5] - 48)){
								case 0: loadmem(&r2[0], &r2[1], &r2[2], &r2[3], r0); break;
								case 1: loadmem(&r2[0], &r2[1], &r2[2], &r2[3], r1); break;
								case 2: loadmem(&r2[0], &r2[1], &r2[2], &r2[3], r2); break;
								case 3: loadmem(&r2[0], &r2[1], &r2[2], &r2[3], r3); break;
								}
								break;
							case 3: switch(((int)ir[5] - 48)){
								case 0: loadmem(&r3[0], &r3[1], &r3[2], &r3[3], r0); break;
								case 1: loadmem(&r3[0], &r3[1], &r3[2], &r3[3], r1); break;
								case 2: loadmem(&r3[0], &r3[1], &r3[2], &r3[3], r2); break;
								case 3: loadmem(&r3[0], &r3[1], &r3[2], &r3[3], r3); break;
								}
								break;
						}
						pc++; break;
					case 14: switch(((int)ir[3] - 48)){
							case 0: loadmem(&acc[0], &acc[1], &acc[2], &acc[3], r0); break;
							case 1: loadmem(&acc[0], &acc[1], &acc[2], &acc[3], r1); break;
							case 2: loadmem(&acc[0], &acc[1], &acc[2], &acc[3], r2); break;
							case 3: loadmem(&acc[0], &acc[1], &acc[2], &acc[3], r3); break;
						}
						pc++; break;
					case 15: switch(((int)ir[3] - 48)){
							case 0: loadmem(&r0[0], &r0[1], &r0[2], &r0[3], acc); break;
							case 1: loadmem(&r1[0], &r1[1], &r1[2], &r1[3], acc); break;
							case 2: loadmem(&r2[0], &r2[1], &r2[2], &r2[3], acc); break;
							case 3: loadmem(&r3[0], &r3[1], &r3[2], &r3[3], acc); break;
						}
						pc++; break;
					case 16: addacc(&acc[0], &acc[1], &acc[2], &acc[3], ir);
						pc++; break;
					case 17: subacc(&acc[0], &acc[1], &acc[2], &acc[3], ir);
						pc++; break;
					case 18: switch(((int)ir[3] - 48)){
							case 0: addrn(&acc[0], &acc[1], &acc[2], &acc[3], r0); break;
							case 1: addrn(&acc[0], &acc[1], &acc[2], &acc[3], r1); break;
							case 2: addrn(&acc[0], &acc[1], &acc[2], &acc[3], r2); break;
							case 3: addrn(&acc[0], &acc[1], &acc[2], &acc[3], r3); break;
						}
						pc++; break;
					case 19: switch(((int)ir[3] - 48)){
							case 0: subrn(&acc[0], &acc[1], &acc[2], &acc[3], r0); break;
							case 1: subrn(&acc[0], &acc[1], &acc[2], &acc[3], r1); break;
							case 2: subrn(&acc[0], &acc[1], &acc[2], &acc[3], r2); break;
							case 3: subrn(&acc[0], &acc[1], &acc[2], &acc[3], r3); break;
						}
						pc++; break;
					case 20: switch(((int)ir[3] - 48)){
							case 0: addacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
							case 1: addacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
							case 2: addacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
							case 3: addacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 21: addacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid)]);
						pc++; break;
					case 22: switch(((int)ir[3] - 48)){
							case 0: subacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
							case 1: subacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
							case 2: subacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
							case 3: subacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 23:  subacc(&acc[0], &acc[1], &acc[2], &acc[3], mem[((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48) + (100 * pid))]);
						pc++; break;
					case 24: switch(((int)ir[3] - 48)){
							case 0: psw[0] = equal(acc, mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
							case 1: psw[0] = equal(acc, mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
							case 2: psw[0] = equal(acc, mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
							case 3: psw[0] = equal(acc, mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 25: switch(((int)ir[3] - 48)){
							case 0: psw[0] = less(acc, mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
							case 1: psw[0] = less(acc, mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
							case 2: psw[0] = less(acc, mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
							case 3: psw[0] = less(acc, mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 26: switch(((int)ir[3] - 48)){
							case 0: psw[0] = greater(acc, mem[((((int)p0[0] - 48) * 10) + ((int)p0[1] - 48)) + (100 * pid)]); break;
							case 1: psw[0] = greater(acc, mem[((((int)p1[0] - 48) * 10) + ((int)p1[1] - 48)) + (100 * pid)]); break;
							case 2: psw[0] = greater(acc, mem[((((int)p2[0] - 48) * 10) + ((int)p2[1] - 48)) + (100 * pid)]); break;
							case 3: psw[0] = greater(acc, mem[((((int)p3[0] - 48) * 10) + ((int)p3[1] - 48)) + (100 * pid)]); break;
						}
						pc++; break;
					case 27: switch(((int)ir[3] - 48)){
							case 0: psw[0] = greater(acc, ir); break;
							case 1: psw[0] = greater(acc, ir); break;
							case 2: psw[0] = greater(acc, ir); break;
							case 3: psw[0] = greater(acc, ir); break;
						}
						pc++; break;
					case 28: switch(((int)ir[3] - 48)){
							case 0: psw[0] = equal(acc, ir); break;
							case 1: psw[0] = equal(acc, ir); break;
							case 2: psw[0] = equal(acc, ir); break;
							case 3: psw[0] = equal(acc, ir); break;
						}
						pc++; break;
					case 29: switch(((int)ir[3] - 48)){
							case 0: psw[0] = less(acc, ir); break;
							case 1: psw[0] = less(acc, ir); break;
							case 2: psw[0] = less(acc, ir); break;
							case 3: psw[0] = less(acc, ir); break;
						}
						pc++; break;
					case 30: switch(((int)ir[3] - 48)){
							case 0: psw[0] = equalrn(acc, r0); break;
							case 1: psw[0] = equalrn(acc, r1); break;
							case 2: psw[0] = equalrn(acc, r2); break;
							case 3: psw[0] = equalrn(acc, r3); break;
						}
						pc++; break;
					case 31: switch(((int)ir[3] - 48)){
							case 0: psw[0] = lessrn(acc, r0); break;
							case 1: psw[0] = lessrn(acc, r1); break;
							case 2: psw[0] = lessrn(acc, r2); break;
							case 3: psw[0] = lessrn(acc, r3); break;
						}
						pc++; break;
					case 32: switch(((int)ir[3] - 48)){
							case 0: psw[0] = greaterrn(acc, r0); break;
							case 1: psw[0] = greaterrn(acc, r1); break;
							case 2: psw[0] = greaterrn(acc, r2); break;
							case 3: psw[0] = greaterrn(acc, r3); break;
						}
						pc++; break;
					case 33: if (psw[0] == 'T'){
							pc = (((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid));
							break;
						}
						else{
							pc++; break;
						}
					case 34: if (psw[0] == 'F'){
							pc = (((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)) + (100 * pid));
							break;
						}
						else{
							pc++; break;
						}
					case 35: pc = ((((int)ir[2] - 48) * 10) + ((int)ir[3] - 48)); break;
					case 36: switch(((int)ir[3] - 48)){
							case 0: switch(((int)ir[5] - 48)){
									case 0: if (r0[3] == '0'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r0[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r0[3] == '1'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r0[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r0[3] =='2'){
											r0[2] = (char)((tmp->pid % 10) + 48);
											r0[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 1: if (r0[3] == '0'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r1[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r0[3] == '1'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r1[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r0[3] =='2'){
											r1[2] = (char)((tmp->pid % 10) + 48);
											r1[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 2: if (r0[3] == '0'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r2[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r0[3] == '1'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r2[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r0[3] =='2'){
											r2[2] = (char)((tmp->pid % 10) + 48);
											r2[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 3: if (r0[3] == '0'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r3[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r0[3] == '1'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r3[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r0[3] =='2'){
											r3[2] = (char)((tmp->pid % 10) + 48);
											r3[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
								} break;
							case 1: switch(((int)ir[5] - 48)){
									case 0: if (r1[3] == '0'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r0[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r1[3] == '1'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r0[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r1[3] =='2'){
											r0[2] = (char)((tmp->pid % 10) + 48);
											r0[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 1: if (r1[3] == '0'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r1[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r1[3] == '1'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r1[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r1[3] =='2'){
											r1[2] = (char)((tmp->pid % 10) + 48);
											r1[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 2: if (r1[3] == '0'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r2[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r1[3] == '1'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r2[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r1[3] =='2'){
											r2[2] = (char)((tmp->pid % 10) + 48);
											r2[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 3: if (r1[3] == '0'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r3[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r1[3] == '1'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r3[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r1[3] =='2'){
											r3[2] = (char)((tmp->pid % 10) + 48);
											r3[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
								} break;
							case 2: switch(((int)ir[5] - 48)){
									case 0: if (r2[3] == '0'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r0[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r2[3] == '1'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r0[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r2[3] =='2'){
											r0[2] = (char)((tmp->pid % 10) + 48);
											r0[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 1: if (r2[3] == '0'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r1[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r2[3] == '1'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r1[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r2[3] =='2'){
											r1[2] = (char)((tmp->pid % 10) + 48);
											r1[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 2: if (r2[3] == '0'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r2[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r2[3] == '1'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r2[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r2[3] =='2'){
											r2[2] = (char)((tmp->pid % 10) + 48);
											r2[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 3: if (r2[3] == '0'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r3[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r2[3] == '1'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r3[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r2[3] =='2'){
											r3[2] = (char)((tmp->pid % 10) + 48);
											r3[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
								} break;
							case 3: switch(((int)ir[5] - 48)){
									case 0: if (r3[3] == '0'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r0[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r3[3] == '1'){
											if ((r0[3] >= '0') || (r0[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r0[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r3[3] =='2'){
											r0[2] = (char)((tmp->pid % 10) + 48);
											r0[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 1: if (r3[3] == '0'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r1[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r3[3] == '1'){
											if ((r1[3] >= '0') || (r1[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r1[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r3[3] =='2'){
											r1[2] = (char)((tmp->pid % 10) + 48);
											r1[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 2: if (r3[3] == '0'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r2[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r3[3] == '1'){
											if ((r2[3] >= '0') || (r2[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r2[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r3[3] =='2'){
											r2[2] = (char)((tmp->pid % 10) + 48);
											r2[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
									case 3: if (r3[3] == '0'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 1;
													pc++; break;
												}
											}
											else if (r3[3] == '4'){
												if (que->count < 1){
													break;
												}
												else{
													que->count = que->count - 4;
													pc++; break;
												}
											}
										}
										else if (r3[3] == '1'){
											if ((r3[3] >= '0') || (r3[3] <= '3')){
												que->count = que->count + 1;
												pc++; break;
											}
											else if (r3[3] == '4'){
												que->count = que->count + 4;
												pc++; break;
											}
										}
										else if (r3[3] =='2'){
											r3[2] = (char)((tmp->pid % 10) + 48);
											r3[3] = (char)(tmp->pid + 48);
											pc++;
										}
										break;
								} break;
						 }
						 break;
					case 37: switch(((int)ir[3] - 48)){
							case 0: switch(((int)ir[5] - 48)){
								case 0: mod(r0, r0, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 1: mod(r0, r1, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 2: mod(r0, r2, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 3: mod(r0, r3, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								} break;
							case 1: switch(((int)ir[5] - 48)){
								case 0: mod(r1, r0, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 1: mod(r1, r1, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 2: mod(r1, r2, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 3: mod(r1, r3, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								} break;
							case 2: switch(((int)ir[5] - 48)){
								case 0: mod(r2, r0, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 1: mod(r2, r1, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 2: mod(r2, r2, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 3: mod(r2, r3, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								} break;
							case 3: switch(((int)ir[5] - 48)){
								case 0: mod(r3, r0, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 1: mod(r3, r1, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 2: mod(r3, r2, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								case 3: mod(r3, r3, &acc[0], &acc[1], &acc[2], &acc[3]); break;
								} break;
						 }
						 pc++; break;
					case 38: pc++; break;
					case 99: if (tmp->fin == 1){
							if (tmp->next_pcb != NULL){
								tmp = tmp->next_pcb;
								loadPCB(tmp);
 								counter = -1;
								break;
							}
							else{
								tmp = ptr;
								loadPCB(tmp);
								counter = -1;
								break;
							}
						 }
						 else{
							savePCB(tmp);
							tmp->fin = 1;
							comp++;
							if (tmp->next_pcb != NULL){
								tmp = tmp->next_pcb;
								loadPCB(tmp);
								counter = -1;
								break;
							}
							else{
								tmp = ptr;
								loadPCB(tmp);
								counter = -1;
								break;
							}
					 	 }
					default: printf("Warning! \nopcode:%d is not recognized\n", op); pc++;
					}
				counter++;
				if (counter == tmp->ic){
					savePCB(tmp);
					counter = 0;
					tmp = tmp->next_pcb;
					if (tmp != NULL){
						loadPCB(tmp);
					}
					else{
						tmp = ptr;
						loadPCB(tmp);
					}
				}
			}
			newer++;
			printf("\nDone! \nPlease enter one of the following commands \n1.load(Load a program into VM and execute it) \n2.mem(View what is stored in memory) \n3.quit(Shutdown the OS)\n");
		}
		else if ((input[0] == 'm') && (input[1] == 'e') && (input[2] == 'm')){
			tmp = ptr;
			pid = 0;
			while(tmp != NULL){
				for(int j = 0; j < 100; j++){
					printf("#%d.", (pid*100) + j);
					for(int k = 0; k < 6; k++){
						printf("%c", mem[(pid*100) + j][k]);
					}
					printf("\n");
				}
				pid++;
				tmp = tmp->next_pcb;
			}
			printf("\n\nPlease enter one of the following commands \n1.load(Load a program int VM and execute it) \n2.mem(View what is stored in memory) \n3.quit(Shutdown the OS)\n");
		}
		else if ((input[0] == 'q') && (input[1] == 'u') && (input[2] == 'i') && (input[3] == 't' )){
			quit = 1;
			system("clear");
		}
		else{
			printf("\nError! Please use one of the following comands \n1.load(Load a program into VM and execute it) \n2.mem(View what is stored in memory) \n3.quit(Shutdown the OS)\n");
		}
	}
	free(ptr);
	return 0;
}
