#if !defined(_ECP_T_JAJKO_IRP6OT_H)
#define _ECP_T_JAJKO_IRP6OT_H

#include "ecp/common/ecp_task.h"

class ecp_task_jajko_irp6ot: public ecp_task  {
protected:
	y_egg_force_generator* yefg;

public:
	// KONSTRUKTORY
	ecp_task_jajko_irp6ot(configurator &_config);
	~ecp_task_jajko_irp6ot();
	
	// methods for ECP template to redefine in concrete classes
	void task_initialization(void);
	void main_task_algorithm(void);
	
};

#endif
