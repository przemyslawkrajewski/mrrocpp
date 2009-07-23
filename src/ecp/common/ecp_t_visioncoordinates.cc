/// \file ecp_t_visioncoordinates.cc
/// \brief definiuje zadanie podazania chwytaka (kamery) robota w strone obiektu zlokalizowanego na ekranie
/// \author Maciej Jerzy Nowak
/// \date 2009.07.22
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <unistd.h>
#include <sstream>

#include "lib/typedefs.h"
#include "lib/impconst.h"
#include "lib/com_buf.h"

#include "lib/srlib.h"

#include "ecp/irp6_on_track/ecp_local.h"
#include "ecp/irp6_postument/ecp_local.h"

#include "ecp_mp/ecp_mp_s_cvfradia.h"
#include "ecp/common/ecp_t_visioncoordinates.h"

namespace mrrocpp {
namespace ecp {
namespace common {
namespace task {

#define debugmsg(msg)		sr_ecp_msg->message(msg);
/////////////////////////////////////////////////////////////////////////////////////////////
// ecp_task_visioncoordinates
/////////////////////////////////////////////////////////////////////////////////////////////

visioncoordinates::visioncoordinates(lib::configurator& _config)
	: task(_config), SETTINGS_SECTION_NAME("[ecp_visioncoordinates_task]")
{
}

void visioncoordinates::task_initialization()
{
	// tworzymy robota
    if (strcmp(config.section_name, "[ecp_irp6_on_track]") == 0)
    {
        ecp_m_robot = new irp6ot::ecp_irp6_on_track_robot (*this);
        debugmsg("IRp6ot loaded");
    }
    else if (strcmp(config.section_name, "[ecp_irp6_postument]") == 0)
    {
        ecp_m_robot = new irp6p::ecp_irp6_postument_robot(*this);
        debugmsg("IRp6p loaded");
    }

	// i powiazane z nim generatory - czucia wizji i ruchu :)
	
	itsVisionGen = new generator::visioncoordinates(*this);
	itsSmoothGen = new generator::smooth(*this, true, false); // synchronized, debug
}

void visioncoordinates::main_task_algorithm()
{
	debugmsg("main_task_alogrithm");

	double bf[8];			// bufor na wspolrzedne prezkazywane do generatora smooth

	while (true)
	{
		// narazie testowo - wyb�r opcji "czego poszukujemy" - w programie.
		if (choose_option("Wybierz rodzaj wyszukiwanego obiektu:\n1 - wybrany w FraDIA\n0 - przerwij", 2) == 0)
			return;

		// aktualnie opieramy sie na wyborze poszukiwanego obiektu w ramach FraDIA (tam wybieramy obiekt z listy)
		setStartPosition();

	
		itsVisionGen->Move();		// wykonujemy ruch, polegajacy na rozpoznaniu okolicy :)
		
		bool found = false;
		while (itsVisionGen->getCoordinates(bf))
		{
			// --- po pobraniu wsp�rz�dnych wykonujemy ruch ---
			itsSmoothGen->load_coordinates(lib::XYZ_EULER_ZYZ, bf[0], bf[1], bf[2], bf[3], bf[4], bf[5], bf[6], bf[7]);
			itsSmoothGen->Move();
			itsSmoothGen->reset();

			// --- czekamy a� chwytak si� ustabilizuje ---
			sleep(1);

			// --- sprawdzamy, czy widzimy dany obiekt ---
			if (itsVisionGen->test())
			{
				found = true;
				break;		// obiekt zostal znaleziony, nie szukamy nastepnych obiektow
			}
		}

		if (found)	
			show_message("Znaleziono poszukiwany obiekt!\nZnajduje si� na wprost chwytaka robota.");
		else
			show_message("Poszukiwanego obiektu NIE znaleziono :-(");
	}

/*	double bf[8];			// bufor na wspolrzedne prezkazywane do generatora smooth
	
	int loop = 5;
	while (loop--)
	{
		debugmsg("itsVisionGen->Move()");
		itsVisionGen->Move();
		debugmsg("getNewCoordinates()");
		itsVisionGen->getNewCoordinates(bf);
		debugmsg("load_coordinates()");
		itsSmoothGen->load_coordinates(lib::XYZ_EULER_ZYZ, bf[0], bf[1], bf[2], bf[3], bf[4], bf[5], bf[6], bf[7]);
		debugmsg("itsSmoothGen->Move()");
		itsSmoothGen->Move();
		debugmsg("itsShoothGen->reset()");
		itsSmoothGen->reset();
	
	}*/
}

void visioncoordinates::setStartPosition()

{
	
	debugmsg("setStartPosition()");

	double bf[MAX_SERVOS_NR]; 
	
	memset(bf, 0, sizeof(bf));
	
	std::string position = config.return_string_value("start_joint_position", SETTINGS_SECTION_NAME);

	
	std::istringstream iss(position.c_str());
	iss >> bf[0] >> bf[1] >> bf[2] >> bf[3] >> bf[4] >> bf[5] >> bf[6]; 

	
	itsSmoothGen->load_coordinates(lib::JOINT, bf[0], bf[1], bf[2], bf[3], bf[4], bf[5], bf[6], bf[7]);

	itsSmoothGen->Move();
	
	itsSmoothGen->reset();

}

/////////////////////////////////////////////////////////////////////////////////////////////
// fabryk abstrakcyjna dla zadan
/////////////////////////////////////////////////////////////////////////////////////////////
task* return_created_ecp_task (lib::configurator &_config)
{
	return new visioncoordinates(_config);
}


#undef debugmsg



} // namespace task
} // namespace common
} // namespace ecp
} // namespace mrrocpp

