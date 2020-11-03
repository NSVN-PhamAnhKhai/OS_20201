#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t thread1, thread2;
pthread_mutex_t lock;

struct Student{
	char name[30];
	int id;
	float gpa;
	int toeicScores;
	int creditsPass;
	int fee;
};
struct Student student = {"Pham Anh Khai", 20172617, 3.15f, 420, 98, 12000000};
char mail[] = "khaiironman99@gmail.com";
struct Student stdTemp;
char mailMessage[500];
char updated[50];


void *updateInfo(void *arg){
	pthread_mutex_lock(&lock);
	printf("Thread %d lock\n", (int)pthread_self());

	printf("Update started:\n");
	printf("GPA: ");
	scanf("%f", &stdTemp.gpa);
	fflush(stdin);
	printf("Toeic scores: ");
	scanf("%d", &stdTemp.toeicScores);
	fflush(stdin);
	printf("Credits pass: ");
	scanf("%d", &stdTemp.creditsPass);
	fflush(stdin);
	printf("Fee: ");
	scanf("%d", &stdTemp.fee);
	fflush(stdin);

	if(student.gpa != stdTemp.gpa){
		student.gpa = stdTemp.gpa;
		strcat(updated, "GPA, ");
	}
	if(student.toeicScores != stdTemp.toeicScores){
		student.toeicScores = stdTemp.toeicScores;
		strcat(updated, ",Toeic scores, ");
	}
	if(student.creditsPass != stdTemp.creditsPass){
		student.creditsPass = stdTemp.creditsPass;
		strcat(updated, ",Credits pass, ");
	}
	if(student.fee != stdTemp.fee){
		student.fee = stdTemp.fee;
		strcat(updated, ",Fee, ");
	}

	printf("Thread %d unlock\n", (int)pthread_self());
	pthread_mutex_unlock(&lock);

	return NULL;
}

void *sendMail(void *arg){
	pthread_mutex_lock(&lock);
	printf("Thread %d lock\n", (int)pthread_self());

	char temp[50];

	//insert message
		strcpy(mailMessage, "The information fields have been updated: ");
		strcat(mailMessage, "\nYour information after being updated\n");
		sprintf(temp, "Your name: %s", student.name);
		strcat(mailMessage, temp);
		sprintf(temp, "\nID: %d", student.id);
		strcat(mailMessage, temp);
		sprintf(temp, "\nGPA: %.2f", (float)student.gpa);
		strcat(mailMessage, temp);
		sprintf(temp, "\nToeic scores: %d", student.toeicScores);
		strcat(mailMessage, temp);
		sprintf(temp, "\nCredits pass: %d", student.creditsPass);
		strcat(mailMessage, temp);
		sprintf(temp, "\nFee: %d \n", student.fee);
		strcat(mailMessage, temp);

		printf("\n");
		printf("The information fields have been updated: ");
		printf("\nYour information after being updated");
		printf("\nYour name: %s", student.name);
		printf("\nID: %d", student.id);
		printf("\nGPA: %.2f", (float)student.gpa);
		printf("\nToeic scores: %d", student.toeicScores);
		printf("\nCredits pass: %d", student.creditsPass);
		printf("\nFee: %d \n", student.fee);
		printf("\n");

	//send mail
	char cmd[100];  // to hold the command.
    char tempFile[100];     // name of tempfile.

    strcpy(tempFile,tempnam("/tmp","sendmail")); // generate temp file name.

    FILE *fp = fopen(tempFile,"w"); // open it for writing.
    fprintf(fp,"%s\n",mailMessage);        // write body to it.
    fclose(fp);             // close it.

    sprintf(cmd,"sendmail %s < %s",mail,tempFile); // prepare command.
    system(cmd);     // execute it.
    printf("Email sent successfully!!!\n");

	printf("Thread %d unlock\n", (int)pthread_self());
	pthread_mutex_unlock(&lock);

	return NULL;
}

int main(void)
{
	int err;
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    err = pthread_create(&thread1, NULL, &updateInfo, NULL);
    if (err != 0)
    {
        printf("\ncan't create thread1 :[%s]", strerror(err));
        exit(0);
    }
    err = pthread_create(&thread2, NULL, &sendMail, NULL);
    if (err != 0)
    {
        printf("\ncan't create thread1 :[%s]", strerror(err));
        exit(0);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_mutex_destroy(&lock);

    return 0;
}