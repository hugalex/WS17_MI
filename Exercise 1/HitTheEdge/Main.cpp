#include "HitTheEdge.h"
#include "mouse\include\ssimouse.h"
#include "ssi.h"
using namespace ssi;

void game();

int main () {

	Factory::RegisterDLL ("ssiframe.dll");
	Factory::RegisterDLL("ssigraphic.dll");
	Factory::RegisterDLL("ssiioput.dll");
	Factory::RegisterDLL ("ssimouse.dll");
	Factory::RegisterDLL("ssisignal.dll");

	Factory::Register (HitTheEdge::GetCreateName (), HitTheEdge::Create);

	ssi_print ("%s\n\nbuild version: %s\n\n", SSI_COPYRIGHT, SSI_VERSION);
	
	game ();

	ssi_print("\n\n\tpress a key to quit\n");
	getchar();

	Factory::Clear ();
	
	return 0;
}

void game() {

	ITheFramework *frame = Factory::GetFramework ();

	Mouse *mouse = ssi_create (Mouse, 0, true);
	mouse->getOptions ()->scale = true;
	mouse->getOptions ()->flip = false;
	mouse->getOptions ()->sr = 30.0;
	ITransformable *cursor_p = frame->AddProvider (mouse, SSI_MOUSE_CURSOR_PROVIDER_NAME);
	frame->AddSensor (mouse); 

	HitTheEdge *game = ssi_create (HitTheEdge, 0, true);
	frame->AddConsumer(cursor_p, game, "8.0s");

	frame->Start ();
	ssi_print ("Press enter to stop!\n");
	getchar ();
	frame->Stop ();
	frame->Clear ();
}
