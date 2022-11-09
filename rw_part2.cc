//C++ program to solve Reader - Writer problem using pthreads library
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <semaphore.h>
#include <sys/syscall.h>

using namespace std;

int read_ = 0, write_ = 0;;
string share = "All work and no play makes Jack a dull boy."; //given string which is shared among readers and writers
sem_t rw_sem, cs_sem; ////accessible to both readers, writers; used for protecting critical sections of readers

void *reader(void *arg)
{
	pid_t tid = syscall(SYS_gettid);
	// While share string contains something - critical section
	while(share != "")
	{
		sem_wait(&cs_sem); // mutex
		if(share.size() > 0)
		{
		
			printf("\nreader %d is reading ... content : %s",tid%read_,share.c_str());
			fflush(stdout);
			sem_post(&rw_sem);
			sleep(1);
		}
		else
		{
			sem_post(&cs_sem);
		}		
	}
	printf("\nThere are still %d writers waiting on the semaphore...",write_ - read_);
	for(int i = 0; i < (write_ - read_); i++)
			{
				sem_post(&rw_sem);
			}
	printf("\nreader %d is exiting ...",tid%read_);
	return 0;
}

void *writer(void *arg)
{
	pid_t tid = syscall(SYS_gettid);
	//loop until share is not empty
	while(share != "")
	{
		sem_wait(&rw_sem);
		if(share.size() > 0)
		{	
			printf("\nwriter %d is writing ...",tid%write_);
		
			share.pop_back(); //poping back last character from string
			sem_post(&cs_sem);
		}
		sleep(1);
		//sleep for 1 second
	}
	sem_post(&rw_sem);
	printf("\nwriter %d is exiting ...",tid%write_);
	return 0;
}


int main(int argc, char* argv[])
{ 
	if(argc < 3 || argc > 3)
	{
		cout<<"Project part - 2\n";
		return 1;
	}
	// command line arguments
	int numReaders = stoi(argv[1]);
	int numWriters = stoi(argv[2]);
	read_ = numReaders;
	write_ = numWriters;
	int newReaders = numReaders;
	int newWriters = numWriters;
	bool tmp = true;
	printf("*** Reader-Writer Problem Simulation ***");
	printf("\nNumber of reader threads: %d",read_);
	printf("\nNumber of writer threads: %d",write_);
	// check return values, 
	if(sem_init(&rw_sem, 0, 1) < 0 || sem_init(&cs_sem, 0, 0) < 0)
	{
		cout<<"error";
		exit(EXIT_FAILURE);
	}
	pthread_t readerThreads[numReaders];
	pthread_t writerThreads[numWriters];

	for(int i = 0; i < numReaders; i++)
	{
		// check error
		if(pthread_create(&readerThreads[i], 0, reader, 0) != 0)
		{
			if(tmp)
			{
				newReaders = i;
				tmp = false;
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
		if(pthread_join(writerThreads[i],0) != 0)
		{
			cout<<"fail"<<endl;
			exit(EXIT_FAILURE);
		}
	}	
	
	for(int i = 0; i < newReaders; i++)
	{
		if(pthread_join(readerThreads[i],0) != 0)
		{
			cout<<"error"<<endl;
			exit(EXIT_FAILURE);
		}		
	}
	
	
	sem_destroy(&rw_sem); //cleanup process
	sem_destroy(&cs_sem);	
	printf("\nAll threads are done.\nResources cleaned up.\n");
    return 0;
}
