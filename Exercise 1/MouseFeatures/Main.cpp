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
	ITransformable *cursor_p = frame->AddProvider (mouse, SSI_MOUSE_CURSOR_PROVIDER_NAME);
	frame->AddSensor (mouse); 

	MouseFeatures *feat = ssi_create (MouseFeatures, "features", true);
	//set options
	feat->getOptions()->distance = false;

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
