#include "HitTheEdge.h"

HitTheEdge::HitTheEdge(const ssi_char_t *file)
{
	//todo
}

HitTheEdge::~HitTheEdge() 
{
	//todo
}

void HitTheEdge::consume_enter(ssi_size_t stream_in_num, ssi_stream_t stream_in[])
{
	//todo
}

void HitTheEdge::consume(IConsumer::info consume_info, 
	ssi_size_t stream_in_num, 
	ssi_stream_t stream_in[])
{
	float MAXVALUE = 0.999;
	float MINVALUE = 0.0001;
	// test for valid input
	if (stream_in[0].dim != 2)
		ssi_wrn("unexpected stream dimension");
	
	// get input
	float *in = ssi_pcast(float, stream_in[0].ptr);
	static int state = 0;
	int right = 0;
	int left = 0;
	int bottom = 0;
	int top = 0;

	for (int j = 0; j < stream_in[0].num; j++) {
		switch (state) {
		case 0:		// case: hit no edge 
			if (in[j*stream_in[0].dim] > MAXVALUE) {					// Hit right edge
				right++;
				state = 1;
			}
			else if (in[j*stream_in[0].dim] < MINVALUE) {				// Hit left edge
				left++;
				state = 1;
			}
			else if (in[j*stream_in[0].dim + 1] > MAXVALUE) {			// Hit bottom edge
				bottom++;
				state = 2;
			}
			else if (in[j*stream_in[0].dim + 1] < MINVALUE) {			// Hit top edge
				top++;
				state = 2;
			}
			break;

		case 1:		// case: hit right or left edge
			if (in[j*stream_in[0].dim] < MAXVALUE && in[j*stream_in[0].dim] > MINVALUE) {
				state = 0;
			}
			else if (in[j*stream_in[0].dim + 1] > MAXVALUE) {			// Hit bottom edge
				bottom++;
				state = 3;
			}
			else if (in[j*stream_in[0].dim + 1] < MINVALUE) {			// Hit top edge
				top++;
				state = 3;
			}
			break;

		case 2:		// case: hit bottom or top edge
			if (in[j*stream_in[0].dim + 1] < MAXVALUE && in[j*stream_in[0].dim + 1] > MINVALUE) {
				state = 0;
			}
			else if (in[j*stream_in[0].dim] > MAXVALUE) {				// Hit right edge
				right++;
				state = 3;
			}
			else if (in[j*stream_in[0].dim] < MINVALUE) {				// Hit left edge
				left++;
				state = 3;
			}
			break;

		case 3:		// case: hit two edges
			if (in[j*stream_in[0].dim + 1] < MAXVALUE && in[j*stream_in[0].dim + 1] > MINVALUE &&
				in[j*stream_in[0].dim] < MAXVALUE && in[j*stream_in[0].dim] > MINVALUE) {
				state = 0;
			}
			else if (in[j*stream_in[0].dim + 1] < MAXVALUE && in[j*stream_in[0].dim + 1] > MINVALUE &&
				in[j*stream_in[0].dim] > MAXVALUE || in[j*stream_in[0].dim] < MINVALUE) {
				state = 1;
			}
			else if (in[j*stream_in[0].dim + 1] > MAXVALUE || in[j*stream_in[0].dim + 1] < MINVALUE &&
				in[j*stream_in[0].dim] < MAXVALUE && in[j*stream_in[0].dim] > MINVALUE) {
				state = 2;
			}
			break;

		case 4:		//case for debugging
			ssi_print("X = %f \t Y = %f\n", in[j*stream_in[0].dim], in[j*stream_in[0].dim + 1]);
			break;
		}
	}

	ssi_print("right hit = %i \t left hit = %i \t top hit = %i \t bottom hit = %i\n", right, left, top, bottom);

}

void HitTheEdge::consume_flush(ssi_size_t stream_in_num, ssi_stream_t stream_in[])
{
	//todo
}

//todo ...

