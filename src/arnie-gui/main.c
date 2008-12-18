#include <glade/glade.h>
#include <gtk/gtk.h>
#include <serial_interface/connection.h>
#include <serial_interface/errno.h>
#include <serial_interface/control_table.h>
#include <assert.h>


#define HEAD_ID 16
#define TORSO_XY_ID 17
#define TORSO_XZ_ID 18
#define LEFT_SHOULDER_XZ_ID 48
#define RIGHT_SHOULDER_XZ_ID 32
#define LEFT_SHOULDER_XY_ID 49
#define RIGHT_SHOULDER_XY_ID 33
#define LEFT_SHOULDER_YZ_ID 50
#define RIGHT_SHOULDER_YZ_ID 34
#define LEFT_ELBOW_XZ_ID 51
#define RIGHT_ELBOW_XZ_ID 35
#define LEFT_HIP_YZ_ID 80
#define RIGHT_HIP_YZ_ID 64
#define LEFT_HIP_XY_ID 81
#define RIGHT_HIP_XY_ID 65
#define LEFT_HIP_XZ_ID 82
#define RIGHT_HIP_XZ_ID 66
#define LEFT_KNEE_XZ_ID 83
#define RIGHT_KNEE_XZ_ID 67
#define LEFT_FOOT_XZ_ID 84
#define RIGHT_FOOT_XZ_ID 68
#define LEFT_FOOT_YZ_ID 85
#define RIGHT_FOOT_YZ_ID 69

#define ROT_SPEED 50

const double ANGLE_CONVERSION_FACTOR = 360.0/0x3FF;
struct ftdi_context ftdic;

void quit(__attribute__((unused)) GtkWidget *widget, __attribute__((unused)) gpointer user_data)
{
  ar_io_close(&ftdic);
  gtk_main_quit();
}

void on_hscale_head_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, HEAD_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, HEAD_ID, val, &sp);
}
void on_hscale_torso_xy_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, TORSO_XY_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, TORSO_XY_ID, val, &sp);
}
void on_hscale_torso_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, TORSO_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, TORSO_XZ_ID, val, &sp);
}
void on_hscale_left_shoulder_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_SHOULDER_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_SHOULDER_XZ_ID, val, &sp);
}
void on_hscale_right_shoulder_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_SHOULDER_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_SHOULDER_XZ_ID, val, &sp);
}
void on_hscale_left_shoulder_xy_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_SHOULDER_XY_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_SHOULDER_XY_ID, val, &sp);
}
void on_hscale_right_shoulder_xy_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_SHOULDER_XY_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_SHOULDER_XY_ID, val, &sp);
}
void on_hscale_left_shoulder_yz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_SHOULDER_YZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_SHOULDER_YZ_ID, val, &sp);
}
void on_hscale_right_shoulder_yz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_SHOULDER_YZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_SHOULDER_YZ_ID, val, &sp);
}
void on_hscale_left_elbow_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_ELBOW_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_ELBOW_XZ_ID, val, &sp);
}
void on_hscale_right_elbow_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_ELBOW_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_ELBOW_XZ_ID, val, &sp);
}
void on_hscale_left_hip_yz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_HIP_YZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_HIP_YZ_ID, val, &sp);
}
void on_hscale_right_hip_yz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data)
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_HIP_YZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_HIP_YZ_ID, val, &sp);
}
void on_hscale_left_hip_xy_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_HIP_XY_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_HIP_XY_ID, val, &sp);
}
void on_hscale_right_hip_xy_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_HIP_XY_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_HIP_XY_ID, val, &sp);
}
void on_hscale_left_hip_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_HIP_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_HIP_XZ_ID, val, &sp);
}
void on_hscale_right_hip_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_HIP_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_HIP_XZ_ID, val, &sp);
}
void on_hscale_left_knee_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_KNEE_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_KNEE_XZ_ID, val, &sp);
}
void on_hscale_right_knee_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_KNEE_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_KNEE_XZ_ID, val, &sp);
}
void on_hscale_left_foot_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_FOOT_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_FOOT_XZ_ID, val, &sp);
}
void on_hscale_right_foot_xz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) {
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_FOOT_XZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_FOOT_XZ_ID, val, &sp);
}
void on_hscale_left_foot_yz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, LEFT_FOOT_YZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, LEFT_FOOT_YZ_ID, val, &sp);
}
void on_hscale_right_foot_yz_value_changed(GtkRange *range, __attribute__((unused)) gpointer user_data) 
{
  unsigned short val = (unsigned short) gtk_range_get_value(range);
  struct ar_io_status_packet sp = { 0,0,0,0 };
  ar_io_set_moving_speed(&ftdic, RIGHT_FOOT_YZ_ID, ROT_SPEED, &sp);
  ar_io_set_goal_position(&ftdic, RIGHT_FOOT_YZ_ID, val, &sp);
}

unsigned char angle_to_goal_pos(double angle)
{
  while(angle > 360.0)
    angle -= 360.0;
  while(angle < 0)
    angle += 360.0;
  return angle / ANGLE_CONVERSION_FACTOR;
}

double goal_pos_to_angle(unsigned char goal_pos)
{
  return ((double) goal_pos) * ANGLE_CONVERSION_FACTOR;
}

