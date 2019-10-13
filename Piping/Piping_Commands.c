// Created by Christian Pizarro on 9/21/19.
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int pipe1[2];                                               //array of size 2
    char *args_1[] = {"ls", "-F", NULL};                        //this will write ls -F
    char *args_2[] = {"nl", NULL};                              //this will provide the nl command to be written
    int child1 = 1;                                             //initialized children variables
    int child2 = 1;
    pipe(pipe1);                                                //creation of the pipe using array, 0 for read, 1 for write.
    printf("(parent id:%d)\n", (int) getpid());                 // shows parent pid
    child1 = fork();                                            //creates the first child
    if (child1 > 0)                                             //conditional ensures that only the parent forks again
    {                                           
        child2 = fork();                                        
        if (child2 > 0)                                         //conditional makes sure only the parent shows children pid's
        {                                       
            printf("(child1 pid:%d)\n", child1);
            printf("(child2 pid:%d)\n", child2);
                                                                
        }
    }
    if (child1 < 0 || child2 < 0)                               //if fork fails it returns -1
    {
        fprintf(stderr, "Pipe Failed" );                        //outputs error message
        return 1;
    } 
    else if (child1 == 0 && child2 > 0)                         //conditional meant for child1
    {
                                                                
        close(pipe1[1]);                                        //close writing end
        dup2(pipe1[0], 0);                                      //reads the information in the pipe aka output of child2
        execvp(args_2[0], args_2);                              //uses nl as the command, and the rest of the array
                                                                //will be added with a space in between. Since there is nothing else
                                                                //it will only use nl
    } 
    else if (child1 > 0 && child2 == 0)                         //conditional meant for child2
    {
        close(pipe1[0]);                                        //close reading end
        dup2(pipe1[1], 1);                                      //writes the information of the rest of this conditional
        execvp(args_1[0], args_1);                              //uses ls as the command, adds -F creating ls -F
    } 
    else                                                        //conditional meant for parent
    {                                                         
        int c1 = wait(NULL);                                    //waits for process to finish which is child2
        close(pipe1[1]);                                        //closes the writing end of pipe which is stopping child1 from finishing
        int c2 = wait(NULL);                                    //waits for another process to finish which is child1
        close(pipe1[0]);                                        //closes reading end of pipe
        printf("parent waited for pid's:%d and %d\n", c1, c2);  //prints the two finished processes
    }
    return 0;
}
