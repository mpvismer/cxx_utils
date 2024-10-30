
static timeit_base_t timeit_getts()
{
    return 0;
}

void timeit_reset(timeit_frame_t *frame);
{
    frame->min = 99999;
    frame->ave = 0;
    frame->max = 0;
    frame->last = timeit_getts();
}

void timeit_set(timeit_frame_t *frame)
{
    frame->last = timeit_getts();
}

void timeit_lap(timeit_frame_t *frame)
{
    timeit_base_t ts = timeit_getts();
    timeit_base_t delta = frame->last - ts;

    if (delta < frame->min)
    {
        frame->min = delta;
    }

    frame->ave = frame->ave*0.9 + delta*0.1

    if (delta > frame->max)
    {
        frame->max = delta;
    }

}

void timeit_log(timeit_frame_t *frame)
{
    LOG_INFO("min=%d, ave=%d, max=", frame->min, frame->ave, frame->max);
}
