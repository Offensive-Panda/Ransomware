#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <dirent.h>

#define DIG 0


	int fcrypt(FILE *in, char *str, char *key);
	int fdcrpt(FILE *in, char *str, char *key, int dig);
	void new_file(char *str1, char *str2);
	void strok(char *str);


int main()
{
	FILE *in;
	DIR *dir;
	struct dirent *lsdir;
	char file[MAX_PATH];
	char src[MAX_PATH];

	char key[]={"Vª╣Ù#A*~#═¸Ù#3¢kéî¤F¾°sú▀wtèklsy±?Çd¤■7┴iß'üÆÍE©æ¬Ë╚░╝Øz?fFò"};

		GetModuleFileName(NULL, src, MAX_PATH);
		strok(src);

		dir = opendir(src);

		while((lsdir=readdir(dir)) != NULL){

			strcpy(file, lsdir->d_name);

			in = fopen(file, "rb");
				if(in == NULL){goto def;}

				if(strstr(file, "[encrypt]") != NULL){
					fdcrpt(in, file, key, 0);
					goto def;
				}

			fcrypt(in, file, key);

			def: printf("");

				fclose(in);
				remove(file);
		}

		closedir(dir);

	return 0;
}


int fcrypt(FILE *in, char *str, char *key){

	int data, hash, i = DIG; 

	char strx[]={"[encrypt]"}; strcat(strx, str);

	FILE *out = fopen(strx, "wb");

		while((data = getc(in)) != EOF){
			hash = ((data ^ key[i]) + sizeof(int));
			putc(hash, out);
			i++;
			if (i >= strlen(key)){i = DIG;}
		}

	fclose(out);

	printf("$: %s\t\tcodificado\n", str);
return 0;
}

int fdcrpt(FILE *in, char *str, char *key, int dig){

	int data, hash, i = dig; 
	char strx[MAX_PATH];

		new_file(str, strx);

	FILE *out = fopen(strx, "wb");

		while((data = getc(in)) != EOF){
			hash = ((data - sizeof(int)) ^ key[i]);
			putc(hash, out);
			i++;
			if (i >= strlen(key)){i = dig;}
		}

	fclose(out);

	printf("$: %s\t\tdecodificado\n", str);
return 0;
}

void strok(char *str){
	int i = 0;
		while(str[i] != '\0'){i++;}
		while(str[i] != '\\'){i--;} i++;
		str[i] = '\0';
}

void new_file(char *str1, char *str2){

	int j = 0, i = 9;
		while(str1[i]!='\0'){
			str2[j] = str1[i];
			i++; j++;
		}

	str2[j] = '\0';
}
