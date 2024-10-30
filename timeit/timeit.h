
typedef double timeit_base_t;

typedef struct {
    timeit_base_t ave;
	timeit_base_t min;
	timeit_base_t max;
	timeit_base_t ave;
} timeit_frame_t


void timeit_reset(timeit_frame_t *frame);

void timeit_set(timeit_frame_t *frame);

void timeit_lap(timeit_frame_t *frame);

void timeit_log(timeit_frame_t *frame);

