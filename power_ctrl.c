
uint8_t power_control_app(uint8_t * pkt) {
	if(type == TC && ser_type == FUNCTION_MANAGEMENT_SERVICE &&  ser_subtype == 1) 
	{
		if(did == SD1)
		{
			if(fid == TURN_ON)
			{
				return R_OK;
			} 
			else if(fid == TURN_OFF)
			{
				return R_OK;
			} 
			else if(fid == RESET)
			{
				return R_OK;
			} 
			else { return R_ERROR; }
		} 
		else if(did == OBC)
		{
			if(fid == TURN_ON || fid == TURN_OFF)
			{
				return R_ERROR;
			} 
			else if(fid == RESET)
			{
				return R_OK;
			} 
			else { return R_ERROR; }
		}
		else { return R_ERROR; }
	} 
	else { return R_ERROR; }
}