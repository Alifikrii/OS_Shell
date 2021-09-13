/*
00ell
=====

Shell adalah program yang membuat proses child untuk menjalankan string perintah yang diberikan.
Shell berupa infinite looping yang berisi siklus fork--exec--wait.
Lengkapilah kode berikut ini untuk membuat program shell sederhana!

Contoh masukan dan keluaran
---------------------------

    $ date
    Tue Dec  9 13:34:17 WIB 2014
    $ whoami
    auriza
    $ exit
*/

// Kelompok 10
// Andi Muhammad Alifikri (G64190005)
// Sigit Raditya Pangestu (G64190071)
// Tifanee Anindhita (G64190080)
// Risda Awalia (G64190106)


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>

// pecah string per kata
void split_words(char *string, char **words)
{
    int i;
    words[0] = strtok(string, " \n");
    for (i = 0; words[i]; i++)
        words[i+1] = strtok(NULL, " \n");
}

//fungsi history
void get_history(int count, char **history)
{
    int i;
    for ( i = 0; i < count; i++)
    {
        printf("%d -> %s \n", i+1, history[i]);
    }
    
}

int main()
{
    char cmd[80];                           //  string perintah
    char *args[20];                         //  argumen string perintah
    char cwd[PATH_MAX];                     //string untuk working directories
    char *history[PATH_MAX];                //untuk menyimpan sbeanyak history terakhir 
    int count=0;
    char person[10];
    while (1)
    {
        
        //ini buat tambahan nama didepan shell
        FILE *fp = popen( "whoami", "r" );
        fgets(person, sizeof person, fp);
        if (person[strlen(person) - 1] == '\n')
			person[strlen(person) - 1] = '\0';

        //untuk mendapat history 
        if (!strcmp(cmd, "history"))
        {
            get_history(count, history);
        }

        // untuk mendapat direktori sekarang
        getcwd(cwd, sizeof(cwd));

        //  cetak prompt "$ " 
        printf("%s@KELOMPOK-10-SO:~%s \n$ ",person,cwd);

        //  baca string perintah
        fgets(cmd, sizeof cmd, stdin);

        //biar ngak segment fault core dumped
        if (cmd[0] == '\n')
			continue;
        else 
        {
            //untuk hitung dan store historynya
            history[count]=strdup(cmd);
            count++;
        }

        //  pecah string perintah per argumen
        split_words(cmd, args);

        
        //  jika perintah = "exit"
        //      break
        if (!strcmp(args[0], "exit"))
			break;


        //  jika perintah = "cd"
        if (!strcmp(args[0], "cd")) {
			int val; 
			if (args[1]) {
				char *dir = args[1];

                //      ganti direktori
				val = chdir(dir);
				if (val == -1) {
					puts("No such file or directory");
				}
			}
			else {
                chdir("/home");
			}

            //      continue
            continue;
		}

        //  buat proses child:
        pid_t pid = fork();

        int val;

        if(pid!=0)
        {
            wait(NULL);
            //  tunggu child selesai
        }
        else
        {
            //      exec string perintah
            val = execvp(args[0], args);
            
            //      cetak pesan error
            //      keluar dengan kode -1

            if (val == -1) {
				puts("command not found");
                return -1;

			}
        }
              
    }

    return 0;
}
