// HOTPATH
// super fast log dispatch research proj 
// when you are gettings lots of logs in + need to act on them quickly
// hotpath + coldpath

// writing to memory and reading at the same time with no performance loss/more latency on the main thread
// cold-thread does error correction on duplicate addresses (ptr subtraction)

// articles
// 1) https://software.intel.com/en-us/forums/intel-moderncode-for-parallel-architectures/topic/755067
// 2) https://blogs.cisco.com/performance/process-and-memory-affinity-why-do-you-care
// 3) S/O post on setting process affinity
// 4) S/O post on setting thread affinity 

// is there a way to generalize this to "super-charge" any process? given a cpu has SMT (hyperthreading)

// 256kb cache per physical core (so 256kb shared between 2 threads, should be able pull simulataneously)

// break up buffer access into segments (cold thread is checking if segments are null)
// say 4 segments: check each segment for null end, if end is null, go to next segment 

// IMPORTS 
#include <stdio.h> 
#include <string.h> 
#include <mmap.h> 
#include <pthread.h>

// CONSTANTS
const int X86_CACHE_LINE_SIZE = 64;

// STRUCTS
typedef struct {
	char* buf[X86_CACHE_LINE_SIZE*X86_CACHE_LINE_SIZE];
} shared_buffer; 

typedef struct {
	char* buf[X86_CACHE_LINE_SIZE];	// TODO: what would I multiply this why?
	int buf_pos = 0;
	int sentinel_pos = 0;
	shared_buffer* sb = NULL;
	pthread_t tid_sentinel;
} hotpath_buffer;


// UTILS FUNCTIONS

inline void sanitize_buf(hotpath_buffer* hpb_ptr, int offset, int num_segs){
	memset(hpb_ptr+offset, NULL, sizeof(char*)*(X86_CACHE_LINE_SIZE/num_segs));
}

// INIT FUNCTIONS
hotpath_buffer init_hotpath_buffer(){
	struct hotpath_buffer hp_e; 
	sanitize_buf(hp_e, 0, 1);
	return hp_e;
}

void init_shared_buffer(hotpath_buffer* hp_b){
	//set mmap  
	shared_buffer* sb = mmap(NULL, sizeof(shared_buffer), 
		PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	hp_b->sb = sb;
}

void set_hotpath_proc_affinity(){
	// set process affinity to single cpu  
}

void set_hotpath_sentinel_affinity(){
	// set thread affinity to same core so cache is shared
}

void* hp_sentinel_loop(void* hp_buf_ptr){
	// check at X86_CACHE_LINE_SIZE - 1 if ptr is NULL
	// if null, skip, else write to shared memory segment 

	// BUFFER is a memory segment of length N
	// 4 states: 
	// 1) Buffer is fresh (all null)
	// 2) buffer is less than half full: (N/2 - 2) is !NULL and (N/2 - 1) is NULL
	// 3) buffer is more than half full: (N/2 - 1) is !NULL and (N - 1) is NULL
	// 4) buffer is full: (N - 1) is !NULL

	hotpath_buffer* hpb_ptr = hp_buf_ptr;

	for (int i = 0; i < X86_CACHE_LINE_SIZE; i++){
		char* buf_val = hpb_ptr->buf[i];
		if (buf_val){
			// push to numa-aware cache 
		}
		else {
			break;
		}
	}

	sanitize_buf(hpb_ptr);
}

// setup hotpath SMT thread 
void init_hotpath_sentinel_thread(hotpath_buffer* hp_b){
	// spawn thread off main 
	pthread_create(&hp_b->tid_sentinel, NULL, hp_sentinel_loop, (void*)hp_b);

	// set thread affinity to same core as hotpath (but different thread)

}

inline void fastbuf_push(){

}

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}