#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv){
  if(argc != 2){
    fprintf(stderr, "Proper usage: pinger hostname | IP address");
  }

  char* hostname = argv[1];
  char str_cmd[10 + strlen(hostname)];
  strcpy(str_cmd, "ping -c 1 ");
  strcat(str_cmd, hostname);
  //  char* output[1000];
  
  while(1){
    // Redirect system output to file
    freopen("output.txt", "w", stdout);
    system(str_cmd);

    // Put file's contents into buffer
    FILE *fp = fopen("output.txt", "rb");
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *data = malloc(fsize+1);
    fread(data, 1, fsize, fp);
    fclose(fp);
    data[fsize] = 0;
    
    // Extract desired data from buffer
    char* time = strstr(data, "time=");
    char* packet_loss = strstr(data, "received, ");
    //     fprintf(stderr, "%s\n", packet_loss);
    if(time == NULL && packet_loss == NULL){
      fprintf(stderr, "Cannot ping specified address\n");
      exit(1);
    }

    char latency[100];
    char pl[100];
    
    // Process data
    if(time != NULL){
      int i = 0;
      while(time[i+5] != '\n'){
	latency[i] = time[i+5];
	i++;
      }
      latency[i] = 0;
    }
    else{
      strcpy(latency, "N/A");
    }

    if(packet_loss != NULL){
      int i = 0;
      while(packet_loss[i+10] != '%'){
	pl[i] = packet_loss[i+10];
	i++;
      }
      pl[i] = 0;
    }
    else{
      strcpy(pl, "N/A");
    }
    
    // Reassign stdout and print data
    freopen("/dev/tty", "w", stdout);
    printf("Latency: %s\nPacket Loss: %s%%\n", latency, pl);
    sleep(1);
  }
}
