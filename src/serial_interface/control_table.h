/** \addtogroup serial_interface */
/*@{*/

#ifndef __AR_IO_CONTROL_TABLE_H__
#define __AR_IO_CONTROL_TABLE_H__

struct ar_io_control_table
{
  unsigned char model_number_lo : 8,
    model_number_hi : 8,
    version_of_firmware : 8,
    id : 8,
    baud_rate : 8,
    return_delay_time : 8,
    cw_angle_limit_lo : 8,
    cw_angle_limit_hi : 8,
    ccw_angle_limit_lo : 8,
    ccw_angle_limit_hi : 8,
    reserved : 8,
    highest_limit_temperature : 8,
    lowest_limit_voltage : 8,
    highest_limit_voltage : 8,
    max_torque_lo : 8,
    max_torque_hi : 8,
    status_return_level : 8,
    alarm_led : 8,
    alarm_shutdown : 8,
    reserved_2 : 8,
    down_calibration_lo : 8,
    down_calibration_hi : 8,
    up_calibration_lo : 8,
    up_calibration_hi : 8,
    torque_enable : 8,
    led : 8,
    cw_compliance_margin : 8,
    ccw_compliance_margin : 8,
    cw_compliance_slope : 8,
    ccw_compliance_slope : 8,
    goal_position_lo : 8,
    goal_position_hi : 8,
    moving_speed_lo : 8,
    moving_speed_hi : 8,
    torque_limit_lo : 8,
    torque_limit_hi : 8,
    present_position_lo : 8,
    present_position_hi : 8,
    present_speed_lo : 8,
    present_speed_hi : 8,
    present_load_lo : 8,
    present_load_hi : 8,
    present_voltage : 8,
    present_temperature : 8,
    registered_instruction : 8,
    reserved_3 : 8,
    moving : 8,
    lock : 8,
    punch_lo : 8,
    punch_hi : 8;
};

#endif

/*@}*/
