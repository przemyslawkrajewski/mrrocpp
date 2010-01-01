/*
 * task/ecp_t_mboryn.cpp
 *
 *  Created on: Dec 11, 2009
 *      Author: mboryn
 */

#include "ecp_t_mboryn.h"

namespace mrrocpp {

namespace ecp {

namespace irp6ot {

namespace task {

ecp_t_mboryn::ecp_t_mboryn(mrrocpp::lib::configurator& _configurator)
//: mrrocpp::ecp::common::task::task(_configurator)
:task(_configurator)
{
	ecp_m_robot = new ecp::irp6ot::robot(*this);
	ecp_g_mboryn_ = new generator::ecp_g_mboryn(*this);

	sr_ecp_msg->message("ecp_t_mboryn::ecp_t_mboryn() fradia setup...");
	vsp_fradia = new ecp_mp::sensor::cvfradia(lib::SENSOR_CVFRADIA,"[vsp_cvfradia_servovision]",
				*this,	sizeof(lib::sensor_image_t::sensor_union_t::object_tracker_t));
	vsp_fradia->configure_sensor();

	sensor_m[lib::SENSOR_CVFRADIA] = vsp_fradia;
	ecp_g_mboryn_->sensor_m = sensor_m;
	sr_ecp_msg->message("ecp_t_mboryn::ecp_t_mboryn() finished.");
}

ecp_t_mboryn::~ecp_t_mboryn() {}

void ecp_t_mboryn::main_task_algorithm(void)
{
	printf("ecp_t_mboryn::main_task_algorithm() begin\n"); fflush(stdout);
	vsp_fradia->get_reading();
	while(vsp_fradia->from_vsp.vsp_report == lib::VSP_SENSOR_NOT_CONFIGURED){
		vsp_fradia->get_reading();
	}
	printf("ecp_t_mboryn::main_task_algorithm() 1\n"); fflush(stdout);
	ecp_g_mboryn_->Move();
	printf("ecp_t_mboryn::main_task_algorithm() 2\n"); fflush(stdout);
	ecp_termination_notice();
	printf("ecp_t_mboryn::main_task_algorithm() 3\n"); fflush(stdout);
}

} // namespace task

} // namespace irp6ot

namespace common {

namespace task {

task* return_created_ecp_task (lib::configurator &_config)
{
	return new irp6ot::task::ecp_t_mboryn(_config);
}

} // namespace task

} // namespace common


} // namespace ecp

} // namespace mrrocpp
