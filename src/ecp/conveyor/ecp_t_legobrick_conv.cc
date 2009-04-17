#include <stdio.h>

#include "lib/srlib.h"
#include "ecp/conveyor/ecp_local.h"
#include "ecp/conveyor/ecp_t_legobrick_conv.h"
//#include "ecp/conveyor/ecp_g_legobrick.h"

namespace mrrocpp {
namespace ecp {
namespace conveyor {
namespace task {

// KONSTRUKTORY
lego_brick::lego_brick(lib::configurator &_config) : task(_config)
{
	absolute_position = 0.0;
}

lego_brick::~lego_brick()
{}

// methods for ECP template to redefine in concrete classes
void lego_brick::task_initialization(void)
{
	ecp_m_robot = new ecp_conveyor_robot (*this);

	sr_ecp_msg->message("ECP loaded");

}


void lego_brick::main_task_algorithm(void)
{
	//conveyor_incremental_move ysg(*this, 100);
	common::generator::smooth gen(*this, true, true);
	gen.flush_pose_list();

	lib::POSE_SPECIFICATION ps;
	ps = lib::JOINT;

	double coordinates[MAX_SERVOS_NR];
	double vp[MAX_SERVOS_NR];
	double vk[MAX_SERVOS_NR];
	double v[MAX_SERVOS_NR];
	double a[MAX_SERVOS_NR];

	absolute_position-= 10;
	coordinates[0] = absolute_position;
	vp[0] = 0.0;
	vk[0] = 0.0;
	v[0] = 0.04;
	a[0] = 0.002;
	for(int i = 1; i < MAX_SERVOS_NR; ++i){
		vp[i] = 0.0;
		vk[i] = 0.0;
		v[i] = 0.0;
		a[i] = 0.0;
		coordinates[i] = 0.0;
	}

	gen.create_pose_list_head(ps, vp, vk, v, a, coordinates);
	//ysg.sensor_m = sensor_m;

	for(;;) {
		sr_ecp_msg->message("Ruch");

		gen.Move();
	}
}

}
} // namespace conveyor

namespace common {
namespace task {

task* return_created_ecp_task (lib::configurator &_config)
{
	return new conveyor::task::lego_brick(_config);
}

}
} // namespace common
} // namespace ecp
} // namespace mrrocpp

