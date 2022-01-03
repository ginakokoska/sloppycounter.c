/* sloppycounter.c
 * Autor: Gina Kokoska
 * Date:  12.12.21
 * /



#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <assert.h>
#include <sys/sysinfo.h>

#define NUMCPUS 6 


struct timespec start, end, result;
struct timespec loop_s, loop_e, loop_r;
struct timespec thread_s, thread_e, thread_r;

int loops = 10000;
int threshold[10] = {2, 4, 8, 16, 32, 64, 128, 265, 512, 1024};

/*define sloppy counter*/
typedef struct __counter_t {
        int             global;
        pthread_mutex_t glock;
        int             local[NUMCPUS];
        pthread_mutex_t llock[NUMCPUS];
        int             threshold;
} counter_t;


/*function to initialize gc*/
void init(counter_t *c, int threshold) {
        c->threshold = threshold;
        c->global = 0;
        pthread_mutex_init(&c->glock, NULL);
        for (int i = 0; i < NUMCPUS; i++) {
                c->local[i] = 0;
                pthread_mutex_init(&c->llock[i], NULL);
        }
}

/*function to update global counter value dependent on threshold s
 * large s = less OVH but less accurate val
 * scmall s = large OVH but more accurate val*/

void *update(counter_t *c, pid_t threadID, int amt) {
        int cpu = threadID % NUMCPUS;
        pthread_mutex_lock(&c->llock[cpu]);
        c->local[cpu] += amt;
        if (c->local[cpu] >= c->threshold) {
                pthread_mutex_lock(&c->glock);
                c->global += c->local[cpu];
                pthread_mutex_unlock(&c->glock);
                c->local[cpu] = 0;
        }
                                                                                                                                                                                          1,1           Top
        pthread_mutex_unlock(&c->llock[cpu]);
}


/*function to return global counter value*/
int get(counter_t *c) {
        pthread_mutex_lock(&c->glock);
        int val = c->global;
        pthread_mutex_unlock(&c->glock);
        return val;
}




/*function pointer for threads to increment counter */
void *mythread_i(void *arg) {
        for (int i = 0; i < loops; i++) {
                update(arg, pthread_self(), 1);
        }
        return NULL;
}



/*function pointer for threadmeasurement*/
void *donothing(void *arg) {
        return NULL;
}



/*function to calculate elapsed time*/
typedef uint64_t(*getlooptime)(struct timespec, struct timespec, struct timespec);

uint64_t loopfunc(struct timespec loop_s, struct timespec loop_e, struct timespec loop_r) {
        clock_gettime(CLOCK_MONOTONIC, &loop_s);
        for (int i = 0; i < loops; i++) {};
        clock_gettime(CLOCK_MONOTONIC, &loop_e);
        timeconvert loop_c = &convert;
        return(loop_c(loop_s, loop_e, loop_r));
        }


/*function to measure time for thread creation&join call*/
typedef uint64_t(*getthreadtime)(struct timespec, struct timespec, struct timespec);

uint64_t threadfunc(struct timespec thread_s, struct timespec thread_e, struct timespec thread_r) {
        pthread_t n;
        clock_gettime(CLOCK_MONOTONIC, &thread_s);
        int rn = pthread_create(&n, NULL, donothing, NULL);
        pthread_join(n, NULL);
        clock_gettime(CLOCK_MONOTONIC, &thread_e);
        timeconvert thread_c = &convert;
        return(thread_c(thread_s, thread_e, thread_r));
}
int main(int argc, char **argv) {
        int treshold[10] = {2, 4, 8, 16, 32, 64, 128, 265, 512, 1024};
        counter_t c[10];
        int rc[10];
        pthread_t  p[10];
        timeconvert t_c[10];
        getlooptime l_c[10];
        getthreadtime th_c[10];
        uint64_t t[10], rawloop[10], rawthread[10];
        struct timespec start[10], end[10], result[10];
        struct timespec loop_s[10], loop_e[10], loop_r[10];
        struct timespec thread_s[10], thread_e[10], thread_r[10];

        /*initialize local counters with different thresholds */
        for (int i = 0; i < 10; i++) {
                init(&c[i], threshold[i]);
        }


        /*create number of threads, each accessing its local counter and updating the gc depend on threshold value*/
        for (int i = 0; i < 10; i++) {
                clock_gettime(CLOCK_MONOTONIC, &start[i]);
                rc[i] = pthread_create(&p[i], NULL, mythread_i, &c[i]);
                assert(rc[i] == 0);
        }

        /*join threads and measure time for each thread*/
        for (int i = 0; i < 10; i++) {
                pthread_join(p[i], NULL);
                clock_gettime(CLOCK_MONOTONIC, &end[i]);
                t_c[i] = &convert;
                t[i] = t_c[i](start[i], end[i], result[i]);
                l_c[i] = &loopfunc;
                rawloop[i] = l_c[i](loop_s[i], loop_e[i], loop_r[i]);
                th_c[i] = &threadfunc;
                rawthread[i] = th_c[i](thread_s[i], thread_e[i], thread_r[i]);
                if ((rawloop[i] + rawthread[i]) > t[i] )  {
                        t[i] = t[i];
                } else {
                        t[i] = t[i] -  rawloop[i] - rawthread[i];
                }

        }

        for (int i = 0; i < 10; i++) {
                printf("Value of global counter with threshold[%d]:\t\t %d\n", threshold[i], get(&c[i]));
                printf("Time measured in nano seconds to finish thread[%d]:\t %lu\n\n", i+1, t[i]);

        }

        return 0;

}