int main(int argc, char **argv)
{
  GladeXML *xml;
  GtkWidget *wnd_main, *hscale_head, *hscale_torso_xy, *hscale_torso_xz,
    *hscale_left_shoulder_xz, *hscale_right_shoulder_xz,
    *hscale_left_shoulder_xy, *hscale_right_shoulder_xy,
    *hscale_left_shoulder_yz, *hscale_right_shoulder_yz,
    *hscale_left_elbow_xz, *hscale_right_elbow_xz,
    *hscale_left_hip_yz, *hscale_right_hip_yz,
    *hscale_left_hip_xy, *hscale_right_hip_xy,
    *hscale_left_hip_xz, *hscale_right_hip_xz,
    *hscale_left_knee_xz, *hscale_right_knee_xz,
    *hscale_left_foot_xz, *hscale_right_foot_xz,
    *hscale_left_foot_yz, *hscale_right_foot_yz;
  gtk_init(&argc, &argv);

  xml = glade_xml_new("share/arnie-gui/gui.glade",
		      NULL,
		      NULL);

  wnd_main = glade_xml_get_widget(xml, "wnd_main");

  hscale_head = glade_xml_get_widget(xml, "hscale_head");
  hscale_torso_xy = glade_xml_get_widget(xml, "hscale_torso_xy");
  hscale_torso_xz = glade_xml_get_widget(xml, "hscale_torso_xz");
  hscale_left_shoulder_xz = glade_xml_get_widget(xml, 
						 "hscale_left_shoulder_xz");
  hscale_right_shoulder_xz = glade_xml_get_widget(xml, 
						  "hscale_right_shoulder_xz");
  hscale_left_shoulder_xy = glade_xml_get_widget(xml, 
						 "hscale_left_shoulder_xy");
  hscale_right_shoulder_xy = glade_xml_get_widget(xml, 
						  "hscale_right_shoulder_xy");
  hscale_left_shoulder_yz = glade_xml_get_widget(xml, 
						 "hscale_left_shoulder_yz");
  hscale_right_shoulder_yz = glade_xml_get_widget(xml, 
						  "hscale_right_shoulder_yz");
  hscale_left_elbow_xz = glade_xml_get_widget(xml, "hscale_left_elbow_xz");
  hscale_right_elbow_xz = glade_xml_get_widget(xml, "hscale_right_elbow_xz");
  hscale_left_hip_yz = glade_xml_get_widget(xml, "hscale_left_hip_yz");
  hscale_right_hip_yz = glade_xml_get_widget(xml, "hscale_right_hip_yz");
  hscale_left_hip_xy = glade_xml_get_widget(xml, "hscale_left_hip_xy");
  hscale_right_hip_xy = glade_xml_get_widget(xml, "hscale_right_hip_xy");
  hscale_left_hip_xz = glade_xml_get_widget(xml, "hscale_left_hip_xz");
  hscale_right_hip_xz = glade_xml_get_widget(xml, "hscale_right_hip_xz");
  hscale_left_knee_xz = glade_xml_get_widget(xml, "hscale_left_knee_xz");
  hscale_right_knee_xz = glade_xml_get_widget(xml, "hscale_right_knee_xz");
  hscale_left_foot_xz = glade_xml_get_widget(xml, "hscale_left_foot_xz");
  hscale_right_foot_xz = glade_xml_get_widget(xml, "hscale_right_foot_xz");
  hscale_left_foot_yz = glade_xml_get_widget(xml, "hscale_left_foot_yz");
  hscale_right_foot_yz = glade_xml_get_widget(xml, "hscale_right_foot_yz");

  gtk_widget_show(wnd_main);

  g_signal_connect(G_OBJECT(wnd_main), "delete-event",
		   G_CALLBACK(quit), NULL);

  int ar_rc;
  ar_rc = ar_io_open(&ftdic);
  assert(ar_rc != -1);

  g_signal_connect(G_OBJECT(hscale_head), "value-changed", G_CALLBACK(on_hscale_head_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_torso_xy), "value-changed", G_CALLBACK(on_hscale_torso_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_torso_xz), "value-changed", G_CALLBACK(on_hscale_torso_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_shoulder_xz), "value-changed", G_CALLBACK(on_hscale_left_shoulder_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_shoulder_xz), "value-changed", G_CALLBACK(on_hscale_right_shoulder_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_shoulder_xy), "value-changed", G_CALLBACK(on_hscale_left_shoulder_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_shoulder_xy), "value-changed", G_CALLBACK(on_hscale_right_shoulder_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_shoulder_yz), "value-changed", G_CALLBACK(on_hscale_left_shoulder_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_shoulder_yz), "value-changed", G_CALLBACK(on_hscale_right_shoulder_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_elbow_xz), "value-changed", G_CALLBACK(on_hscale_left_elbow_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_elbow_xz), "value-changed", G_CALLBACK(on_hscale_right_elbow_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_hip_yz), "value-changed", G_CALLBACK(on_hscale_left_hip_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_hip_yz), "value-changed", G_CALLBACK(on_hscale_right_hip_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_hip_xy), "value-changed", G_CALLBACK(on_hscale_left_hip_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_hip_xy), "value-changed", G_CALLBACK(on_hscale_right_hip_xy_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_hip_xz), "value-changed", G_CALLBACK(on_hscale_left_hip_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_hip_xz), "value-changed", G_CALLBACK(on_hscale_right_hip_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_knee_xz), "value-changed", G_CALLBACK(on_hscale_left_knee_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_knee_xz), "value-changed", G_CALLBACK(on_hscale_right_knee_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_foot_xz), "value-changed", G_CALLBACK(on_hscale_left_foot_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_foot_xz), "value-changed", G_CALLBACK(on_hscale_right_foot_xz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_left_foot_yz), "value-changed", G_CALLBACK(on_hscale_left_foot_yz_value_changed), NULL);
g_signal_connect(G_OBJECT(hscale_right_foot_yz), "value-changed", G_CALLBACK(on_hscale_right_foot_yz_value_changed), NULL);


  gtk_main();
  return 0;
}
