
#include "filter.h"


bool match::id(can_frame *frame, int id)
{
    return(frame->can_id == id);
}

int match::check(void)
{
        return 0;
}
