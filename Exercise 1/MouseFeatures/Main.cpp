#include "MouseFeatures.h"
#include "ssi.h"
#include "mouse\include\ssimouse.h"
using namespace ssi;

void mousefeat ();

int main () {

	Factory::RegisterDLL ("ssiframe.dll");
	Factory::RegisterDLL ("ssimouse.dll");
	Factory::RegisterDLL ("ssigraphic.dll");

	Factory::Register (MouseFeatures::GetCreateName (), MouseFeatures::Create);

	ssi_print ("%s\n\nbuild version: %s\n\n", SSI_COPYRIGHT, SSI_VERSION);
	
	mousefeat();

	Factory::Clear ();
	
	return 0;
}

void mousefeat() {

	ITheFramework *frame = Factory::GetFramework ();

	Decorator *decorator = ssi_create(Decorator, 0, true);
	frame->AddDecorator(decorator);
	
	Mouse *mouse = ssi_create (Mouse, "mouse", true);
	mouse->getOptions ()->sr = 50.0;
	mouse->getOptions()->scale = true;
	ITransformable *cursor_p = frame->AddProvider (mouse, SSI_MOUSE_CURSOR_PROVIDER_NAME);
	frame->AddSensor (mouse); 

	MouseFeatures *feat = ssi_create (MouseFeatures, "features", true);
	//set options
	feat->getOptions()->frame_min_x = true;
	feat->getOptions()->frame_max_x = true;
	feat->getOptions()->frame_average_x = true;
	feat->getOptions()->frame_min_y = true;
	feat->getOptions()->frame_max_y = true;
	feat->getOptions()->frame_average_y = true;
	feat->getOptions()->global_min_x = true;;
	feat->getOptions()->global_max_x = true;
	feat->getOptions()->global_average_x = true;
	feat->getOptions()->global_min_y = true;
	feat->getOptions()->global_max_y = true;
	feat->getOptions()->global_average_y = true;
	feat->getOptions()->distance = true;
	feat->getOptions()->velocity = true;
	feat->getOptions()->variability = true;
	feat->getOptions()->standard_deviation = true;

	ITransformable *feat_t = frame->AddTransformer(cursor_p, feat, "1.0s");
	
	SignalPainter *sigpaint = 0;

	sigpaint = ssi_create_id (SignalPainter, 0, "plot");
	sigpaint->getOptions ()->setTitle("raw");
	sigpaint->getOptions ()->size = 10.0;
	frame->AddConsumer (cursor_p, sigpaint, "0.5s");

	sigpaint = ssi_create_id(SignalPainter, 0, "plot");
	sigpaint->getOptions ()->setTitle ("features");
	sigpaint->getOptions ()->size = 10.0;
	frame->AddConsumer(feat_t, sigpaint, "0.5s");

	decorator->add("console", 0, 0, 650, 800);
	decorator->add("plot*", 650, 0, 400, 400);

	frame->Start ();
	ssi_print ("Press enter to stop!\n");
	getchar ();
	frame->Stop ();
	frame->Clear ();
}
