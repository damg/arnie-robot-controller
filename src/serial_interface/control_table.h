/** \addtogroup serial_interface */
/*@{*/

#ifndef __AR_IO_CONTROL_TABLE_H__
#define __AR_IO_CONTROL_TABLE_H__

struct ar_io_control_table
{
  unsigned char id,
    baud_rate,
    return_delay_time;
  unsigned short cw_angle_limit,
    ccw_angle_limit;
  unsigned char highest_limit_temperature,
    lowest_limit_voltage,
    highest_limit_voltage;
  unsigned short max_torque;
  unsigned char status_return_level,
    alarm_led,
    alarm_shutdown,
    reserved,
    torque_enable,
    led,
    cw_compliance_margin,
    ccw_compliance_margin,
    cw_compliance_slope,
    ccw_compliance_slope;
  unsigned short goal_position,
    moving_speed,
    torque_limit;
  unsigned char registered_instruction,
    lock;
  unsigned short punch;
};

#endif

/*@}*/
