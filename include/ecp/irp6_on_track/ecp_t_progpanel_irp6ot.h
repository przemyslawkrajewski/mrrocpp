#if !defined(_ECP_T_PROGPANEL_IRP6OT_H)
#define _ECP_T_PROGPANEL_IRP6OT_H

#include "ecp/common/ecp_task.h"


class ecp_task_progpanel_irp6ot: public ecp_task  {
protected:
	progpanel_generator* ppg;
//	ecp_teach_in_generator* tig;

public:
	// KONSTRUKTORY
	ecp_task_progpanel_irp6ot(configurator &_config);
	~ecp_task_progpanel_irp6ot();
	
	// methods for ECP template to redefine in concrete classes
	void task_initialization(void);
	void main_task_algorithm(void);
	
};

#endif
