//C++ program to solve Reader - Writer problem using pthreads library
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <semaphore.h>
#include <sys/syscall.h>

using namespace std;
int readerCount = 0, write_ = 0, read_=0;
string share = "All work and no play makes Jack a dull boy."; //given string which is shared among readers and writers
sem_t rw_sem; //accessible to both readers, writers
sem_t cs_sem; //used for protecting critical sections of readers

/* Reader function which accepts one parameter and return int value*/
void *reader(void *arg)
{	
	pid_t tid = syscall(SYS_gettid);
	// enter the critical section
	while(share != "")
	{
		sem_wait(&cs_sem); // mutex
		readerCount++;
		printf("\nread_count increments to: %d.",readerCount);
		fflush(stdout);
		if(readerCount == 1)
			sem_wait(&rw_sem); //send wait signal
		sem_post(&cs_sem);
		
		if(share.size() > 0)
		printf("\nreader %d is reading ... content : %s",tid%read_,share.c_str());
		fflush(stdout);
		sem_wait(&cs_sem);
		readerCount--;
		printf("\nread_count decrements to: %d.",readerCount);
		fflush(stdout);
		if(readerCount == 0)
			sem_post(&rw_sem);
		sem_post(&cs_sem);
		sleep(1);
	}
	printf("\nreader %d is exiting ...",tid%read_);
	return 0;
}

/* Writer function which accepts one parameter and return int value*/
void *writer(void *param)
{
	pid_t tid = syscall(SYS_gettid);
	//loop until the string is empty: - (chopping the last character of the string)
	while(share != "")
	{
		//print out the shared string
		sem_wait(&rw_sem);
		if(share.size() > 0)
		{	
			
			printf("\nwriter %d is writing ...",tid%write_);
			fflush(stdout);
			share.pop_back(); //(chopping the last character of the string)
		}
		sem_post(&rw_sem);
		sleep(1);//sleep os for 1 sec
	}
	printf("\nwriter %d is exiting ...",tid%write_);
	return 0;
}

int main(int argc, char* argv[])
{ 
	if(argc < 3 || argc > 3)
	{
		cout<<"Project part - 1\n";
		return 1;
	}
	bool tmp = true;
	// command line arguments
	int numReaders = stoi(argv[1]);
	int numWriters = stoi(argv[2]);
	int newReaders = numReaders;
	int newWriters = numWriters;
	read_ = numReaders;
	write_ = numWriters;
	printf("*** Reader-Writer Problem Simulation ***");
	printf("\nNumber of reader threads: %d",numReaders);
	printf("\nNumber of writer threads: %d",numWriters);
	// check return values, 
	if(sem_init(&rw_sem, 0, 1) < 0 || sem_init(&cs_sem, 0, 1) < 0)
	{
		cout<<"Oh! Error occured!";
		exit(EXIT_FAILURE); //exit with failure signal
	}
	// create array for threads
	pthread_t readerThreads[numReaders];
	pthread_t writerThreads[numWriters];

	for(int i = 0; i < numReaders; i++)
	{
		// check if there is any error
		if(pthread_create(&readerThreads[i], 0, reader, 0) != 0)
		{
			if(tmp)
			{
				newReaders = i;
				tmp = false; //changing tmp
			}
		}			
	}
	tmp = true;
	for(int i = 0; i < numWriters; i++)
	{
		// check error	
		if(pthread_create(&writerThreads[i], 0, writer, 0) != 0)
		{
			if(tmp)
			{
				newWriters = i;
				tmp = false;
			}
		}			
	}
	
	for(int i = 0; i < newWriters; i++)
	{
		// check error
		if(pthread_join(writerThreads[i],0) != 0)
		{
			cout<<"error";
			exit(EXIT_FAILURE);
		}	
	}
	for(int i = 0; i < newReaders; i++)
	{
		// check error
		if(pthread_join(readerThreads[i],0) != 0)
		{
			cout<<"failure";
			exit(EXIT_FAILURE);
		}		
	}
	// clean the space
	sem_destroy(&rw_sem);
	sem_destroy(&cs_sem);
	printf("\nAll threads are done.\nResources cleaned up.\n");
	
    return 0;
}
