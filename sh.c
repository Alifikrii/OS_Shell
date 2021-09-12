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

int main()
{
    char cmd[80];                           //  string perintah
    char *args[20];                         //  argumen string perintah
    char cwd[PATH_MAX];                     //string untuk working directories

    while (1)
    {

        // untuk mendapat direktori sekarang
        getcwd(cwd, sizeof(cwd));

        //  cetak prompt "$ " 
        printf("kelompok_SO_Pertemuan4:~%s$ ",cwd);

        //  baca string perintah
        fgets(cmd, sizeof cmd, stdin);

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
            val = execlp(args[0], *args, NULL);
            
            //      cetak pesan error
            //      keluar dengan kode -1

            if (val == -1) {
				puts("command not found");
			}
        }
              
    }

    return 0;
}
